// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com

#include "config.h"

#ifdef REFLOW_FEATURE
#include <Arduino.h>
#include <stdint.h>

#include "buzzer.hpp"
#include "display.hpp"
#include "outputs.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"
#include "read_profiles.hpp"
#include "reflow.hpp"
#include "run_oven.hpp"
#include "sd_wrapper.hpp"
#include "servo.hpp"

#include "utils.hpp"

#ifdef DEBUG
static const char TO_DESIRED_TEMP[] PROGMEM = "to desired temperature";
#endif

static void reflow(const profiles *selectedProfile);

bool reflowHelper(Menu::Action_t action, Menu::Engine &_engine) {
    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    if (profileExitItem.Child == NULL) {
        showErrorMsgP(PSTR("No Profiles found!"));
        afterShowMsg();
        waitForUserAction();
    } else {

        // We use dynamically generated Menu entries so we have to set this flag to false!
        Menu::Engine::progMemEntries = false;

        Menu::Engine profileSelectEngine(profileExitItem.Child);

        // Reset selected flag, yes we are abusing here
        profileExitItem.Next = NULL;
        showMenu(profileSelectEngine, &profileExitItem);

        // Check if our reset flag was set!
        if (profileExitItem.Next != NULL) {

            // Per construction of profiles the address of its Menu Item should be the same!
            profiles *selected = (profiles *)profileSelectEngine.currentItemInfo.item;

            reflow(selected);
        }

        Menu::Engine::progMemEntries = true;
    }

    _engine.forceNewRender = true;
    return true;
}

// Calculate the expected power level based on the desired temperature and desired rate-of-rise
static inline uint8_t getBasePIDPower(Temperature temperature, Temperature increment, uint8_t *bias, uint8_t maxBias) {

    uint8_t adjustedTemperature = UNDO_ADJUST_TEMP(constrain(temperature, ADJUST_TEMP(29), ADJUST_TEMP(250)));
    // First, figure out the power required to maintain this temperature
    // Start by extrapolating the power using all elements at 120C
    // 120C = 100%, 150C = 125%, 200C = 166%
#ifndef MAX6675_USE_FIX_PT
    uint16_t basePower = (adjustedTemperature * prefs.learnedPower * 0.83f) / 100;
#else
    // 0b110101 is a good approximation(0.828125) for 0.83f in Q0.6 format!
    // other operands are in Q8.0 and Q7.0 format -> Q8.0 * Q7.0 * Q0.6 = Q15.6 format
    uint16_t basePower = static_cast<uint16_t>(((static_cast<uint32_t>(static_cast<uint16_t>(adjustedTemperature) * static_cast<uint16_t>(prefs.learnedPower)) * 0b110101) / 100) >> 6);
#endif
    // Adjust this number slightly based on the expected losses through the insulation. Heat losses will be higher at high temperatures
    uint16_t insulationPower = map(prefs.learnedInsulation, 0, 300, map(adjustedTemperature, 0, 400, 0, 20), 0);

    // Adjust by the desired rate-of-rise
    uint16_t risePower = UNDO_ADJUST_TEMP(increment * (basePower << 1));

    // Adjust power by the bias, since some elements may receive less than the calculated power
    // Hope that the user hasn't made the boost element receive the most power - that isn't good.
    // Example, if top/bottom bias was 50/100 and power was 40%, then power should increase to 53.33%
    // as shown below
    // No bias:
    //   top: bias @ 100% * power @ 40% ==> 40%
    //   bottom: bias @ 100% * power @ 40% ==> 40%
    //   average power: 40% + 40% / 2 ==> 40%
    // 50/100 top/bottom bias:
    //   top: bias @50% * power @ 53.33% ==> 26.66%
    //   bottom: bias @100% * power @ 53.33% ==> 53.33%
    //   average power: 26.66% + 53.33% / 2 ==> 40%
    //
    // Actual calculation:
    //   Bias factor = 2 / (Btop/Bmax + Bbottom/Bmax) = 2 * Bmax / (Btop + Bbottom)
    // totalBasePower * BiasFactor;
    // Put it all together
    uint8_t totalBasePower = (((basePower + insulationPower + risePower) << 1) * maxBias) / (bias[TYPE_BOTTOM_ELEMENT - 1] + bias[TYPE_TOP_ELEMENT - 1]);

#ifdef DEBUG
    SerialPrintP(PSTR("Base PID power at "));
    SERIALprint(temperature);
    SerialPrintP(PSTR("C:  B="));
    SERIALprint(basePower);
    SerialPrintP(PSTR(" I="));
    SERIALprint(insulationPower);
    SerialPrintP(PSTR(" R="));
    SERIALprint(risePower);
    SerialPrintP(PSTR(" Total="));
    SERIALprint(totalBasePower);
#endif

    return util::MIN(totalBasePower, static_cast<uint8_t>(100));
}

