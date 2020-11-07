#pragma once

#include "config.h"

#include <FastPin.h>

#define initPinHIGHConstExpr(p)       \
    FastPin<p>::setOutput(); \
    FastPin<p>::hi();

#define initPinLOWConstExpr(p) \
    FastPin<p>::setOutput();   \
    FastPin<p>::lo();


#if defined(__AVR) && defined(PIN_OPTIMIZATIONS)
//////////////////////////////////////////
// macros from http://masteringarduino.blogspot.com.es/2013/10/fastest-and-smallest-digitalread-and.html
/////////////////////////////////////////
#define portOfPin(P) \
    (((P) <= 7) ? &PORTD : (((P) <= 13) ? &PORTB : &PORTC))
#define ddrOfPin(P) \
    (((P) <= 7) ? &DDRD : (((P) <= 13) ? &DDRB : &DDRC))
#define pinOfPin(P) \
    (((P) <= 7) ? &PIND : (((P) <= 13) ? &PINB : &PINC))
#define pinIndex(P) ((uint8_t)(P > 13 ? P - 14 : P & 7))
#define pinMask(P) ((uint8_t)(1 << pinIndex(P)))

#define pinAsInput(P) *(ddrOfPin(P)) &= ~pinMask(P)
#define pinAsInputPullUp(P)        \
    *(ddrOfPin(P)) &= ~pinMask(P); \
    digitalHigh(P)
#define pinAsOutput(P) *(ddrOfPin(P)) |= pinMask(P)
#define digitalLow(P) *(portOfPin(P)) &= ~pinMask(P)
#define digitalHigh(P) *(portOfPin(P)) |= pinMask(P)
#define isHigh(P) ((*(pinOfPin(P)) & pinMask(P)) > 0)
#define isLow(P) ((*(pinOfPin(P)) & pinMask(P)) == 0)
#define digitalState(P) ((uint8_t)isHigh(P))

#define digitalToggle(P) *(portOfPin(P)) ^= pinMask(P)

#define initPinHIGH(p) \
    pinAsOutput(p);    \
    digitalHigh(p);

#define initPinLOW(p) \
    pinAsOutput(p);   \
    digitalLow(p);
#else
void initPinHIGH(uint8_t pin);

void initPinLOW(uint8_t pin);
#endif
