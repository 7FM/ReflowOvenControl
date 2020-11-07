// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com
#include "config.h"

#ifdef REFLOW_FEATURE
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <ctype.h>
#include <string.h>

#include "display.hpp"
#include "profile.hpp"
#include "progmem_strings.hpp"
#include "read_profiles.hpp"
#include "sd_wrapper.hpp"

#include "utils.hpp"

// Strings
static const char PARSE_ERROR_UNKNOWN_TOKEN[] PROGMEM = "Unknown token: ";
static const char PARSE_ERROR_MISSING_QUOTE[] PROGMEM = "Missing double-quote of string.";
static const char PARSE_ERROR_NO_NUMBER[] PROGMEM = "Token expected number could not be found.";
static const char PARSE_ERROR_INVALID_PROFILE[] PROGMEM = " Parsing failed of: ";

#ifdef DEBUG
static const char PROCESSING_FILE[] PROGMEM = "Processing file: ";
static const char SUCCESS_PROCESSING_FILE[] PROGMEM = "Successfully compiled file: ";
static const char PRINT_TOKEN_DEGREE[] PROGMEM = "C";
static const char PRINT_TOKEN_SECONDS[] PROGMEM = " seconds";
static const char PRINT_TOKEN_ON[] PROGMEM = " on";
static const char PRINT_TOKEN_OFF[] PROGMEM = " off";
static const char PRINT_TOKEN_WAIT[] PROGMEM = "Wait";
static const char PRINT_TOKEN_DEVIATION[] PROGMEM = "Deviation to abort ";
static const char PRINT_TOKEN_MAX_TEMPERATURE[] PROGMEM = "Maximum temperature ";
//static const char PRINT_TOKEN_INITIALIZE_TIMER[] PROGMEM = "Initialize timer to ";
//static const char PRINT_TOKEN_START_TIMER[] PROGMEM = "Start timer";
//static const char PRINT_TOKEN_STOP_TIMER[] PROGMEM = "Stop timer";
static const char PRINT_TOKEN_MAX_DUTY[] PROGMEM = "Maximum duty ";
static const char PRINT_TOKEN_OVEN_DOOR_OPEN[] PROGMEM = "Open door over ";
static const char PRINT_TOKEN_OVEN_DOOR_CLOSE[] PROGMEM = "Close door over ";
static const char PRINT_TOKEN_OVEN_DOOR_PERCENT_1[] PROGMEM = "Door percentage ";
static const char PRINT_TOKEN_OVEN_DOOR_PERCENT_2[] PROGMEM = "% over ";
static const char PRINT_TOKEN_BIAS[] PROGMEM = "Bias ";
static const char PRINT_TOKEN_CONVECTION_FAN[] PROGMEM = "Convection fan";
static const char PRINT_TOKEN_COOLING_FAN[] PROGMEM = "Cooling fan ";
static const char PRINT_TOKEN_TEMPERATURE_TARGET_1[] PROGMEM = "Ramp temperature to ";
static const char PRINT_TOKEN_TEMPERATURE_TARGET_2[] PROGMEM = " in ";
static const char PRINT_TOKEN_MAINTAIN_TEMP_1[] PROGMEM = "Maintain ";
static const char PRINT_TOKEN_MAINTAIN_TEMP_2[] PROGMEM = " for ";
//static const char PRINT_TOKEN_SHOW_GRAPH[] PROGMEM = "Show graph to ";
static const char PRINT_TOKEN_SHOW_GRAPH[] PROGMEM = "Show graph";
//static const char PRINT_TOKEN_GRAPH_DIVIDER[] PROGMEM = "Graph divider at ";
//static const char PRINT_TOKEN_START_PLOTTING[] PROGMEM = "Start plotting at ";
static const char PRINT_TOKEN_ELEMENT_DUTY_CYCLES[] PROGMEM = "Element duty cycle ";
static const char PRINT_TOKEN_WAIT_UNTIL_ABOVE_C[] PROGMEM = " until above ";
static const char PRINT_TOKEN_WAIT_UNTIL_BELOW_C[] PROGMEM = " until below ";
static const char PRINT_TOKEN_PLAY_DONE_TUNE[] PROGMEM = "Play tune";
static const char PRINT_TOKEN_PLAY_DONE_BEEP[] PROGMEM = "Play beep";
//static const char PRINT_TOKEN_TAP_SCREEN[] PROGMEM = "User taps screen";