static IterationHandler handleEverySecond;
static IterationHandler handleEveryIteration;

// Reflow steps
typedef enum : uint8_t {
    REFLOW_PHASE_NEXT_COMMAND,  // Get the next command (token) in the profile
    REFLOW_WAITING_FOR_TIME,    // Waiting for a set time to pass
    REFLOW_WAITING_UNTIL_ABOVE, // Waiting until the oven temperature rises above a certain temperature
    REFLOW_WAITING_UNTIL_BELOW, // Waiting until the oven temperature drops below a certain temperature
    REFLOW_WAITING_FOR_TAP,     // Waiting for the user to tap the screen
    REFLOW_MAINTAIN_TEMP,       // Hold a specific temperature for a certain duration
    REFLOW_PID,                 // Use PID to get to the specified temperature
    REFLOW_ALL_DONE,            // All done, waiting for user to tap screen
    REFLOW_ABORT                // Abort (or done)
} ReflowStep;

typedef struct {
    // Set inital values / default values
    // Default values can be changed by the profile during execution!
    uint16_t countdownTimer = 0;
    ReflowStep reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
    uint8_t maxDeviation = DEFAULT_REFLOW_MAX_DEVIATION;
    Temperature pidPreviousError = 0;
    Temperature pidIntegral = 0;
    Temperature pidTemp = 0;
    Temperature pidTempRampDelta = 0;
    Temperature maxTemp = ADJUST_TEMP(DEFAULT_REFLOW_MAX_TEMP);
    Temperature desiredTemp = 0;
    // This value needs to be changed after creation!
    File *profile = NULL;
    // More default values
    uint8_t maxBias = util::MAXConst(util::MAXConst(DEFAULT_REFLOW_BOTTOM_ELEMENT_BIAS, DEFAULT_REFLOW_TOP_ELEMENT_BIAS), DEFAULT_REFLOW_BOOST_ELEMENT_BIAS);
    uint8_t maxDuty[3] = {DEFAULT_REFLOW_BOTTOM_ELEMENT_MAX_DUTY, DEFAULT_REFLOW_TOP_ELEMENT_MAX_DUTY, DEFAULT_REFLOW_BOOST_ELEMENT_MAX_DUTY};
    uint8_t bias[3] = {DEFAULT_REFLOW_BOTTOM_ELEMENT_BIAS, DEFAULT_REFLOW_TOP_ELEMENT_BIAS, DEFAULT_REFLOW_BOOST_ELEMENT_BIAS};
} ReflowState;

// Perform a reflow
// Stay in this function until the reflow is done or canceled
static void reflow(const profiles *selectedProfile) {

    // Make sure learning has completed
    if (prefs.learningComplete == LEARNING_NOT_DONE) {
        showErrorMsgP(PSTR("The oven characterisitics were not yet learned!"));
    reflowErrorExit:
        afterShowMsg();
        waitForUserAction();
        return;
    }

    updateStatusBar(selectedProfile->name, false);

    File profile(openPrecompiledProfile(selectedProfile->name, true));

    if (!profile) {
        showErrorMsgP(PSTR("Could not open selected profile!"));
        goto reflowErrorExit;
    }
    if (!profile.available()) {
        // Always close the profile
        profile.close();
        showErrorMsgP(PSTR("selected profile is empty!"));
        goto reflowErrorExit;
    }

    ReflowState state;
    // Set profile to use
    state.profile = &profile;

    drawInitialProgressDisplay(selectedProfile->peakTemperature);

    runOven(handleEverySecond, handleEveryIteration, &state.maxTemp, &state.pidTemp, &state);

    // Always close the profile
    profile.close();
}

