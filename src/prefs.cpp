// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com

#include <Arduino.h>
#include <stdint.h>

#include "buzzer.hpp"
#include "config.h"
#include "display.hpp"
#include "prefs.hpp"
#include "progmem_strings.hpp"
#include "utils.hpp"

#ifdef EEPROM_SETTINGS_FEATURE
#include <EEPROM.h>
#endif

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
#include "outputs.hpp"
#endif

#ifdef SD_SETTINGS_FEATURE
#include "sd_wrapper.hpp"
static void importFromSD();
#endif

#ifdef SERVO_SUPPORT
#include "servo.hpp"
#endif

// This sequence number is increased after each write to be able to identify the latest prefs.
// Blocks are initialized to 0xFF's after erase, so prefrences should be added with this in mind.

Calibration prefs;

#ifdef EEPROM_SETTINGS_FEATURE
static uint16_t lastPrefsOffset = 0;
#endif

static void resetToDefault() {
    // Initialize the whole of prefs to zero.
    memset(&prefs, 0, PREFS_SIZE);

    // Only need to initialize non-zero prefs here
    // Door servo positions
#if defined(SERVO_SUPPORT) && defined(SERVO_PIN) && SERVO_PIN != -1
    prefs.servoPin = SERVO_PIN;
#else
    prefs.servoPin = NO_SERVO;
#endif
    prefs.servoOpenDegrees = SERVO_DEFAULT_OPEN_DEGREES;
    prefs.servoClosedDegrees = SERVO_DEFAULT_CLOSE_DEGREES;
    prefs.servoMinPulseWidth = SERVO_DEFAULT_MIN_PULSE_WIDTH;
    prefs.servoMaxPulseWidth = SERVO_DEFAULT_MAX_PULSE_WIDTH;

    // Assume some default learning values, close to the oven in the build guide
    prefs.learnedPower = DEFAULT_LEARNED_POWER;
    prefs.learnedInertia = DEFAULT_LEARNED_INERTIA;
    prefs.learnedInsulation = DEFAULT_LEARNED_INSULATION;

    // We expect at least one heating element at pin SSR_EN_PIN
    prefs.outputType[0] = TYPE_BOTTOM_ELEMENT;
    prefs.outputPin[0] = SSR_EN_PIN;
#ifdef SSR_EN_INVERSE_LOGIC
    prefs.invertedLogicBitMap |= INVERTED_LOGIC_MASK(0);
#endif
}

void getPrefs() {
#ifdef EEPROM_SETTINGS_FEATURE
    // Get the prefs with the highest sequence number
    uint16_t highestSequenceNumber = 0;
    uint16_t prefOffset = START_OFFSET;
    for (uint16_t readPos = START_OFFSET; readPos <= MAX_WRITE_OFFSET; readPos += PREFS_SIZE) {
        uint16_t seqNo;
        EEPROM.get(readPos, seqNo);

        // Skip blocks that are erased
        if (seqNo == MAX_SEQUENCE_NUMBER)
            continue;
        // Is this the highest sequence number?
        if (seqNo < highestSequenceNumber)
            continue;

        // So far, this is the prefs block to use
        highestSequenceNumber = seqNo;
        prefOffset = readPos;
    }

    // Read all the prefs in now
    EEPROM.get(prefOffset, prefs);

    // If this is the first time the prefs are read in, initialize them
    if (prefs.sequenceNumber == MAX_SEQUENCE_NUMBER) {
        resetToDefault();
    }

    // Remember which offset was last used to save prefs
    lastPrefsOffset = prefOffset;

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
    initOutputs();
#endif
#ifdef SERVO_SUPPORT
    initServo();
#endif
#else
#ifdef SD_SETTINGS_FEATURE
    if (!importFromSD()) {
#endif
        resetToDefault();

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
        initOutputs();
#endif
#ifdef SERVO_SUPPORT
        initServo();
#endif
#ifdef SD_SETTINGS_FEATURE
    }
#endif
#endif
}

