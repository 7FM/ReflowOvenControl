#pragma once

#include "config.h"

#include <avr/pgmspace.h>


#ifdef DISABLE_SERIAL
#define SERIALprint(...)

#define SERIALprintln(...)

#define SERIALavailable() 0

#define SERIALparseInt(...) 0

#define SERIALwrite(...)

#define SERIALbegin(...)

#else
#define SERIALprint(...) \
    Serial.print(__VA_ARGS__)

#define SERIALprintln(...) \
    Serial.println(__VA_ARGS__)

#define SERIALavailable() \
    Serial.available()

#define SERIALparseInt(...) \
    Serial.parseInt(__VA_ARGS__)

#define SERIALwrite(...) \
    Serial.write(__VA_ARGS__)

#define SERIALbegin(...) \
    Serial.begin(__VA_ARGS__)

#endif

#ifdef DEBUG
extern const char DUMP_PROFILE_DISPLAY[] PROGMEM;
extern const char DUMP_PROFILE_TITLE[] PROGMEM;
#endif
extern const char OUTPUTS_NOT_CONFIGURED[] PROGMEM;

#define SerialPrintP(str) SERIALprint(reinterpret_cast<const __FlashStringHelper *>(str))
#define SerialPrintlnP(str) SERIALprintln(reinterpret_cast<const __FlashStringHelper *>(str))
