#pragma once

#include "config.h"

#include "thermo.hpp"
#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include <stdint.h>

typedef bool IterationHandler(Temperature currentTemperature, bool &waitForAction, void *statePtr);

void runOven(IterationHandler *handleEverySecond, IterationHandler *handleEveryIteration, Temperature *maxTemp, Temperature *expectedTemp, void *statePtr);
#endif