void writePrefsToFlash() {
#ifdef EEPROM_SETTINGS_FEATURE
    //TODO check first if settings changed?

    // Increase the preference sequence number
    ++prefs.sequenceNumber;

    // Prefs get stored in the next block (not the current one). This reduces flash wear, and adds some redundancy
    lastPrefsOffset += PREFS_SIZE;

    if (lastPrefsOffset > MAX_WRITE_OFFSET) {
        lastPrefsOffset = START_OFFSET;
    }

    // Write the prefences to EEPROM
    EEPROM.put(lastPrefsOffset, prefs);
#elif defined(SD_SETTINGS_FEATURE)
    // Else write to SD card
    exportPrefs();
#endif
}

// This performs a factory reset, erasing preferences and profiles
bool factoryReset(Menu::Action_t action, Menu::Engine &__) {

    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    //TODO dialog if you really wanna do this
#ifdef EEPROM_SETTINGS_FEATURE
    // Reset all possible positions for a sequence number by setting the max sequence value
    // This avoids overwriting all EEPROM cells
    prefs.sequenceNumber = MAX_SEQUENCE_NUMBER;
    for (uint16_t readPos = START_OFFSET; readPos <= MAX_WRITE_OFFSET; readPos += PREFS_SIZE) {
        EEPROM.put(readPos, prefs.sequenceNumber);
    }
#elif defined(SD_SETTINGS_FEATURE)
    SD.remove(EXPORT_PREFS_PATH);
#endif

    // Get the factory-default values
    resetToDefault();

    // Set the sequence number to a initial value and save the preferences
    prefs.sequenceNumber = 0;
    writePrefsToFlash();

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
    initOutputs();
#endif

#ifdef SERVO_SUPPORT
    initServo();
#endif

    // Signal action was executed might be replaced with the dialog
    playTones(TUNE_STARTUP);

    return true;
}

#ifdef SD_SETTINGS_FEATURE
bool exportPrefs(Menu::Action_t action, Menu::Engine &engine) {

    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    if (SD.exists(EXPORT_PREFS_PATH)) {
        SD.remove(EXPORT_PREFS_PATH);
    }

    File exportFile(SD.open(EXPORT_PREFS_PATH, FILE_WRITE));

    if (exportFile) {
        exportFile.write(reinterpret_cast<const uint8_t *>(&prefs), PREFS_SIZE);
        // ensure bytes are actually written!
        exportFile.flush();
        exportFile.close();

        prepareShowMsg();
        DisplayProgmemPrint(PSTR("Settings exported!"));
    } else {
        showErrorMsgP(PSTR("Setting export file could not be opened!"));
    }
    afterShowMsg();
    waitForUserAction();
    engine.forceNewRender = true;

    return true;
}

static void importFromSD() {
    File import(SD.open(EXPORT_PREFS_PATH, FILE_READ));
    if (!import) {
        showErrorMsgP(PSTR("No exported settings found!"));
    } else {
        uint16_t seqNum = prefs.sequenceNumber;
        uint8_t read = import.readBytes((uint8_t *)&prefs, PREFS_SIZE);
        import.close();
        // We need to keep the current sequence number else if we want to save current settings to flash
        // it might have a lower sequence number than previously used settings -> previous settings will be used after reboot
        prefs.sequenceNumber = seqNum;
        bool res = read == PREFS_SIZE;
        if (res) {
            prepareShowMsg();
            DisplayProgmemPrint(PSTR("Settings imported!"));
        } else {
            showErrorMsgP(PSTR("Error importing settings! Resetting to default!"));
            resetToDefault();
        }

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
        initOutputs();
#endif
#ifdef SERVO_SUPPORT
        initServo();
#endif
    }

    afterShowMsg();
    waitForUserAction();
}

bool importPrefs(Menu::Action_t action, Menu::Engine &engine) {
    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    importFromSD();
    engine.forceNewRender = true;

    return true;
}
#endif
