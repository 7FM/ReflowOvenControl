#pragma once

#include "config.h"
#include "makros.h"
#include "menu.hpp"

#ifdef EEPROM_SETTINGS_FEATURE
class EEPROM;

#define MAX_AVAILABLE_SPACE EEPROM.length()
// This can be changed to reduce writes of sequence numbers at the same addresses over and over again
#define START_OFFSET 0
#endif

// Outputs
#define NUMBER_OF_OUTPUTS 6
// Types of outputs
#define TYPE_UNUSED 0
#define TYPE_BOTTOM_ELEMENT 1
#define TYPE_TOP_ELEMENT 2
#define TYPE_BOOST_ELEMENT 3
#define TYPE_CONVECTION_FAN 4
#define TYPE_COOLING_FAN 5

// Number of types
#define NO_OF_TYPES 6

#define isHeatingElement(x) (x == TYPE_TOP_ELEMENT || x == TYPE_BOTTOM_ELEMENT || x == TYPE_BOOST_ELEMENT)

// Learning defines
#define LEARNING_NOT_DONE 0 // Learning has not yet been done
#define LEARNING_DONE 1     // Learning has been done
#define LEARNING_BYPASSED 2 // Learning has been manually bypassed

// Preferences (this can be 1024 bytes maximum).
typedef struct {
    uint16_t sequenceNumber;               // Prefs are rotated between 4 blocks in flash, each 4K in size
    uint8_t outputType[NUMBER_OF_OUTPUTS]; // The type of output for each of the 6 outputs
    uint8_t outputPin[NUMBER_OF_OUTPUTS];
    uint8_t invertedLogicBitMap; // BitMap (assumes <= 8 Outputs) to check for inverted logic (LOW to turn on)
    uint8_t servoPin;
    uint8_t servoOpenDegrees;    // Servo door open
    uint8_t servoClosedDegrees;  // Servo door closed
    uint16_t servoMinPulseWidth; // Servo min pulse width
    uint16_t servoMaxPulseWidth; // Servo max pulse width
    uint8_t learningComplete;    // Have the learning runs been completed?
    uint8_t learnedPower;        // The power (duty cycle) of all elements to keep the oven at 120C
    uint16_t learnedInertia;     // The time need to rasie the temperature from 120C to 150C
    uint16_t learnedInsulation;  // The insulation value of the oven
} __attribute__((packed)) Calibration;

extern Calibration prefs;

#define PREFS_SIZE (sizeof(prefs))
#define MAX_SEQUENCE_NUMBER MAX_SINGLE_BIT_VALUE_UNSIGNED(prefs.sequenceNumber)

#define NO_SERVO MAX_SINGLE_BIT_VALUE_UNSIGNED(prefs.servoPin)

#define NO_OF_PREFS_POSITIONS ((MAX_AVAILABLE_SPACE - START_OFFSET) / PREFS_SIZE)

#define MAX_WRITE_OFFSET ((NO_OF_PREFS_POSITIONS - 1) * PREFS_SIZE + START_OFFSET)

#define INVERTED_LOGIC_MASK(x) (1 << x)
#define IS_INVERTED_LOGIC(x) (prefs.invertedLogicBitMap & INVERTED_LOGIC_MASK(x))

void getPrefs();

void writePrefsToFlash();

Menu::Callback_t factoryReset;

#ifdef SD_SETTINGS_FEATURE
#define EXPORT_PREFS_PATH "/SETTINGS.BIN"
// #define EXPORT_PREFS_PATH "/settings.bin"

Menu::Callback_t exportPrefs;
Menu::Callback_t importPrefs;
#endif