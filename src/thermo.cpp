#include "config.h"

#include <Arduino.h>
#include <max6675.h>

#include "thermo.hpp"
#include "pin.hpp"

void initThermo() {
    // use Arduino pins
#if defined(THERMO_VCC_PIN) && THERMO_VCC_PIN != -1
    initPinHIGHConstExpr(THERMO_VCC_PIN);
#endif
#if defined(THERMO_GND_PIN) && THERMO_GND_PIN != -1
    initPinLOWConstExpr(THERMO_GND_PIN);
#endif
    MAX6675<THERMO_CLK, THERMO_CS, THERMO_SO>::init();
}

static volatile Temperature temp = 0;

Temperature getTemperature() {
    Temperature ret;

    do {
        ret = temp;
    } while (ret != temp);
    // Prevent corruption when ISR updates the temp value
    // This is needed because we have a multi byte variable which takes more
    // than one cycle to be read

    return ret;
}

// This must be called every >= ~250ms
void /*Temperature*/ thermocoupleService() {
    /*return*/ temp = MAX6675<THERMO_CLK, THERMO_CS, THERMO_SO>::readCelsius();
}
