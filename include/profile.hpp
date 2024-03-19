#pragma once

#include "config.h"
#include <avr/pgmspace.h>
#include <stdint.h>

#include "display.hpp"
#include "makros.h"
#include "sd_wrapper.hpp"

#ifdef REFLOW_FEATURE
// #define PRECOMPILE_FOLDER "/"
#define PRECOMPILE_FOLDER "/PRECMP/"
// #define PRECOMPILE_FOLDER "/precmp/"
#define PRECOMPILE_FOLDER_LEN CONST_STR_LEN(PRECOMPILE_FOLDER)

#define PRECOMPILE_SUFFIX ".PRE"
// #define PRECOMPILE_SUFFIX ".pre"
#define PRECOMPILE_SUFFIX_LEN CONST_STR_LEN(PRECOMPILE_SUFFIX)

// Length of 8 because of 8.3 file name convention
#define PRECOMPILE_NAME_LEN 8

// Tokens used for profile file
enum TOKEN : uint8_t {
    NOT_A_TOKEN,           // Used to indicate end of profile (no more tokens)
    TOKEN_NAME,            // The name of the profile (max 31 characters)
    TOKEN_COMMENT1,        // Comment 1 = #
    TOKEN_COMMENT2,        // Comment 2 = //
    TOKEN_DEVIATION,       // The allowed temperature deviation before the reflow aborts
    TOKEN_MAX_TEMPERATURE, // If this temperature is ever exceeded then the reflow will be aborted and the door opened

    // TOKEN_INITIALIZE_TIMER,     // Initialize the reflow timer for logging so that comparisons can be made to datasheets
    // TOKEN_START_TIMER,          // Initialize the reflow timer for logging so that comparisons can be made to datasheets
    // TOKEN_STOP_TIMER,          // Initialize the reflow timer for logging so that comparisons can be made to datasheets
    TOKEN_MAX_DUTY,            // The highest allowed duty cycle of the elements
    TOKEN_DISPLAY,             // Display a message to the screen (progress message)
    TOKEN_OVEN_DOOR_OPEN,      // Open the oven door, over a duration in seconds
    TOKEN_OVEN_DOOR_CLOSE,     // Close the oven door, over a duration in seconds
    TOKEN_BIAS,                // The bottom/top/boost bias (weighting) for the elements
    TOKEN_CONVECTION_FAN_ON,   // Turn the convection fan on
    TOKEN_CONVECTION_FAN_OFF,  // Turn the convection fan off
    TOKEN_COOLING_FAN_ON,      // Turn the cooling fan on
    TOKEN_COOLING_FAN_OFF,     // Turn the cooling fan off
    TOKEN_TEMPERATURE_TARGET,  // The PID temperature target, and the time to get there
    TOKEN_ELEMENT_DUTY_CYCLES, // Element duty cycles can be forced (typically followed by WAIT)
    TOKEN_WAIT_FOR_SECONDS,    // Wait for the specified seconds, or until the specified temperature is reached
    TOKEN_WAIT_UNTIL_ABOVE_C,  // Wait for the specified seconds, or until the specified temperature is reached
    TOKEN_WAIT_UNTIL_BELOW_C,  // Wait for the specified seconds, or until the specified temperature is reached
    TOKEN_PLAY_DONE_TUNE,      // Play a tune to let the user things are done
    TOKEN_PLAY_BEEP,           // Play a beep
    TOKEN_OVEN_DOOR_PERCENT,   // Open the oven door a certain percentage
    TOKEN_MAINTAIN_TEMP,       // Maintain a specific temperature for a certain duration

    // TOKEN_TAP_SCREEN,          // Wait for the screen to be tapped
    // Graph is always shown but this can be used to start drawing from beginning!
    TOKEN_SHOW_GRAPH,         // Display a graph as the reflow progresses
    // TOKEN_GRAPH_DIVIDER,       // Draw a temperature line on the graph
    // TOKEN_START_PLOTTING,      // Start plotting the graph
    TOKEN_TITLE, // The title that is displayed when the profile is running
    TOKEN_END_OF_PROFILE
};

// This is the instruction set.  The instructions must be unique
static const char _token_1[] PROGMEM = "not_a_token";
static const char _token_2[] PROGMEM = "name";
static const char _token_3[] PROGMEM = "#";
static const char _token_4[] PROGMEM = "//";
static const char _token_5[] PROGMEM = "deviation";
static const char _token_6[] PROGMEM = "maximum temperature";
//static const char _token_7[] PROGMEM = "initialize timer";
//static const char _token_8[] PROGMEM = "start timer";
//static const char _token_9[] PROGMEM = "stop timer";
static const char _token_10[] PROGMEM = "maximum duty";
static const char _token_11[] PROGMEM = "display";
static const char _token_12[] PROGMEM = "open door";
static const char _token_13[] PROGMEM = "close door";
static const char _token_14[] PROGMEM = "bias";
static const char _token_15[] PROGMEM = "convection fan on";
static const char _token_16[] PROGMEM = "convection fan off";
static const char _token_17[] PROGMEM = "cooling fan on";
static const char _token_18[] PROGMEM = "cooling fan off";
static const char _token_19[] PROGMEM = "ramp temperature";
static const char _token_20[] PROGMEM = "element duty cycle";
static const char _token_21[] PROGMEM = "wait for";
static const char _token_22[] PROGMEM = "wait until above";
static const char _token_23[] PROGMEM = "wait until below";
static const char _token_24[] PROGMEM = "play tune";
static const char _token_25[] PROGMEM = "play beep";
static const char _token_26[] PROGMEM = "door percentage";
static const char _token_27[] PROGMEM = "maintain";
//static const char _token_28[] PROGMEM = "user taps screen";
static const char _token_29[] PROGMEM = "show graph";
//static const char _token_30[] PROGMEM = "graph divider";
//static const char _token_31[] PROGMEM = "start plotting";
static const char _token_32[] PROGMEM = "title";
const char *const TOKENS[] PROGMEM = {
    _token_1,
    _token_2,
    _token_3,
    _token_4,
    _token_5,
    _token_6,
    //_token_7,
    //_token_8,
    //_token_9,
    _token_10,
    _token_11,
    _token_12,
    _token_13,
    _token_14,
    _token_15,
    _token_16,
    _token_17,
    _token_18,
    _token_19,
    _token_20,
    _token_21,
    _token_22,
    _token_23,
    _token_24,
    _token_25,
    _token_26,
    _token_27,
    //_token_28,
    //_token_29,
    //_token_30,
    //_token_31,
    _token_32};

#define NUM_TOKENS (NUMELEMS(TOKENS))

// Profiles
#define MAX_PROFILE_NAME_LENGTH 25
#define MAX_PROFILE_DISPLAY_STR 20 // Maximum length of "display" string in profile file
#define MAX_PROFILE_TITLE_STR 20   // Maximum length of "title" string in profile file

typedef struct _profiles {
    Menu::Item_t item;
    char name[MAX_PROFILE_NAME_LENGTH + 1]; // Name of the profile
    uint16_t peakTemperature;               // The peak temperature of the profile
} __attribute__((packed)) profiles;

// This is needed to avoid the need for a useless iteration over all profiles to configure the MenuItems as wanted
extern Menu::Item_t profileExitItem;

profiles *createNewProfile();
void freeProfileList(profiles **profileList);
void releaseProfile(profiles *profile);
void deleteProfileFile(const char *profileName);
File openPrecompiledProfile(const char *profileName, bool read);
#endif