static const char DUMP_PROFILE_START_OF_PROFILE[] PROGMEM = "---- Start of profile ----";
static const char DUMP_PROFILE_END_OF_PROFILE[] PROGMEM = "---- End of profile ----";
#endif

static profiles *processFile(File &file, profiles *profileList, const char *name);
static void precompileProfiles(profiles *profileList);

Menu::Item_t profileExitItem;

static bool handleProfileSelected(Menu::Action_t action, Menu::Engine &_) {

    if (action == Menu::Action_t::actionCustom) {
        // We have got a selection!
        // We abuse here the next field which should never be accessed for rendering.
        // I mean we have this memory anyway so why not use it x.x
        profileExitItem.Next = (Menu::Item_t *)1;
    }

    return true;
}

void initProfiles() {
    // Initialize out profile exit element
    profileExitItem.Label = "Select a Profile";
    profileExitItem.Previous = NULL;
    profileExitItem.Next = NULL;
    profileExitItem.Parent = &profileExitItem;
    profileExitItem.Callback = handleProfileSelected;

    // Dont look at it more abusing here :D
    precompileProfiles((profiles *)&profileExitItem);

    // Set the default value to indicate that no profiles where found and overwrite if that changed!
    profileExitItem.Child = profileExitItem.Next;
    if (profileExitItem.Child != NULL) {
        // This circumvents const correctness...
        ((profiles *)profileExitItem.Child)->item.Previous = NULL;
        
        profileExitItem.Next = NULL;
    }
}

bool ejectSD(Menu::Action_t action, Menu::Engine &engine) {
    if (action != Menu::Action_t::actionTrigger) {
        return true;
    }

    // Free current stored profiles
    freeProfileList((profiles **)&profileExitItem.Child);

    prepareShowMsg();
    DisplayProgmemPrint(PSTR("Press a button when SD card was reinserted!"));
    afterShowMsg();
    waitForUserAction();
    initProfiles();

    engine.forceNewRender = true;

    return true;
}

static inline File createPrecompiledProfileFile(const char *profileName) {
    return openPrecompiledProfile(profileName, false);
}

template <class T>
T directoryWalk(File &dir, T (*callback)(File &, T), T arg) {

    File entry;
    while ((entry = dir.openNextFile())) {

        // If this is a directory then process it (using recursion)
        if (entry.isDirectory()) {
            arg = directoryWalk(entry, callback, arg);
        } else {
            arg = callback(entry, arg);
        }

        entry.close();
    }

    return arg;
}

static profiles *profileWalkHelper(File &entry, profiles *profileList) {

#ifdef USE_SDFAT
    char name[PRECOMPILE_NAME_LEN + PRECOMPILE_SUFFIX_LEN + 1];

    if (entry.getName(name, sizeof(name))) {
#else
    const char *name = entry.name();
#endif
        int8_t length = strlen(name) - 4;
        const char *endingOffset = name + length;

        // Only look at TXT files
        if (length > 0 && (!strcmp(endingOffset, ".TXT") || !strcmp(endingOffset, ".txt"))) {
            profileList = processFile(entry, profileList, name);
        }

#ifdef USE_SDFAT
    }
#endif

    return profileList;
}

// We do not need to call mkdir for the root folder!
static constexpr bool checkMKDirNeeded() {
    return !(PRECOMPILE_FOLDER[0] == '/' && PRECOMPILE_FOLDER[1] == '\0');
}

