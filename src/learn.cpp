// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com

#include "config.h"

#ifdef LEARNING_FEATURE
#include <Arduino.h>
#include <stdint.h>

#include "buzzer.hpp"
#include "display.hpp"
#include "learn.hpp"
#include "outputs.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"
#include "run_oven.hpp"
#include "servo.hpp"
#include "utils.hpp"

#ifdef DEBUG
static const char PERFORMANCE_STR[] PROGMEM = "Score: ";
#endif

// Calculate the overall oven score
static inline uint8_t ovenScore() {
    uint8_t score;

    // Ability to hold temperature using very little power is worth 40%
    // 12% (or less) duty cycle scores all 40 points, sliding to 0 if required power is 30%
    score = map(constrain(prefs.learnedPower, 12, 30), 12, 30, 40, 0);

    // Thermal inertia is worth another 40%
    // Taking 36 seconds (or less) to gain 30C scores all the points, sliding to 0 if it takes 60 seconds or longer
    score += map(constrain(prefs.learnedInertia, 36, 60), 36, 60, 40, 0);

    // The remaining 20% is allocated towards insulation
    // A well-insulated oven will lose 30C in just over 2 minutes, and a poorly insulated one in 80 seconds or less
    score += map(constrain(prefs.learnedInsulation, 80, 150), 80, 150, 0, 20);

    // And that is the oven score!
    return score;
}

#define GOOD 0
#define OKAY 1
#define BAD 2

static const char GOOD_STR[] PROGMEM = "Good";
static const char OKAY_STR[] PROGMEM = "Okay";
static const char BAD_STR[] PROGMEM = "Bad";
static const char *const description[] PROGMEM = {GOOD_STR, OKAY_STR, BAD_STR};

static const char INERTIA_STR[] PROGMEM = "Inertia: ";
static const char PERCENTAGE_STR[] PROGMEM = "% ";

// The learned numbers are shown once the oven has completed the 1-hour learning run
static inline void showLearnedNumbers() {
    prepareShowMsg();
    DisplayProgmemPrintLn(PSTR("Done learning! Results:"));
    // Display the power required to keep the oven at a stable temperature
    DisplayProgmemPrint(PSTR("Power: "));
    DisplayPrint(prefs.learnedPower);
    DisplayProgmemPrintLn(PERCENTAGE_STR);
    // Display the time needed to reach the higher temperatures (inertia)
    DisplayProgmemPrint(INERTIA_STR);
    DisplayPrint(prefs.learnedInertia);
    DisplayProgmemPrintLn(PSTR("s "));
    // Show the emoticon that corresponds to the insulation
    DisplayProgmemPrint(PSTR("Insulation: "));
    const uint8_t result = prefs.learnedInsulation > 127 ? GOOD : prefs.learnedInsulation > 103 ? OKAY : BAD;
    DisplayProgmemPrintLn(pgm_read_ptr(description + result));

    // Display the overall oven score
    DisplayProgmemPrint(PSTR("Oven score: "));
    uint8_t score = ovenScore();
    DisplayPrint(score);
    DisplayProgmemPrint(PERCENTAGE_STR);
    DisplayProgmemPrint(PSTR("= "));
    if (score > 90) {
        DisplayProgmemPrintLn(PSTR("Excellent"));
    } else if (score > 80) {
        DisplayProgmemPrint(PSTR("Very "));
        DisplayProgmemPrintLn(GOOD_STR);
    } else if (score > 70) {
        DisplayProgmemPrintLn(GOOD_STR);
    } else if (score > 60) {
        DisplayProgmemPrintLn(OKAY_STR);
    } else {
        DisplayProgmemPrintLn(BAD_STR);
    }
    afterShowMsg();
}

static IterationHandler handleEverySecond;
static IterationHandler handleEveryIteration;

typedef enum : uint8_t {
    LEARNING_PHASE_INITIAL_RAMP,
    LEARNING_PHASE_CONSTANT_TEMP,
    LEARNING_PHASE_THERMAL_INERTIA,
    LEARNING_PHASE_INSULATION,
    LEARNING_PHASE_START_COOLING,
    LEARNING_PHASE_COOLING,
    LEARNING_PHASE_DONE,
    LEARNING_PHASE_ABORT,
} LearningStep;