static bool handleEveryIteration(Temperature currentTemperature, bool &waitForAction, void *statePtr) {
    ReflowState &state = *((ReflowState *)statePtr);

    switch (state.reflowPhase) {

        case REFLOW_ALL_DONE:
            // Nothing to do here.
            // Intentional fall through to exit loop
            //break;

        case REFLOW_ABORT:
            // All done!
            return true;

        case REFLOW_PHASE_NEXT_COMMAND: {
            // Get the next token from flash, and act on it

            uint16_t numbers[3];
            char buffer[util::MAXConst(util::MAXConst(MAX_PROFILE_NAME_LENGTH, MAX_PROFILE_DISPLAY_STR), MAX_PROFILE_TITLE_STR) + 1];
            TOKEN token = getNextToken(*state.profile, buffer, numbers);

            switch (token) {
                case TOKEN_DISPLAY:
#ifdef DEBUG
                    SerialPrintP(DUMP_PROFILE_DISPLAY);
                    SERIALprint(buffer);
                    SERIALprint('"');
                    updateProgressDisplayState(buffer, false);
                    break;
#endif

                case TOKEN_TITLE:
#ifdef DEBUG
                    SerialPrintP(DUMP_PROFILE_TITLE);
                    SERIALprint(buffer);
                    SERIALprint('"');
#endif
                    updateProgressDisplayState(buffer, false);
                    break;

                case TOKEN_MAX_DUTY:
                case TOKEN_ELEMENT_DUTY_CYCLES:
                    for (uint8_t i = 0; i < sizeof(state.maxDuty); ++i) {
                        if (token == TOKEN_MAX_DUTY) {
                            // Overwrite the default max duty cycles of the elements (max is 100%)
                            state.maxDuty[i] = util::MIN(static_cast<uint8_t>(numbers[i]), static_cast<uint8_t>(100));
                        } else {
                            // The user wants these specific values to be used
                            // Force a specific duty cycle on the elements.
                            currentDuty[i] = util::MIN(static_cast<uint8_t>(numbers[i]), static_cast<uint8_t>(state.maxDuty[i]));
                        }
                    }
                    break;

                case TOKEN_BIAS:
                    // Specify a bottom/top/boost temperature bias
                    // They cannot all be zero
                    if ((numbers[0] | numbers[1] | numbers[2]) == 0) {
                        break;
                    }

                    state.bias[TYPE_BOTTOM_ELEMENT - 1] = numbers[0];
                    state.bias[TYPE_TOP_ELEMENT - 1] = numbers[1];
                    state.maxBias = util::MAX(state.bias[TYPE_BOTTOM_ELEMENT - 1], state.bias[TYPE_TOP_ELEMENT - 1]);
                    state.bias[TYPE_BOOST_ELEMENT - 1] = numbers[2];
                    state.maxBias = util::MAX(state.maxBias, state.bias[TYPE_BOOST_ELEMENT - 1]);
                    break;

                case TOKEN_DEVIATION:
                    // Specify the delta between the target temperature and current temperature which if exceeded will
                    // cause abort. This is only used when running PID.
                    state.maxDeviation = constrain(numbers[0], 1, 100);
                    break;

                case TOKEN_MAX_TEMPERATURE:
                    // Specify the maximum temperature the oven isn't allowed to exceed
                    state.maxTemp = ADJUST_TEMP(numbers[0]);
                    break;

                case TOKEN_OVEN_DOOR_OPEN:
                case TOKEN_OVEN_DOOR_CLOSE:
                    // Open/Close the oven door over X seconds
                    numbers[0] = util::MIN(static_cast<uint8_t>(numbers[0]), static_cast<uint8_t>(30));
                    setServoPosition(*(&prefs.servoOpenDegrees + (token - TOKEN_OVEN_DOOR_OPEN)), numbers[0] * 1000);
                    break;

                case TOKEN_OVEN_DOOR_PERCENT:
                    // Open the oven door a certain percentage
                    numbers[0] = util::MIN(static_cast<uint8_t>(numbers[0]), static_cast<uint8_t>(100));
                    numbers[1] = util::MIN(static_cast<uint8_t>(numbers[1]), static_cast<uint8_t>(30));
                    setServoPosition(map(numbers[0], 0, 100, prefs.servoClosedDegrees, prefs.servoOpenDegrees), numbers[1] * 1000);
                    break;

                case TOKEN_WAIT_FOR_SECONDS:
                    // Keep the oven in this state for a number of seconds
                    state.countdownTimer = numbers[0];
                    state.reflowPhase = REFLOW_WAITING_FOR_TIME;
                    break;

                case TOKEN_WAIT_UNTIL_ABOVE_C:
                    // Wait until the oven temperature is above a certain temperature
                    state.pidTemp = state.desiredTemp = util::MIN(((Temperature)ADJUST_TEMP(numbers[0])), state.maxTemp);
                    updateProgressDisplaySetPoint(state.desiredTemp);
                    state.reflowPhase = REFLOW_WAITING_UNTIL_ABOVE;
                    break;

                case TOKEN_WAIT_UNTIL_BELOW_C:
                    // Wait until the oven temperature is below a certain temperature
                    state.pidTemp = state.desiredTemp = ADJUST_TEMP(util::MAX(static_cast<uint8_t>(numbers[0]), static_cast<uint8_t>(30)));
                    updateProgressDisplaySetPoint(state.desiredTemp);
                    state.reflowPhase = REFLOW_WAITING_UNTIL_BELOW;
                    break;

                case TOKEN_CONVECTION_FAN_ON:
                case TOKEN_CONVECTION_FAN_OFF:
                    // Turn on/off the convection fan
                    turnConvectionFanOn(token == TOKEN_CONVECTION_FAN_ON);
                    break;

                case TOKEN_COOLING_FAN_ON:
                case TOKEN_COOLING_FAN_OFF:
                    // Turn on/off the cooling fan
                    turnCoolingFanOn(token == TOKEN_COOLING_FAN_ON);
                    break;

                case TOKEN_PLAY_DONE_TUNE:
                    // Play a tune to let the user know reflow is done
                    playTones(TUNE_REFLOW_DONE);
                    break;

                case TOKEN_PLAY_BEEP:
                    // Play a tune to let the user know reflow is done
                    playTones(TUNE_REFLOW_BEEP);
                    break;

                case TOKEN_SHOW_GRAPH:
                    restartDrawProgressDisplay();
                    break;

                case TOKEN_MAINTAIN_TEMP:
                case TOKEN_TEMPERATURE_TARGET:
                    // Save the parameters
                    state.desiredTemp = util::MIN(((Temperature)ADJUST_TEMP(numbers[0])), state.maxTemp);
                    updateProgressDisplaySetPoint(state.desiredTemp);
                    state.countdownTimer = util::MAX(numbers[1], static_cast<uint16_t>(1));

                    // The temperature control is now done using PID
                    // Initialize the PID variables
                    state.pidPreviousError = 0;
                    state.pidIntegral = 0;

                    if (token == TOKEN_MAINTAIN_TEMP) {
                        state.pidTempRampDelta = 0;
                        state.pidTemp = state.desiredTemp;
                        state.reflowPhase = REFLOW_MAINTAIN_TEMP;
                    } else {
                        // Start the PID temperature at the current temperature
                        state.pidTemp = currentTemperature;
                        // Calculate a straight line between the current temperature and the desired end temperature
                        state.pidTempRampDelta = (state.desiredTemp - currentTemperature) / state.countdownTimer;
                        state.reflowPhase = REFLOW_PID;
                    }
                    break;

                case NOT_A_TOKEN:
                    // Handle error state: ensure user sees error msg
                    waitForAction = true;
                case TOKEN_END_OF_PROFILE:
                    // The end of the profile has been reached.  Reflow is done
                    // Change the STOP button to DONE
                    state.reflowPhase = REFLOW_ALL_DONE;
                    break;

                // Should not occurr so just ignore
                case TOKEN_NAME:
                case TOKEN_COMMENT1:
                case TOKEN_COMMENT2:

                    break;
            }
            break;
        }

        // Ignore remaining phases because they are executed every second
        default:
            break;
    }

    return false;
}

