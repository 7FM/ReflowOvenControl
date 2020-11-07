#include <Arduino.h>
#include <avr/pgmspace.h>
#include <stdint.h>

#include "progmem_strings.hpp"

#ifdef DEBUG
const char DUMP_PROFILE_DISPLAY[] PROGMEM = "Display \"";
const char DUMP_PROFILE_TITLE[] PROGMEM = "Title \"";
#endif
const char OUTPUTS_NOT_CONFIGURED[] PROGMEM = "The oven outputs are not configured! At least one heating element is needed!";