#define LEARNING_SOAK_TEMP ADJUST_TEMP(120)    // Target temperature to run tests against
#define LEARNING_INERTIA_TEMP ADJUST_TEMP(150) // Target temperature to run inertia tests against

#define LEARNING_RAMP_TO_TEMP_DURATION 200  // Duration of the initial "get-to-temperature" phase
#define LEARNING_CONSTANT_TEMP_DURATION 780 // Duration of the constant temperature measurement
#define LEARNING_INERTIA_DURATION 120       // Duration of the rate-of-rise temperature measurement
#define LEARNING_COOLING_DURATION 400       // Duration of cool-down (insulation) measurement

typedef struct {
    uint16_t secondsIntoPhase = 0;
    uint8_t learningDutyCycle = 60;
    LearningStep learningPhase = LEARNING_PHASE_INITIAL_RAMP;
    uint8_t learningIntegral = 0;
    uint8_t coolingDuration = 0;
    long lastOverTempTime = 0;
    Temperature setPoint;

    // Initialize varaibles used for learning
    uint16_t secondsLeftOfLearning = LEARNING_RAMP_TO_TEMP_DURATION + LEARNING_CONSTANT_TEMP_DURATION + LEARNING_INERTIA_DURATION + LEARNING_COOLING_DURATION;
    uint16_t secondsLeftOfPhase = LEARNING_RAMP_TO_TEMP_DURATION + LEARNING_CONSTANT_TEMP_DURATION;
} CalibrationState;

static void applyDuty(uint8_t newDuty) {
    currentDuty[0] = util::MIN(newDuty, static_cast<uint8_t>(80));
    for (uint8_t i = 1; i < sizeof(currentDuty); ++i) {
        currentDuty[i] = newDuty;
    }
}

// Stay in this function until learning is done or canceled
bool learn(Menu::Action_t action, Menu::Engine &engine) {
    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    // Turn on any convection fans
    setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_ON, COOLING_FAN_OFF);

    CalibrationState state;

    Temperature maxTemp = 200;

    applyDuty(state.learningDutyCycle);

    drawInitialProgressDisplay(maxTemp);
    updateProgressDisplayState(PSTR("Ramp to 120\367C"), true);
    state.setPoint = LEARNING_SOAK_TEMP;
    updateProgressDisplaySetPoint(state.setPoint);

    maxTemp = ADJUST_TEMP(maxTemp);
    runOven(handleEverySecond, handleEveryIteration, &maxTemp, &state.setPoint, &state);

    // Undo any learned values that weren't saved (prefs are saved if learning completes successfully)
    getPrefs();

    engine.forceNewRender = true;
    return true;
}