static bool handleEverySecond(Temperature currentTemperature, bool &waitForAction, void *statePtr) {
    ReflowState &state = *((ReflowState *)statePtr);

    --state.countdownTimer;

    switch (state.reflowPhase) {

        case REFLOW_WAITING_FOR_TIME:
            // We were waiting for a certain period of time. Have we waited long enough?
            if (state.countdownTimer == 0) {
#ifdef DEBUG
                SerialPrintlnP(PSTR("Finished waiting"));
#endif
                // Get the next command
                state.reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
            }
            break;

        case REFLOW_WAITING_UNTIL_ABOVE:
            // We were waiting for the oven temperature to rise above a certain point
            if (currentTemperature >= state.desiredTemp) {
#ifdef DEBUG
                SerialPrintP(PSTR("Heated "));
                SerialPrintlnP(TO_DESIRED_TEMP);
#endif
                // Get the next command
                state.reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
            }
            break;

        case REFLOW_WAITING_UNTIL_BELOW:
            // We were waiting for the oven temperature to drop below a certain point
            if (currentTemperature <= state.desiredTemp) {
#ifdef DEBUG
                SerialPrintP(PSTR("Cooled "));
                SerialPrintlnP(TO_DESIRED_TEMP);
#endif
                // Get the next command
                state.reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
            }
            break;

        case REFLOW_MAINTAIN_TEMP:

            // We were waiting for a certain period of time.  Have we waited long enough?
            if (state.countdownTimer == 0) {
#ifdef DEBUG
                SerialPrintlnP(PSTR("Finished maintaining temperature"));
#endif
                // Get the next command
                state.reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
                break;
            }

            // Is the oven over the desired temperature?
            if (currentTemperature >= state.desiredTemp) {
                // Turn all the elements off
                currentDuty[TYPE_BOTTOM_ELEMENT - 1] = 0;
                currentDuty[TYPE_TOP_ELEMENT - 1] = 0;
                currentDuty[TYPE_BOOST_ELEMENT - 1] = 0;
                // Reset the PID variables
                state.pidIntegral = 0;
                state.pidPreviousError = 0;
                break;
            }
            // Intentional fall-through if under temperature ...

        case REFLOW_PID: {

            // Has the desired temperature been reached? Go to the next phase then
            // The PID phase terminates when the temperature is reached, not when the
            // timer reaches zero.
            if (currentTemperature >= state.desiredTemp) {
                // Get the next command
                currentDuty[TYPE_BOTTOM_ELEMENT - 1] = 0;
                currentDuty[TYPE_TOP_ELEMENT - 1] = 0;
                currentDuty[TYPE_BOOST_ELEMENT - 1] = 0;
                state.reflowPhase = REFLOW_PHASE_NEXT_COMMAND;
                break;
            }

            // Calculate what the expected temperature should be at this point
            state.pidTemp += state.pidTempRampDelta;

            // Abort if deviated too far from the required temperature
            if (state.reflowPhase == REFLOW_PID && UNDO_ADJUST_TEMP(abs(state.pidTemp - currentTemperature)) > state.maxDeviation && state.pidTemp < state.desiredTemp) {
                // Open the oven door
                setServoPosition(prefs.servoOpenDegrees, 3000);
                showErrorMsgP(PSTR("ERROR: temperature delta exceeds maximum allowed!"));
                waitForAction = true;
                state.reflowPhase = REFLOW_ALL_DONE;
                break;
            }

            // Assume a certain power level, based on the current temperature and the rate-of-rise
            // This should be fairly accurate, and is based on the learned values
            uint8_t pidPower = getBasePIDPower(state.pidTemp, state.pidTempRampDelta, state.bias, state.maxBias);

            // Do the PID calculation now.  The base power will be adjusted a bit based on this result
            // This is the standard PID formula, using a 1-second interval
            Temperature thisError = state.pidTemp - currentTemperature;
            state.pidIntegral = state.pidIntegral + thisError;
            // We do not need to divide by the time because this should be executed each second :)
            Temperature pidDerivative = thisError - state.pidPreviousError;
            state.pidPreviousError = thisError;

            // The black magic of PID tuning!
            // Compared to most other closed-loop systems, reflow ovens are slow to respond to input, so the derivative term is the
            // most important one.  The other terms are assigned lower weights.
            // Kp = 2.  This says that if the temperature is 5 degrees too low, the power should be increased by 10%.  This doesn't
            //   sound like much (and it isn't) but heating elements are slow to heat up and cool down, so this is reasonable.
            // Ki = 0.01. This is a very small number.  It basically says that if we're under-temperature for a very long time then
            //   increase the power to the elements a tiny amount.  Having this any higher will create oscillations.
            // Kd is based on the learned inertia value and for the typical reflow oven it should be around 35.  Some resistive
            //   elements take a very long time to heat up and cool down so this will be a much higher value.
            const uint8_t Kd = map(constrain(prefs.learnedInertia, 30, 100), 30, 100, 30, 75);

#ifdef DEBUG
            // Dump these values out over USB for debugging
            SerialPrintP(PSTR("T="));
            SERIALprint(currentTemperature);
            SerialPrintP(PSTR(" P="));
            SERIALprint(state.pidTemp);
            SerialPrintP(PSTR(" D="));
            SERIALprint(state.pidTempRampDelta);
            SerialPrintP(PSTR(" E="));
            SERIALprint(thisError);
            SerialPrintP(PSTR(" I="));
            SERIALprint(state.pidIntegral);
            SerialPrintP(PSTR(" D="));
            SERIALprint(pidDerivative);
            SerialPrintP(PSTR(" Kd="));
            SERIALprintln(Kd);
#endif

            //TODO check that undo adjust is correct here!
            //TODO we might have problems here due to negative numbers x.x because ((-1 >> 2) = -1) (if arithmetic shift was perform we have to verify if that happens else we get huge problems!)
            // If we're over-temperature, it is best to slow things down even more since taking a bit longer in a phase is better than taking less time
            thisError = UNDO_ADJUST_TEMP((thisError < 0 ? 4 : 2) * thisError + state.pidIntegral / 100 + Kd * pidDerivative);

            // The base power we calculated first should be close to the required power, but allow the PID value to adjust
            // this up or down a bit. The effect PID has on the outcome is deliberately limited because moving between zero
            // (elements off) and 100 (full power) will create hot and cold spots.  PID can move the power by 60%; 30% down or up.
            thisError = constrain(thisError, -30, 30);

#ifdef DEBUG
            SerialPrintP(PSTR("Power was "));
            SERIALprint(pidPower);
            SerialPrintP(PSTR(" and is now "));
            SERIALprintln(pidPower + thisError);
#endif

            // Add the base power and the PID delta
            pidPower += thisError;

            // Make sure the resulting power is reasonable
            pidPower = constrain(pidPower, 0, 100);

            // Determine the duty cycle of each element based on the top/bottom/boost bias
            // Make sure none of the max duty cycles are exceeded
            for (uint8_t i = 0; i < sizeof(currentDuty); ++i) {
                currentDuty[i] = util::MAX(static_cast<uint8_t>((static_cast<uint16_t>(pidPower) * state.bias[i]) / state.maxBias), state.maxDuty[i]);
            }
            break;
        }

        // Ignore remaining phases because they are executed every second
        default:
            break;
    }

    return false;
}
#endif
