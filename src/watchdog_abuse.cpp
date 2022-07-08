#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include "outputs.hpp"
#endif
#include "display.hpp"
#include "thermo.hpp"
#include "watchdog_abuse.hpp"

#include <Arduino.h>

#define WATCHDOG_PRESCALE 4 // 2,4,8,16,32,64,128,256,512,1024

// Time-out Interval:
// 16ms
#if WATCHDOG_PRESCALE == 2
#define WATCHDOG_PRESCALE_MASK 0

// 32ms
#elif WATCHDOG_PRESCALE == 4
#define WATCHDOG_PRESCALE_MASK _BV(WDP0)

// 64ms
#elif WATCHDOG_PRESCALE == 8
#define WATCHDOG_PRESCALE_MASK _BV(WDP1)

// 125ms
#elif WATCHDOG_PRESCALE == 16
#define WATCHDOG_PRESCALE_MASK _BV(WDP0) | _BV(WDP1)

// 250ms
#elif WATCHDOG_PRESCALE == 32
#define WATCHDOG_PRESCALE_MASK _BV(WDP2)

// 500ms
#elif WATCHDOG_PRESCALE == 64
#define WATCHDOG_PRESCALE_MASK _BV(WDP2) | _BV(WDP0)

// 1s
#elif WATCHDOG_PRESCALE == 128
#define WATCHDOG_PRESCALE_MASK _BV(WDP2) | _BV(WDP1)

// 2s
#elif WATCHDOG_PRESCALE == 256
#define WATCHDOG_PRESCALE_MASK _BV(WDP2) | _BV(WDP1) | _BV(WDP0)

// 4s
#elif WATCHDOG_PRESCALE == 512
#define WATCHDOG_PRESCALE_MASK _BV(WDP3)

// 8s
#elif WATCHDOG_PRESCALE == 1024
#define WATCHDOG_PRESCALE_MASK _BV(WDP3) | _BV(WDP0)

#else
#error "Invalid watchdog prescale!"
#endif

void initWatchdogAbuse() {
    noInterrupts();

    // reset the watchdog timer
    __asm__ __volatile__("wdr");
    // We need to clear WDRF before we can clear WDE
    MCUSR &= ~_BV(WDRF);
    // Start timed sequence
    WDTCSR = _BV(WDCE) | _BV(WDE);
    // Overwrite bits with the wanted configuration
    // Set new prescaler(time-out) and enable interrupt mode
    WDTCSR = _BV(WDIF) | _BV(WDIE) | WATCHDOG_PRESCALE_MASK;

    interrupts();
}

static uint8_t thermoReadCnt = 0;
// static uint8_t displayUpdateCnt = 0;

// ISR_NOBLOCK let the compiler reenable interrupts ASAP
//TODO this might lead to problems?
ISR(WDT_vect, ISR_NOBLOCK /*__attribute__((flatten))*/) {

    // TODO routines here
#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
    updateOutputState();
#endif

#if defined(USE_DISPLAY) && defined(ROTARY_ISR_SERVICE)
    encoderService();
#endif

    // This must be called every >= ~250ms
    if (++thermoReadCnt >= 32 / WATCHDOG_PRESCALE) {
        thermoReadCnt = 0;
        /*Temperature t = */thermocoupleService();
        /*
        // Mod 4 to ensure the display update occurrs only once per ~1s
        displayUpdateCnt = (displayUpdateCnt + 1) & 3;
        if (displayUpdateCnt == 0) {
            //TODO this creates artifacts when interrupting other rendering!
            updateTemperature(t);
        }
        */
    }
}