// Read all the profiles from the SD card. The profiles can be in sub-directories
// Scan the SD card, looking for profiles
static void precompileProfiles(profiles *profileList) {
    // Ensure the target directory exists
    if (checkMKDirNeeded()) {
        SD.mkdir(PRECOMPILE_FOLDER);
    }

    // Open the root folder to look for files
    File root(SD.open("/"));

    // Look for profile files in this directory
    directoryWalk(root, profileWalkHelper, profileList);

    root.close();
}

// Read a string from the file.  The string must be contained inside double-quotes.
// Return false if the end-of-file is reached before the second double-quote is read.
// Only save up to the maximum string length, and ignore (discard) any characters over
// the maximum length
static bool getStringFromFile(File &file, char *strBuffer, uint8_t maxLength) {
    strBuffer[0] = '\0';

    while (file.available() && file.read() != '"') {
    }

    while (file.available()) {
        char c = file.read();
        // Is this the first double-quote (the start of the string)?
        if (c == '"') {
            // Is this the second double-quote (the end of the string)?
            return true;
        }

        // Have we come to the end of the line without finding a double-quote?
        if (c == 0x0A || c == 0x0D) {
            goto stringReadError;
        }

        // Save this character if the max length hasn't been exceeded
        if (maxLength) {
            *strBuffer++ = c;
            *strBuffer = '\0';
            --maxLength;
        }
    }
stringReadError:
    showErrorMsgP(PARSE_ERROR_MISSING_QUOTE);
    // We've reached the end of the file
    return false;
}

// Read a number from the file.  This method doesn't care what the delimiter is; it just
// reads until it finds a digit, and continues until it finds something that isn't a
// digit.  This reads uint16_t numbers, so they are limited to 65,536 (2^16).
// Return false if the end-of-file is reached before the number is found.
static bool getNumberFromFile(File &file, uint16_t *num) {
    bool digitFound = false;
    uint16_t readNum = 0;

    while (file.available()) {
        char c = file.read();

        bool isDigit = isdigit(c);

        if (!digitFound) {
            // Have we come to the end of the line without finding a digit?
            if (c == 0x0A || c == 0x0D) {
                goto numReadError;
            }

            // Is this the first digit?
            if (isDigit) {
                digitFound = true;
                readNum = c - '0';
            }

            // This isn't a digit, and we haven't found one yet.  Keep looking
            continue;
        }

        // We have found a number already.  Is this the delimiter?
        if (!isDigit) {
            break;
        }

        // We have found another digit of the number
        readNum = (readNum * 10) + c - '0';
    }

    if (digitFound) {
        *num = readNum;
        return true;
    }
numReadError:
    showErrorMsgP(PARSE_ERROR_NO_NUMBER);
    // We've reached the end of the file
    return false;
}

