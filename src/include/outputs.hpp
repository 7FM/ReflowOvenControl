#pragma once

#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include <stdint.h>

// To be used with setOvenOutputs()
#define ELEMENTS_OFF 0
#define LEAVE_ELEMENTS_AS_IS 1
#define CONVECTION_FAN_OFF 0
#define CONVECTION_FAN_ON 1
#define COOLING_FAN_OFF 0
#define COOLING_FAN_ON 1

// Having the top element on too much can damage the insulation, or the IR can heat up dark components
// too much.  The boost element is typically not installed in the ideal location, nor designed for
// continuous use.  Restrict the amount of power given to these elements
#define MAX_TOP_DUTY_CYCLE 80
#define MAX_BOOST_DUTY_CYCLE 60

extern volatile uint8_t currentDuty[3];

void initOutputs();
void setOutputHIGH(uint8_t outputNumber);
void setOutputLOW(uint8_t outputNumber);
inline void setOutput(uint8_t outputNumber, bool enable) {
    if (enable) {
        setOutputHIGH(outputNumber);
    } else {
        setOutputLOW(outputNumber);
    }
}
void setOvenOutputs(bool elementsOn, bool convectionFanOn, bool coolingFanOn);
void turnConvectionFanOn(bool on);
void turnCoolingFanOn(bool on);
uint8_t numOutputsConfigured();
bool hasHeatingElement();

void resetDutyCounter();
void updateOutputState();
#endif