static bool handleEverySecond(Temperature currentTemperature, bool &waitForAction, void *statePtr) {
    CalibrationState &state = *((CalibrationState *)statePtr);

    --state.secondsLeftOfLearning;
    --state.secondsLeftOfPhase;

    switch (state.learningPhase) {
        case LEARNING_PHASE_INITIAL_RAMP: {
            // Is the oven close to the desired temperature?
            Temperature i = state.setPoint - currentTemperature;
            // Reduce the duty cycle as the oven closes in on the desired temperature
            if (i < ADJUST_TEMP(30) && i > ADJUST_TEMP(15)) {
                state.learningDutyCycle = 30;
            } else if (i < ADJUST_TEMP(15)) {
                state.learningPhase = LEARNING_PHASE_CONSTANT_TEMP;
#ifdef DEBUG
                SerialPrintlnP(PSTR("Initial temperature ramp complete. Maintaining 120C using all elements"));
#endif
                updateProgressDisplayState(PSTR("Maintain 120\367C"), true);
                state.secondsIntoPhase = 0;
                state.learningDutyCycle = 15;
            }
            // Can't stay in the "ramp to temperature" phase too long
            if (state.secondsLeftOfPhase < LEARNING_CONSTANT_TEMP_DURATION) {
                showErrorMsgP(PSTR("Learning aborted because oven could not ramp up to 120\367C!"));
                waitForAction = true;
                state.learningPhase = LEARNING_PHASE_START_COOLING;
            } else {
                applyDuty(state.learningDutyCycle);
            }
            break;
        }

        case LEARNING_PHASE_CONSTANT_TEMP: {

            ++state.secondsIntoPhase;

#ifdef DEBUG
            // Give the user some indiction of how the oven is performing
            if (state.secondsIntoPhase > 20) {
                // (12% = excellent, 35% = awful)
                const uint8_t i = constrain(state.learningDutyCycle, 12, 35);
                SerialPrintP(PERFORMANCE_STR);
                SERIALprintln(map(i, 12, 35, 100, 0));
            }
#endif
            Temperature delta = state.setPoint - currentTemperature;

            // Is the oven too hot?
            if (delta < 0) {
                if (currentDuty[0] > 0) {
                    // Turn all heating elements off
                    setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_ON, COOLING_FAN_OFF);

                    // The duty cycle caused the temperature to exceed the bake temperature, so decrease it
                    // (but not more than once every 30 seconds)
                    unsigned long currentMillis = millis();
                    if ((unsigned long)(currentMillis - state.lastOverTempTime) >= 30000) {
                        state.lastOverTempTime = currentMillis;
                        if (state.learningDutyCycle > 0)
                            --state.learningDutyCycle;
                    }

                    // Reset the bake integral, so it will be slow to increase the duty cycle again
                    state.learningIntegral = 0;
#ifdef DEBUG
                    SerialPrintlnP(PSTR("Over-temp. Elements off"));
#endif
                }
            } else {
                // The oven is heating up
                // Increase the bake integral if not close to temperature
                if (delta > ADJUST_TEMP(1))
                    ++state.learningIntegral;
                if (delta > ADJUST_TEMP(5))
                    ++state.learningIntegral;

                // Has the oven been under-temperature for a while?
                if (state.learningIntegral > 30) {
                    state.learningIntegral = 0;
                    // Increase duty cycles
                    if (state.learningDutyCycle < 100)
                        ++state.learningDutyCycle;
#ifdef DEBUG
                    SerialPrintlnP(PSTR("Under-temp. Increasing duty cycle"));
#endif
                }

                applyDuty(state.learningDutyCycle);
            }

            // Time to end this phase?
            if (state.secondsLeftOfPhase == 0) {
                // Save the duty cycle needed to maintain this temperature
                prefs.learnedPower = state.learningDutyCycle;

#ifdef DEBUG
                SerialPrintlnP(PSTR("Measuring thermal inertia - how long it takes to get to 150C using all elements at 80%"));
#endif
                updateProgressDisplayState(PSTR("Thermal inertia"), true);
                state.setPoint = LEARNING_INERTIA_TEMP;
                updateProgressDisplaySetPoint(state.setPoint);
                state.learningPhase = LEARNING_PHASE_THERMAL_INERTIA;
                state.secondsLeftOfPhase = LEARNING_INERTIA_DURATION;
                prefs.learnedInertia = 0;

                // Crank up the power to 80% to see the rate-of-rise
                state.learningDutyCycle = 80;
                applyDuty(state.learningDutyCycle);
            }
            break;
        }

        case LEARNING_PHASE_THERMAL_INERTIA: {

            ++prefs.learnedInertia;

#ifdef DEBUG
            // Display the intertia score (35 seconds = excellent, 60 seconds = awful)
            // This will start "excellent" and worsen as time ticks by
            if (prefs.learnedInertia > 20) {
                const uint16_t i = constrain(prefs.learnedInertia, 36, 66);

                SerialPrintP(PERFORMANCE_STR);
                SERIALprintln(map(i, 36, 66, 100, 0));
            }
#endif

            // Has the temperature passed 150C yet?
            if (currentTemperature >= state.setPoint) {
                // Stop heating the oven
                // Turn all heating elements off
                setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_ON, COOLING_FAN_OFF);

#ifdef DEBUG
                // Update the message to show the oven is trying to stabilize around 120C again
                SerialPrintlnP(PSTR("Cooling back to 120C with all elements off. Measuring heat retention"));
#endif
                updateProgressDisplayState(PSTR("Learn insulation"), true);
                state.setPoint = LEARNING_SOAK_TEMP;
                updateProgressDisplaySetPoint(state.setPoint);

                // Move to the next phase
                state.learningPhase = LEARNING_PHASE_INSULATION;
                state.secondsLeftOfLearning = LEARNING_COOLING_DURATION;
                state.secondsLeftOfPhase = LEARNING_COOLING_DURATION;
                prefs.learnedInsulation = 0;
            }

            // Can't stay in the "ramp to temperature" phase too long
            if (state.secondsLeftOfPhase == 0) {
                showErrorMsgP(PSTR("Learning aborted because oven could not ramp up to 150\367C!"));
                waitForAction = true;
                state.learningPhase = LEARNING_PHASE_START_COOLING;
            }
            break;
        }

        case LEARNING_PHASE_INSULATION: {

            // Is the cool-down time being measured?
            if (currentTemperature >= state.setPoint && currentTemperature <= LEARNING_INERTIA_TEMP) {
                ++prefs.learnedInsulation;

#ifdef DEBUG
                // Display the performance graph if it's been more then 60 seconds of cooling
                if (prefs.learnedInsulation > 60) {
                    const uint16_t i = constrain(prefs.learnedInsulation, 80, 150);

                    SerialPrintP(PERFORMANCE_STR);
                    SERIALprintln(map(i, 80, 150, 100, 0));
                }
#endif
            }

            // A very well insulated oven could take longer than the allocated time
            if (state.secondsLeftOfLearning == 0) {
                // What happens if a relay gets stuck on?
                if (prefs.learnedInsulation == 0) {
                    // The oven is still above 150C!!!
                    state.learningPhase = LEARNING_PHASE_START_COOLING;
                    showErrorMsgP(PSTR("Learning aborted because temperature wasn't going down!"));
                    waitForAction = true;
                }

                // Extend the phase duration
                state.secondsLeftOfLearning = 60;
                state.secondsLeftOfPhase = 60;
            }

            // Has the measurement been taken (oven cooled to 120C)?
            // Abort if the insulation score is already beyond excellent
            if (currentTemperature < state.setPoint || prefs.learnedInsulation >= 300) {
                // Show the results now
                showLearnedNumbers();
                updateProgressDisplayState(PSTR("Cooling down"), true);
                // Done with measuring the oven.  Start cooling
                state.learningPhase = LEARNING_PHASE_START_COOLING;
                // Save all the learned values now
                prefs.learningComplete = LEARNING_DONE;
                // Force the writing of Prefs to flash
                writePrefsToFlash();
            }

            break;
        }

        case LEARNING_PHASE_COOLING:

            // Wait in this phase until the oven has cooled
            if (state.coolingDuration > 0)
                --state.coolingDuration;
            if (state.coolingDuration == 0 && currentTemperature < ADJUST_TEMP(50)) {
                // Turn all elements and fans off
                setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_OFF, COOLING_FAN_OFF);
                // Close the oven door now, over 3 seconds
                setServoPosition(prefs.servoClosedDegrees, 3000);
                // Stay on this screen and wait for the user tap
                state.learningPhase = LEARNING_PHASE_DONE;
                // Play a tune to let the user know baking is done
                playTones(TUNE_REFLOW_DONE);
            }
            break;

        // Ignore remaining phases because they are executed every second
        default:
            break;
    }

    return false;
}