#ifdef DEBUG
// Convert the token to readable text
static void printToken(TOKEN token, const uint16_t *numbers) {
    switch (token) {
        case TOKEN_DEVIATION:
            SerialPrintP(PRINT_TOKEN_DEVIATION);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_DEGREE);
            break;
        case TOKEN_MAX_TEMPERATURE:
            SerialPrintP(PRINT_TOKEN_MAX_TEMPERATURE);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_DEGREE);
            break;
            /*
        case TOKEN_INITIALIZE_TIMER:
            SerialPrintP(PRINT_TOKEN_INITIALIZE_TIMER);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_START_TIMER:
            SerialPrintlnP(PRINT_TOKEN_START_TIMER);
            break;
        case TOKEN_STOP_TIMER:
            SerialPrintlnP(PRINT_TOKEN_STOP_TIMER);
            break;
        */
        case TOKEN_MAX_DUTY:
            SerialPrintP(PRINT_TOKEN_MAX_DUTY);
            SERIALprint(numbers[0]);
            SERIALprint('/');
            SERIALprint(numbers[1]);
            SERIALprint('/');
            SERIALprintln(numbers[2]);
            break;
        case TOKEN_OVEN_DOOR_OPEN:
            SerialPrintP(PRINT_TOKEN_OVEN_DOOR_OPEN);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_OVEN_DOOR_CLOSE:
            SerialPrintP(PRINT_TOKEN_OVEN_DOOR_CLOSE);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_OVEN_DOOR_PERCENT:
            SerialPrintP(PRINT_TOKEN_OVEN_DOOR_PERCENT_1);
            SERIALprint(numbers[0]);
            SerialPrintP(PRINT_TOKEN_OVEN_DOOR_PERCENT_2);
            SERIALprint(numbers[1]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_BIAS:
            SerialPrintP(PRINT_TOKEN_BIAS);
            SERIALprint(numbers[0]);
            SERIALprint('/');
            SERIALprint(numbers[1]);
            SERIALprint('/');
            SERIALprintln(numbers[2]);
            break;
        case TOKEN_CONVECTION_FAN_ON:
            SerialPrintP(PRINT_TOKEN_CONVECTION_FAN);
            SerialPrintlnP(PRINT_TOKEN_ON);
            break;
        case TOKEN_CONVECTION_FAN_OFF:
            SerialPrintP(PRINT_TOKEN_CONVECTION_FAN);
            SerialPrintlnP(PRINT_TOKEN_OFF);
            break;
        case TOKEN_COOLING_FAN_ON:
            SerialPrintP(PRINT_TOKEN_COOLING_FAN);
            SerialPrintlnP(PRINT_TOKEN_ON);
            break;
        case TOKEN_COOLING_FAN_OFF:
            SerialPrintP(PRINT_TOKEN_COOLING_FAN);
            SerialPrintlnP(PRINT_TOKEN_OFF);
            break;
        case TOKEN_TEMPERATURE_TARGET:
            SerialPrintP(PRINT_TOKEN_TEMPERATURE_TARGET_1);
            SERIALprint(numbers[0]);
            SerialPrintP(PRINT_TOKEN_DEGREE);
            SerialPrintP(PRINT_TOKEN_TEMPERATURE_TARGET_2);
            SERIALprint(numbers[1]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_MAINTAIN_TEMP:
            SerialPrintP(PRINT_TOKEN_MAINTAIN_TEMP_1);
            SERIALprint(numbers[0]);
            SerialPrintP(PRINT_TOKEN_DEGREE);
            SerialPrintP(PRINT_TOKEN_MAINTAIN_TEMP_2);
            SERIALprint(numbers[1]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_SHOW_GRAPH:
            SerialPrintlnP(PRINT_TOKEN_SHOW_GRAPH);
            break;
            /*
        case TOKEN_SHOW_GRAPH:
            SerialPrintP(PRINT_TOKEN_SHOW_GRAPH);
            SERIALprint(numbers[0]);
            SerialPrintP(PRINT_TOKEN_DEGREE);
            SerialPrintP(PRINT_TOKEN_MAINTAIN_TEMP_2);
            SERIALprint(numbers[1]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_GRAPH_DIVIDER:
            SerialPrintP(PRINT_TOKEN_GRAPH_DIVIDER);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_DEGREE);
            break;
        case TOKEN_START_PLOTTING:
            SerialPrintP(PRINT_TOKEN_START_PLOTTING);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
            */
        case TOKEN_ELEMENT_DUTY_CYCLES:
            SerialPrintP(PRINT_TOKEN_ELEMENT_DUTY_CYCLES);
            SERIALprint(numbers[0]);
            SERIALprint('/');
            SERIALprint(numbers[1]);
            SERIALprint('/');
            SERIALprintln(numbers[2]);
            break;
        case TOKEN_WAIT_FOR_SECONDS:
            SerialPrintP(PRINT_TOKEN_WAIT);
            SerialPrintP(PRINT_TOKEN_MAINTAIN_TEMP_2);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_SECONDS);
            break;
        case TOKEN_WAIT_UNTIL_ABOVE_C:
            SerialPrintP(PRINT_TOKEN_WAIT);
            SerialPrintP(PRINT_TOKEN_WAIT_UNTIL_ABOVE_C);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_DEGREE);
            break;
        case TOKEN_WAIT_UNTIL_BELOW_C:
            SerialPrintP(PRINT_TOKEN_WAIT);
            SerialPrintP(PRINT_TOKEN_WAIT_UNTIL_BELOW_C);
            SERIALprint(numbers[0]);
            SerialPrintlnP(PRINT_TOKEN_DEGREE);
            break;
        case TOKEN_PLAY_DONE_TUNE:
            SerialPrintlnP(PRINT_TOKEN_PLAY_DONE_TUNE);
            break;
        case TOKEN_PLAY_BEEP:
            SerialPrintlnP(PRINT_TOKEN_PLAY_DONE_BEEP);
            break;
            /*
        case TOKEN_TAP_SCREEN:
            SerialPrintlnP(PRINT_TOKEN_TAP_SCREEN);
            break;
            */
    }
}
#endif

