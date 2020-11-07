// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com

#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include <Arduino.h>
#include <stdint.h>

#include "outputs.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"

#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)
#include "pin.hpp"
#include "utils.hpp"
#else
static volatile uint8_t *outputRegs[NUMBER_OF_OUTPUTS];
static uint8_t bitMasks[NUMBER_OF_OUTPUTS];
#endif

static uint8_t outputCount = 0;
static uint8_t elementDutyCounter[NUMBER_OF_OUTPUTS];
volatile uint8_t currentDuty[3];

void resetDutyCounter() {
    // Turn off elements.
    currentDuty[TYPE_BOTTOM_ELEMENT - 1] = 0;
    currentDuty[TYPE_TOP_ELEMENT - 1] = 0;
    currentDuty[TYPE_BOOST_ELEMENT - 1] = 0;

    // Stagger the element start cycle to avoid abrupt changes in current draw
    // Simple method: there are 6 outputs but the first ones are likely the heating elements
    elementDutyCounter[0] = 65;
    for (uint8_t i = 1; i < outputCount; ++i) {
        // equivalent short version: elementDutyCounter[i] = (65 * i) % 100;
        uint8_t dutyCounter = elementDutyCounter[i - 1] + 65;
        if (dutyCounter >= 100) {
            dutyCounter -= 100;
        }
        elementDutyCounter[i] = dutyCounter;
    }
}

void updateOutputState() {
    //TODO we should probably optimize this further! and might need to reduce the time of one duty period... it is currently 2s!

    // Turn the outputs on or off based on the duty cycle
    for (uint8_t i = 0; i < outputCount; ++i) {
        const uint8_t outputType = prefs.outputType[i];
        switch (outputType) {
            case TYPE_TOP_ELEMENT:
            case TYPE_BOTTOM_ELEMENT:
            case TYPE_BOOST_ELEMENT: {
                uint8_t dutyCounter = elementDutyCounter[i];
                const uint8_t duty = currentDuty[outputType - 1];

                if (dutyCounter >= duty) {
                    // Turn it off at the right duty cycle
                    setOutputLOW(i);
                } else if (dutyCounter == 0 && duty > 0) {
                    // Turn the output on at 0, and off at the duty cycle value
                    setOutputHIGH(i);
                }

                // Increment the duty counter
                // equivalent short version: elementDutyCounter[i] = (dutyCounter + 1) % 100;
                dutyCounter += 1;
                if (dutyCounter == 100) {
                    dutyCounter = 0;
                }
                elementDutyCounter[i] = dutyCounter;

                break;
            }
        }
    }
}

// See how many outputs are used
static inline void updateOutputsConfigured() {
    uint8_t i = 0;

    for (; i < NUMBER_OF_OUTPUTS && prefs.outputType[i] != TYPE_UNUSED; ++i) {
    }

    outputCount = i;
}

// Initialize the registers controlling the outputs and turn them off
void initOutputs() {
    resetDutyCounter();

    updateOutputsConfigured();

    for (uint8_t i = 0; i < outputCount; ++i) {

#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)
        if (IS_INVERTED_LOGIC(i)) {
            initPinHIGH(prefs.outputPin[i]);
        } else {
            initPinLOW(prefs.outputPin[i]);
        }
#else
        uint8_t pin = prefs.outputPin[i];
        uint8_t port = digitalPinToPort(pin);
        uint8_t bitMask = digitalPinToBitMask(pin);

        *portModeRegister(port) |= bitMask;
        volatile uint8_t *outReg = portOutputRegister(port);
        if (IS_INVERTED_LOGIC(i)) {
            *outReg |= bitMask;
        } else {
            *outReg &= ~bitMask;
        }
        outputRegs[i] = outReg;
        bitMasks[i] = bitMask;
#endif
    }
}

void setOutputLOW(uint8_t outputNumber) {
#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)
    if (IS_INVERTED_LOGIC(outputNumber)) {
        digitalHigh(prefs.outputPin[outputNumber]);
    } else {
        digitalLow(prefs.outputPin[outputNumber]);
    }
#else
    if (IS_INVERTED_LOGIC(outputNumber)) {
        *outputRegs[outputNumber] |= bitMasks[outputNumber];
    } else {
        *outputRegs[outputNumber] &= ~bitMasks[outputNumber];
    }
#endif
}

// Turn an output on or off
void setOutputHIGH(uint8_t outputNumber) {
#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)
    if (IS_INVERTED_LOGIC(outputNumber)) {
        digitalLow(prefs.outputPin[outputNumber]);
    } else {
        digitalHigh(prefs.outputPin[outputNumber]);
    }
#else
    if (IS_INVERTED_LOGIC(outputNumber)) {
        *outputRegs[outputNumber] &= ~bitMasks[outputNumber];
    } else {
        *outputRegs[outputNumber] |= bitMasks[outputNumber];
    }
#endif
}

// Turn elements and fans on or off in one go
void setOvenOutputs(bool elementsOn, bool convectionFanOn, bool coolingFanOn) {
    if (!elementsOn) {
        // Turn off elements.
        currentDuty[TYPE_BOTTOM_ELEMENT - 1] = 0;
        currentDuty[TYPE_TOP_ELEMENT - 1] = 0;
        currentDuty[TYPE_BOOST_ELEMENT - 1] = 0;
    }

    for (uint8_t i = 0; i < outputCount; ++i) {
        switch (prefs.outputType[i]) {
            case TYPE_UNUSED:
                break;
            case TYPE_CONVECTION_FAN:
                setOutput(i, convectionFanOn);
                break;
            case TYPE_COOLING_FAN:
                setOutput(i, coolingFanOn);
                break;
            default:
                // This can only be used to turn the elements off
                if (!elementsOn)
                    setOutputLOW(i);
                break;
        }
    }
}

// Turn the convection fan on or off
void turnConvectionFanOn(bool on) {
    for (uint8_t i = 0; i < outputCount; ++i) {
        if (prefs.outputType[i] == TYPE_CONVECTION_FAN)
            setOutput(i, on);
    }
}

// Turn the cooling fan on or off
void turnCoolingFanOn(bool on) {
    for (uint8_t i = 0; i < outputCount; ++i) {
        if (prefs.outputType[i] == TYPE_COOLING_FAN)
            setOutput(i, on);
    }
}

uint8_t numOutputsConfigured() {
    return outputCount;
}

// Check to see if outputs are configured. Prevent bake/reflow if not
// At least one element must be a heating element
bool hasHeatingElement() {
    for (uint8_t i = 0; i < outputCount; ++i) {
        if (isHeatingElement(prefs.outputType[i])) {
            return true;
        }
    }
    return false;
}

#endif
