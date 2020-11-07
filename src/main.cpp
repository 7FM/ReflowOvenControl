#include "config.h"
#include <Arduino.h>
#include <FastPin.h>
#include <stdint.h>

#include "display.hpp"
#include "menu.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"
#include "watchdog_abuse.hpp"

#if defined(REFLOW_FEATURE) || defined(SD_SETTINGS_FEATURE) || (defined(USE_BUZZER) && defined(BUZZER_TRACKS_FROM_SD))
#include "sd_wrapper.hpp"
#endif

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include "outputs.hpp"
#include "thermo.hpp"
#endif

#ifdef REFLOW_FEATURE
#include "profile.hpp"
#include "read_profiles.hpp"
#include "reflow.hpp"
#endif

#ifdef USE_BUZZER
#include "buzzer.hpp"
#endif

#ifdef SERVO_SUPPORT
#include "servo.hpp"
#endif

#ifdef LEARNING_FEATURE
#include "learn.hpp"
#endif

void setup() {
    SERIALbegin(9600);

#ifdef USE_BUZZER
    initBuzzer();
#endif

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
    initThermo();
    initOutputs();
#endif

#ifdef SERVO_SUPPORT
    initServo();
#endif

#ifdef USE_DISPLAY
#ifdef SD_CS
    // We need to be sure that the devices that share the SPI bus are disabled!
    FastPin<SD_CS>::setOutput();
    FastPin<SD_CS>::hi();
#endif
#endif

    initDisplay();

#if defined(REFLOW_FEATURE) || defined(SD_SETTINGS_FEATURE) || (defined(USE_BUZZER) && defined(BUZZER_TRACKS_FROM_SD))
    if (!initSD()) {
        // Stay here forever
        for (;;) {
        }
    }
#endif

#ifdef REFLOW_FEATURE
    initProfiles();
#endif

    // Get the stored preferences
    getPrefs();

    // wait for MAX6675 chip to stabilize
    _delay_ms(500);

    // Finally initialize watchdog abuse as its ISR contains routines which require that everything is setup!
    initWatchdogAbuse();
}

static Menu::Engine renderEngine(Menu::Engine::getChild(&miExit));

void loop() {
    showMenu(renderEngine, &miExit);
}