static bool saveTokenAndData(File &target, uint8_t token, const char *data, uint8_t length) {
    if (!target) {
        showErrorMsgP(PSTR("Name token must come before any other tokens!"));
        return false;
    }

    // Save the token
    target.write(token);
    // Save the data
    target.write(data, length);

    return true;
}

// Save the token and its parameters to flash
static inline bool saveTokenAndNumbers(File &target, uint8_t token, const uint16_t *numbers, uint8_t numOfNumbers) {
    return saveTokenAndData(target, token, (const char *)numbers, sizeof(*numbers) * numOfNumbers);
}

// Save a string to flash.  "Display" and "Title" take a string as
// a parameter.  The string is saved null-terminated.
static inline bool saveTokenAndString(File &target, uint8_t token, const char *str) {
    return saveTokenAndData(target, token, str, strlen(str) + 1);
}

// Set the tokens back to the beginning to continue the search for them
static inline void initTokenPtrs(const char *tokenPtr[NUM_TOKENS]) {
    for (uint8_t i = 0; i < NUM_TOKENS; ++i) {
        tokenPtr[i] = (const char *)pgm_read_word(TOKENS + i);
    }
}

// If the given character matches the required character in the token then advance
// the token search.  Return the found token if this was the last character
static inline TOKEN hasTokenBeenFound(const char *tokenPtr[NUM_TOKENS], char c) {
    // Make the token search case-insensitive
    c = tolower(c);
    // Start this loop at 1 because the first "token" is not-a-token
    for (uint8_t i = 1; i < NUM_TOKENS; ++i) {
        // Does this character match?
        if (c == pgm_read_byte(tokenPtr[i])) {
            // Advance the token pointer
            ++tokenPtr[i];
            // Is this the end of the string?
            if (pgm_read_byte(tokenPtr[i]) == 0) {
                return (TOKEN)i;
            }
        } else {
            // The character didn't match.  Reset the token pointer
            tokenPtr[i] = (const char *)pgm_read_word(TOKENS + i);
        }
    }
    // This wasn't the last character of any of the tokens
    return NOT_A_TOKEN;
}