static bool handleEveryIteration(Temperature currentTemperature, bool &waitForAction, void *statePtr) {
    CalibrationState &state = *((CalibrationState *)statePtr);

    switch (state.learningPhase) {
        case LEARNING_PHASE_START_COOLING:
            // Turn off all elements and turn on the fans
            setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_ON, COOLING_FAN_ON);

            // If a servo is attached, use it to open the door over 3 seconds
            setServoPosition(prefs.servoOpenDegrees, 3000);

            // Move to the next phase
            state.learningPhase = LEARNING_PHASE_COOLING;

            // Cooling should be at least 60 seconds in duration
            state.coolingDuration = 60;
            break;

        case LEARNING_PHASE_DONE:
            // Nothing to do here.
            // learningPhase = LEARNING_PHASE_ABORT;
            // Intentional fall through to exit loop
            //break;

        case LEARNING_PHASE_ABORT:
#ifdef DEBUG
            SerialPrintlnP(PSTR("Learning is over!"));
#endif
            // Turn all elements and fans off
            setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_OFF, COOLING_FAN_OFF);
            // Close the oven door now, over 3 seconds
            setServoPosition(prefs.servoClosedDegrees, 3000);
            // Return to the main menu
            return true;

        // Ignore remaining phases because they are executed every second
        default:
            break;
    }

    return false;
}

#endif