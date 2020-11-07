#include "config.h"

#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)

#else
#include <Arduino.h>
#include <stdint.h>

#include "pin.hpp"

void initPinHIGH(uint8_t pin) {
    uint8_t port = digitalPinToPort(pin);
    uint8_t bitMask = digitalPinToBitMask(pin);
    *portModeRegister(port) |= bitMask;
    *portOutputRegister(port) |= bitMask;
}

void initPinLOW(uint8_t pin) {
    uint8_t port = digitalPinToPort(pin);
    uint8_t bitMask = digitalPinToBitMask(pin);
    *portModeRegister(port) |= bitMask;
    *portOutputRegister(port) &= ~bitMask;
}
#endif