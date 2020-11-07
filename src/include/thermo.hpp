#pragma once

#include "config.h"

#ifdef MAX6675_USE_FIX_PT
typedef int16_t Temperature;
#define ADJUST_TEMP(x) (((Temperature)(x)) << 2)
#define UNDO_ADJUST_TEMP(x) (((Temperature)(x)) >> 2)
// The upper three bits can not be set so use this as value (with compatibility for signed)
#define NO_THERMO_VALUE 0b0110000000000000
#define NO_THERMO(x) ((x) == NO_THERMO_VALUE)
#else
typedef float Temperature;
#define ADJUST_TEMP(x) ((Temperature)(x))
#define UNDO_ADJUST_TEMP(x) ((Temperature)(x))
#define NO_THERMO_VALUE NAN
#define NO_THERMO(x) isnan(x)
#endif

void initThermo();

Temperature getTemperature();

void /*Temperature*/ thermocoupleService();