// Process a file with a TXT extension
static inline profiles *processFile(File &file, profiles *profileList, const char *name) {

#ifdef DEBUG
    SerialPrintP(PROCESSING_FILE);
    SERIALprintln(name);
#endif

    uint16_t numbers[3]; // Array used to store numbers read from the file

    profiles *newProfile = NULL;
    File precompiledProfile;

    const char *tokenPtr[NUM_TOKENS];
    char buffer[util::MAXConst(util::MAXConst(MAX_PROFILE_NAME_LENGTH, MAX_PROFILE_DISPLAY_STR), MAX_PROFILE_TITLE_STR) + 1];

    // Reset the token search
    initTokenPtrs(tokenPtr);

    // Keep reading characters until the entire file has been processed
    while (file.available()) {
        // See if this character resulted in a token being found
        TOKEN token = hasTokenBeenFound(tokenPtr, file.read());
        if (token == NOT_A_TOKEN)
            continue;

        // A token was found!
        // Reset the token pointers now (before we forget)
        initTokenPtrs(tokenPtr);

        uint8_t readNumCount = 0;

        switch (token) {
            case TOKEN_NAME:
                // Has a name been extracted from the file already?
                if (newProfile != NULL) {
                    showErrorMsgP(PSTR("Duplicate Name token!"));
                    goto tokenError;
                }
                // Get the name of the profile
                if (!getStringFromFile(file, buffer, MAX_PROFILE_NAME_LENGTH)) {
                    goto tokenError;
                }

                // Create a new profile
                newProfile = createNewProfile();
                if (newProfile == NULL) {
                    showErrorMsgP(PSTR("Could not allocate another profile!"));
                    goto tokenError;
                }
                // Save the name
                strncpy(newProfile->name, buffer, MAX_PROFILE_NAME_LENGTH);

                precompiledProfile = createPrecompiledProfileFile(newProfile->name);

                if (!precompiledProfile) {
                    showErrorMsgP(PSTR("Could not create precompile file!"));
                    goto tokenError;
                }

                break;

            case TOKEN_COMMENT1:
            case TOKEN_COMMENT2:
                // Discard everything until a new line character
                while (file.available()) {
                    char c = file.read();
                    if (c == '\n' || c == '\r')
                        break;
                }
                break;

            case TOKEN_DISPLAY:
            case TOKEN_TITLE:
                // This should be followed by a string that should be displayed
                if (!getStringFromFile(file, buffer, token == TOKEN_DISPLAY ? MAX_PROFILE_DISPLAY_STR : MAX_PROFILE_TITLE_STR)) {
                    goto tokenError;
                }
                // Save the display string
                if (!saveTokenAndString(precompiledProfile, token, buffer)) {
                    goto tokenError;
                }
                break;

                // Those needs 3 numbers => fall through tree times
            case TOKEN_MAX_DUTY:
            case TOKEN_ELEMENT_DUTY_CYCLES:
            case TOKEN_BIAS:
                ++readNumCount;
                // Intentional fall through

                // Those needs 2 numbers => fall through twice
            //case TOKEN_SHOW_GRAPH:
            case TOKEN_TEMPERATURE_TARGET:
            case TOKEN_OVEN_DOOR_PERCENT:
            case TOKEN_MAINTAIN_TEMP:
                ++readNumCount;
                // Intentional fall through

                // Those needs 2 numbers => fall through once
            case TOKEN_DEVIATION:
            case TOKEN_MAX_TEMPERATURE:
            //case TOKEN_INITIALIZE_TIMER:
            //case TOKEN_GRAPH_DIVIDER:
            //case TOKEN_START_PLOTTING:
            case TOKEN_OVEN_DOOR_OPEN:
            case TOKEN_OVEN_DOOR_CLOSE:
            case TOKEN_WAIT_FOR_SECONDS:
            case TOKEN_WAIT_UNTIL_ABOVE_C:
            case TOKEN_WAIT_UNTIL_BELOW_C:
                ++readNumCount;
                // Intentional fall through

            // Those needs 0 numbers => no fall through readNumCount should be unchanged = 0
            case TOKEN_SHOW_GRAPH: // abuse old meaning but does not require any numbers anymore
            //case TOKEN_START_TIMER:
            //case TOKEN_STOP_TIMER:
            //case TOKEN_TAP_SCREEN:
            case TOKEN_CONVECTION_FAN_ON:
            case TOKEN_CONVECTION_FAN_OFF:
            case TOKEN_COOLING_FAN_ON:
            case TOKEN_COOLING_FAN_OFF:
            case TOKEN_PLAY_DONE_TUNE:
            case TOKEN_PLAY_BEEP:
                for (uint8_t i = 0; i < readNumCount; ++i) {
                    if (!getNumberFromFile(file, numbers + i)) {
                        goto tokenError;
                    }
                }
                // Save the oven open/close to flash
                if (!saveTokenAndNumbers(precompiledProfile, token, numbers, readNumCount)) {
                    goto tokenError;
                }

                // This could be the peak temperature
                if ((token == TOKEN_WAIT_UNTIL_ABOVE_C || token == TOKEN_TEMPERATURE_TARGET || token == TOKEN_MAINTAIN_TEMP) &&
                    numbers[0] > newProfile->peakTemperature)
                    newProfile->peakTemperature = numbers[0];
                break;

            case NOT_A_TOKEN:
            case TOKEN_END_OF_PROFILE:
                // Should never get here but if so just ignore
                break;
        }
    }

    // Done reading the file
    if (newProfile != NULL) {
        newProfile->item.Previous = (Menu::Item_t *)profileList;
        profileList->item.Next = (Menu::Item_t *)newProfile;
        profileList = newProfile;
        precompiledProfile.flush();
        precompiledProfile.close();

#ifdef DEBUG
        SerialPrintP(SUCCESS_PROCESSING_FILE);
        SERIALprintln(name);
#endif
    }
    return profileList;

tokenError:
    DisplayProgmemPrint(PARSE_ERROR_INVALID_PROFILE);
    DisplayPrint(name);
    // If there was any error, throw the entire thing away.  Better that the user see that the profile
    // wasn't read than it was read - but not knowing if it was read correctly or not.
    // Unfortunately this doesn't take into account incorrectly spelled or ordered tokens (e.g. "door close" instead of "close door")

    // Delete this profile
    if (precompiledProfile) {
        precompiledProfile.close();
        // This implies newProfile != NULL
        deleteProfileFile(newProfile->name);
    }
    if (newProfile != NULL) {
        releaseProfile(newProfile);
    }

    afterShowMsg();
    waitForUserAction();

    return profileList;
}

