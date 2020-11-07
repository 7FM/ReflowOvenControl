#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include <Arduino.h>

#include "display.hpp"
#include "outputs.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"
#include "run_oven.hpp"
#include "servo.hpp"
#include "thermo.hpp"
#include "utils.hpp"

void runOven(IterationHandler *handleEverySecond, IterationHandler *handleEveryIteration, Temperature *maxTemp, Temperature *expectedTemp, void *statePtr) {
    // Reset counters and duty to ensure oven is off by default
    resetDutyCounter();

    // Ensure the oven is closed!
    setServoPosition(prefs.servoClosedDegrees, 1000);

    // Verify the outputs are configured
    if (!hasHeatingElement()) {
        showErrorMsgP(OUTPUTS_NOT_CONFIGURED);
        afterShowMsg();
        waitForUserAction();
        return;
    }

    drawLegend(PSTR("[Abort] Click/DClick"));

    unsigned long lastLoopTime = millis();
    uint8_t counter = 0;

    bool waitForAction = false;

    for (;;) {

        // Execute this loop every 20ms (50 times per second)
        /*
        unsigned long currentTime = millis();
        if (currentTime < lastLoopTime) {
            if (MAX_VALUE_UNSIGNED(currentTime) - lastLoopTime + currentTime < 20) {
            wait_till_20ms:
                _delay_ms(1);
                continue;
            }
        } else if (currentTime - lastLoopTime < 20) {
            goto wait_till_20ms;
        }
        */
        if ((unsigned long)(millis() - lastLoopTime) < 20) {
            _delay_ms(1);
            continue;
        }
        lastLoopTime += 20;

        // Determine if this is on a 1-second interval
        bool isOneSecondInterval = ++counter == 50;
        if (isOneSecondInterval) {
            counter = 0;
        }

        // Read the current temperature
        Temperature currentTemperature = getTemperature();
        if (NO_THERMO(currentTemperature)) {
            // Abort the reflow
            showErrorMsgP(PSTR("Abort because of thermocouple error!"));
            // Ensure that the user sees the error message!
            waitForAction = true;

            goto openOvenDoor;
            break;
        } else {
            // Try not to update everything in the same 20ms time slice
            // Update the temperature
            if (counter == 8) {
                updateProgressDisplay(currentTemperature, *expectedTemp);
            }
        }

        // Was the maximum temperature exceeded?
        if (currentTemperature > *maxTemp) {
            // Abort the reflow
            showErrorMsgP(PSTR("Aborted because of maximum temperature exceeded!"));
            // Ensure that the user sees the error message!
            waitForAction = true;

            goto openOvenDoor;
            break;
        }

        // Got a user action: he is probably in panic and trying to abort!
        if (receivedUserAction()) {
            // Abort the reflow
            showErrorMsgP(PSTR("Aborted as requested by user!"));
            // Ensure that the user sees the error message!
            waitForAction = true;

        openOvenDoor:
            // Open the oven door to cool things off
            setServoPosition(prefs.servoOpenDegrees, 3000);
            break;
        }

        if (handleEveryIteration(currentTemperature, waitForAction, statePtr) || (isOneSecondInterval && handleEverySecond(currentTemperature, waitForAction, statePtr))) {
            // Close the oven door
            setServoPosition(prefs.servoClosedDegrees, 3000);

            break;
        }
    }

    // Turn off all outputs!
    setOvenOutputs(ELEMENTS_OFF, CONVECTION_FAN_OFF, COOLING_FAN_OFF);

    if (waitForAction) {
        afterShowMsg();
        waitForUserAction();
    }
}
#endif