// Get the next token from precompiled profile.
TOKEN getNextToken(File &profile, char *str, uint16_t *num) {

    if (!profile.available()) {
        return TOKEN_END_OF_PROFILE;
    }

    // Figure out what token parameters need to be returned
    TOKEN token = (TOKEN)profile.read();
    uint8_t readNums = 0;
    switch (token) {
        case TOKEN_TITLE:
        case TOKEN_DISPLAY: {
            // Read string which is null terminated
            uint8_t read = profile.readBytesUntil('\0', str, token == TOKEN_DISPLAY ? MAX_PROFILE_DISPLAY_STR : MAX_PROFILE_TITLE_STR);
            str[read] = '\0';
            if (profile.peek() == '\0') {
                profile.read();
            }
            break;
        }

        case TOKEN_MAX_DUTY:
        case TOKEN_ELEMENT_DUTY_CYCLES:
        case TOKEN_BIAS:
            // This should be followed by 3 numbers, indicating bottom/top/boost
            ++readNums;
            // Intentional fall through

        //case TOKEN_SHOW_GRAPH:
        case TOKEN_TEMPERATURE_TARGET:
        case TOKEN_OVEN_DOOR_PERCENT:
        case TOKEN_MAINTAIN_TEMP:
            // This should be followed by 2 numbers
            ++readNums;
            // Intentional fall through

        //case TOKEN_INITIALIZE_TIMER:
        //case TOKEN_START_PLOTTING:
        //case TOKEN_GRAPH_DIVIDER:
        case TOKEN_DEVIATION:
        case TOKEN_MAX_TEMPERATURE:
        case TOKEN_OVEN_DOOR_OPEN:
        case TOKEN_OVEN_DOOR_CLOSE:
        case TOKEN_WAIT_FOR_SECONDS:
        case TOKEN_WAIT_UNTIL_ABOVE_C:
        case TOKEN_WAIT_UNTIL_BELOW_C:
            // These require 1 parameter
            ++readNums;
            // Intentional fall through

        case TOKEN_SHOW_GRAPH: // abuse old meaning but does not require any numbers anymore
        //case TOKEN_START_TIMER:
        //case TOKEN_STOP_TIMER:
        //case TOKEN_TAP_SCREEN:
        case TOKEN_CONVECTION_FAN_ON:
        case TOKEN_CONVECTION_FAN_OFF:
        case TOKEN_COOLING_FAN_ON:
        case TOKEN_COOLING_FAN_OFF:
        case TOKEN_PLAY_DONE_TUNE:
        case TOKEN_PLAY_BEEP:
            // These don't take parameters

            // Intentional fall through

        case TOKEN_END_OF_PROFILE:
            // Nothing to do, but don't advance over this token

            // For fall through options read needed count of numbers
            profile.readBytes((uint8_t *)num, sizeof(*num) * readNums);
            break;

        case NOT_A_TOKEN:
        case TOKEN_NAME:
        case TOKEN_COMMENT1:
        case TOKEN_COMMENT2:
            // default:
            // Should never get here
            showErrorMsgP(PARSE_ERROR_UNKNOWN_TOKEN);
            DisplayPrint(token);
            // Signal that an error happened!
            token = NOT_A_TOKEN;
            break;
    }
    return token;
}

#ifdef DEBUG
// Dump profile for debugging
void dumpProfile(const char *profileName) {
    uint16_t numbers[4];
    char buffer100Bytes[100];

    File profile = openPrecompiledProfile(profileName, true);

    if (!profile) {
        return;
    }

    SerialPrintlnP(DUMP_PROFILE_START_OF_PROFILE);

    while (profile.available()) {
        // Get the next token
        TOKEN token = getNextToken(profile, buffer100Bytes, numbers);

        // Display the token and parameters
        switch (token) {
            case TOKEN_DISPLAY:
                SerialPrintP(DUMP_PROFILE_DISPLAY);
                SERIALprint(buffer100Bytes);
                SERIALprintln('"');
                break;

            case TOKEN_TITLE:
                SerialPrintP(DUMP_PROFILE_TITLE);
                SERIALprint(buffer100Bytes);
                SERIALprintln('"');
                break;

            case TOKEN_MAX_DUTY:
            case TOKEN_ELEMENT_DUTY_CYCLES:
            case TOKEN_BIAS:
                // This should be followed by 3 numbers, indicating bottom/top/boost
                printToken(token, numbers);
                break;

            case TOKEN_TEMPERATURE_TARGET:
            case TOKEN_OVEN_DOOR_PERCENT:
            case TOKEN_MAINTAIN_TEMP:
            case TOKEN_SHOW_GRAPH:
                // These should be followed by 2 numbers
                printToken(token, numbers);
                break;

            case TOKEN_DEVIATION:
            case TOKEN_MAX_TEMPERATURE:
            //case TOKEN_INITIALIZE_TIMER:
            //case TOKEN_GRAPH_DIVIDER:
            //case TOKEN_START_PLOTTING:
            case TOKEN_OVEN_DOOR_OPEN:
            case TOKEN_OVEN_DOOR_CLOSE:
            case TOKEN_WAIT_FOR_SECONDS:
            case TOKEN_WAIT_UNTIL_ABOVE_C:
            case TOKEN_WAIT_UNTIL_BELOW_C:
                // These require 1 parameter
                printToken(token, numbers);
                break;

            case TOKEN_CONVECTION_FAN_ON:
            //case TOKEN_START_TIMER:
            //case TOKEN_STOP_TIMER:
            //case TOKEN_TAP_SCREEN:
            case TOKEN_CONVECTION_FAN_OFF:
            case TOKEN_COOLING_FAN_ON:
            case TOKEN_COOLING_FAN_OFF:
            case TOKEN_PLAY_DONE_TUNE:
            case TOKEN_PLAY_BEEP:
                // These don't take parameters
                printToken(token, numbers);
                break;

            case TOKEN_END_OF_PROFILE:
                SerialPrintlnP(DUMP_PROFILE_END_OF_PROFILE);
                goto done_reading;

            case NOT_A_TOKEN:
            default:
                // Should never get here
                SerialPrintP(PARSE_ERROR_UNKNOWN_TOKEN);
                SERIALprintln(token);
                goto done_reading;
        }
    }

done_reading:
    profile.close();
}
#endif

#endif
