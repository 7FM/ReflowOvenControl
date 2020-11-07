#include "config.h"

#include "buzzer.hpp"
#include "display.hpp"
#include "menu.hpp"
#include "prefs.hpp"

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

#ifdef SERVO_SUPPORT
#include "servo.hpp"
#endif

#ifdef LEARNING_FEATURE
#include "learn.hpp"
#endif

// ===================================================================================================
// Main menu entries

// Name, Label, Next, Previous, Parent, Child, Callback
MenuItemPtr(miExit, "Main Menu", NULL, NULL, &miExit, miMainMenu, NULL);

#ifdef REFLOW_FEATURE
MenuItemPtr(miReflow, "Start Reflow", miReflowNext, miReflowPrev, &miExit, NULL, reflowHelper);
#endif
#ifdef LEARNING_FEATURE
MenuItemPtr(miLearn, "Learn characteristics", miLearnNext, miLearnPrev, &miExit, NULL, learn);
#endif
MenuItemPtr(miSettings, "Settings", miSettingsNext, miSettingsPrev, &miExit, miSettingsSubMenu, NULL);
#ifdef PREF_SETTINGS
#ifdef SERVO_SUPPORT
MenuItemPtr(miServo, "Servo Settings", miServoNext, miServoPrev, &miExit, miServoSubMenu, NULL);
#endif
#ifdef OUTPUT_SETTINGS
#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)
MenuItemPtr(miOutput, "Output Settings", miOutputNext, miOutputPrev, &miExit, miOutputSubMenu, NULL);
#endif
#endif
#endif
#ifdef REFLOW_FEATURE
MenuItemPtr(miEject, "Eject SD", miEjectNext, miEjectPrev, &miExit, NULL, ejectSD);
#endif

#if defined(USE_BUZZER) && defined(BUZZER_SOUNDTRACKS)
static bool randomTrackWrapper(Menu::Action_t action, Menu::Engine &__) {
    if (action == Menu::Action_t::actionTrigger) {

        //TODO maybe select? also from sd?
        playRandomTrack();
    }

    return true;
}

MenuItemPtr(miSound, "Random Soundtrack", miSoundNext, miSoundPrev, &miExit, NULL, randomTrackWrapper);
#endif

// ===================================================================================================
// Submenu settings

static bool saveSettingsWrapper(Menu::Action_t action, Menu::Engine &__) {
    if (action == Menu::Action_t::actionTrigger) {

        writePrefsToFlash();
        //TODO show message? we might wanna use the legend for this + play a sound!

        playTones(TUNE_STARTUP);
    }

    return true;
}

static bool bypassLearning(Menu::Action_t action, Menu::Engine &__) {
    if (action == Menu::Action_t::actionTrigger) {

        //TODO show message? we might wanna use the legend for this + play a sound!

        prefs.learningComplete = LEARNING_BYPASSED;

        playTones(TUNE_STARTUP);
    }

    return true;
}

MenuItemPtr(miBypass, "Bypass Learning", miBypassNext, miBypassPrev, &miSettings, NULL, bypassLearning);
MenuItemPtr(miSave, "Save settings", miSaveNext, miSavePrev, &miSettings, NULL, saveSettingsWrapper);
#ifdef SD_SETTINGS_FEATURE
MenuItemPtr(miExport, "Export Settigns", miExportNext, miExportPrev, &miSettings, NULL, exportPrefs);
MenuItemPtr(miImport, "Import Settings", miImportNext, miImportPrev, &miSettings, NULL, importPrefs);
#endif
MenuItemPtr(miReset, "Factory Reset", miResetNext, miResetPrev, &miSettings, NULL, factoryReset);

// ===================================================================================================
// Submenu servo settings

#ifdef PREF_SETTINGS
#ifdef SERVO_SUPPORT

typedef struct {
    uint8_t is8bit;
    void *currentValue;
    const void *lowerBound;
    const void *upperBound;
    void (*handleChange)(uint16_t newValue, Menu::Engine &engine);
} __attribute__((packed)) EditNumParam;

extern const EditNumParam EDIT_SERVO_SETTINGS[] PROGMEM;

static void defaultServoHandleChange(uint16_t newValue, Menu::Engine &engine) {
    const uint8_t *params = (uint8_t *)(EDIT_SERVO_SETTINGS + (engine.currentItemInfo.position - 1));
    void *currentValuePtr = pgm_read_ptr(params + offsetof(EditNumParam, currentValue));
    // are only 8 bits used?
    if (pgm_read_byte(params + offsetof(EditNumParam, is8bit))) {
        *((uint8_t *)currentValuePtr) = newValue;
    } else {
        *((uint16_t *)currentValuePtr) = newValue;
    }
}

static void changeMinPulseServoSettings(uint16_t newValue, Menu::Engine &engine) {
    defaultServoHandleChange(newValue, engine);
    initServo();
    setServoPosition(prefs.servoClosedDegrees, 1000);
}

static void changeMaxPulseServoSettings(uint16_t newValue, Menu::Engine &engine) {
    defaultServoHandleChange(newValue, engine);
    initServo();
    setServoPosition(prefs.servoOpenDegrees, 1000);
}

static void changeServoDegreeSettings(uint16_t newValue, Menu::Engine &engine) {
    defaultServoHandleChange(newValue, engine);
    setServoPosition(newValue, 1000);
}

static constexpr const uint8_t servoPinLowerBound = 0;
static constexpr const uint8_t servoPinUpperBound = 19;

static constexpr const uint8_t servoDegreeLowerBound = 0;
static constexpr const uint8_t servoDegreeUpperBound = 180;

static constexpr const uint16_t servoMinPulseLowerBound = 100;
static constexpr const uint16_t servoMinPulseUpperBound = 1000;

static constexpr const uint16_t servoMaxPulseLowerBound = 2000;
static constexpr const uint16_t servoMaxPulseUpperBound = 3000;

static const EditNumParam EDIT_SERVO_SETTINGS[] PROGMEM = {
    {1,
     &prefs.servoPin,
     &servoPinLowerBound,
     &servoPinUpperBound,
     defaultServoHandleChange},
    {1,
     &prefs.servoOpenDegrees,
     &servoDegreeLowerBound,
     &servoDegreeUpperBound,
     changeServoDegreeSettings},
    {1,
     &prefs.servoClosedDegrees,
     &servoDegreeLowerBound,
     &servoDegreeUpperBound,
     changeServoDegreeSettings},
    {0,
     &prefs.servoMinPulseWidth,
     &servoMinPulseLowerBound,
     &servoMinPulseUpperBound,
     changeMinPulseServoSettings},
    {0,
     &prefs.servoMaxPulseWidth,
     &servoMaxPulseLowerBound,
     &servoMaxPulseUpperBound,
     changeMaxPulseServoSettings},
};

static bool handleServoSettingsCallbacks(Menu::Action_t action, Menu::Engine &engine) {
    const uint8_t *params = (const uint8_t *)(EDIT_SERVO_SETTINGS + (engine.currentItemInfo.position - 1));
    void *currentValuePtr = pgm_read_ptr(params + offsetof(EditNumParam, currentValue));
    bool is8bit = pgm_read_byte(params + offsetof(EditNumParam, is8bit));

    if (action == Menu::Action_t::actionDisplay) {
        showValue(engine, is8bit ? *((uint8_t *)currentValuePtr) : *((uint16_t *)currentValuePtr));
    } else if (action == Menu::Action_t::actionTrigger) {

        const void *lowerBoundPtr = pgm_read_ptr(params + offsetof(EditNumParam, lowerBound));
        const void *upperBoundPtr = pgm_read_ptr(params + offsetof(EditNumParam, upperBound));

        uint16_t currentValue = 0;
        uint16_t lowerBound = 0;
        uint16_t upperBound = 0;
        // are only 8 bits used?
        if (is8bit) {
            currentValue = *((uint8_t *)currentValuePtr);
            lowerBound = *((const uint8_t *)lowerBoundPtr);
            upperBound = *((const uint8_t *)upperBoundPtr);
        } else {
            currentValue = *((uint16_t *)currentValuePtr);
            lowerBound = *((const uint16_t *)lowerBoundPtr);
            upperBound = *((const uint16_t *)upperBoundPtr);
        }

        if (changeNum(engine, currentValue, lowerBound, upperBound, (void (*)(uint16_t, Menu::Engine &))pgm_read_ptr(params + offsetof(EditNumParam, handleChange)))) {
            // Lets save the settings!
            writePrefsToFlash();
        }
        initServo();
        // Reset servo to closed state!
        setServoPosition(prefs.servoClosedDegrees, 1000);
    }

    return true;
}

MenuItemPtr(miServoPin, "Edit Servo Pin", miServoPinNext, miServoPinPrev, &miServo, NULL, handleServoSettingsCallbacks);
MenuItemPtr(miServoOpen, "Edit Open Degree", miServoOpenNext, miServoOpenPrev, &miServo, NULL, handleServoSettingsCallbacks);
MenuItemPtr(miServoClose, "Edit Close Degree", miServoCloseNext, miServoClosePrev, &miServo, NULL, handleServoSettingsCallbacks);
MenuItemPtr(miServoMin, "Edit Min Pulse", miServoMinNext, miServoMinPrev, &miServo, NULL, handleServoSettingsCallbacks);
MenuItemPtr(miServoMax, "Edit Max Pulse", miServoMaxNext, miServoMaxPrev, &miServo, NULL, handleServoSettingsCallbacks);

#endif
#endif

// ===================================================================================================
// Submenu output settings

#ifdef PREF_SETTINGS

#ifdef OUTPUT_SETTINGS

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)

extern const Menu::Item_t miSelectOutputType;
extern const Menu::Item_t miChangePin;

static uint8_t outputIndex = 0;

static bool handleModifyOutputTypeResult(Menu::Action_t action, Menu::Engine &engine) {

    // We assume that the item is set to the parent of the selected element
    uint8_t *target = prefs.outputType + outputIndex /*+ (engine.currentItemInfo.item == &miSelectOutputType ? 0 : NUMBER_OF_OUTPUTS)*/;

    if (action == Menu::Action_t::actionDisplay) {
        const uint8_t currentValue = *target;

        const char *valueLabel = NULL;

        // Check if the value is a valid number!
        if (currentValue < NO_OF_TYPES) {
            // Get the corresponding label
            const Menu::Item_t *labelItem = miOutputTypeStart;
            for (uint8_t i = 0; i < currentValue; ++i) {
                labelItem = Menu::Engine::getNext(labelItem);
            }

            valueLabel = Menu::Engine::getLabel(labelItem);
        } else {
            valueLabel = PSTR("INVALID");
        }

        // Show the label as value
        showStrLabel(engine, valueLabel);
    } else if (action == Menu::Action_t::actionCustom) {
        // We have got a selection!
        // We assume that there was no render in the meantime so the position stored in currentItemInfo is still for the selected Item!
        uint8_t value = engine.currentItemInfo.position - 1;
        // Lets save that number
        *target = value;
    }

    return true;
}

static void defaultHandleOutputPinChange(uint16_t newValue, Menu::Engine &engine) {
    uint8_t *target = prefs.outputPin + outputIndex;
    *target = newValue;
}

static bool handleChangeOutputPin(Menu::Action_t action, Menu::Engine &engine) {

    uint8_t *target = prefs.outputPin + outputIndex;

    if (action == Menu::Action_t::actionDisplay) {
        showValue(engine, *target);
    } else if (action == Menu::Action_t::actionTrigger) {
        changeNum(engine, *target, 0, 19, defaultHandleOutputPinChange);
    }

    return true;
}

// Top subsubmenu element
MenuItemPtr(miChangeOutputExit, "Modify/Add Output", NULL, NULL, &miChangeOutputExit, &miSelectOutputType, NULL);

// Subsubsubmenu start element
MenuItemPtr(miSelectOutputType, "Change Type", &miChangePin, NULL, &miChangeOutputExit, miOutputTypeStart, handleModifyOutputTypeResult);
MenuItemPtr(miChangePin, "Change Pin", NULL, &miSelectOutputType, &miChangeOutputExit, NULL, handleChangeOutputPin);

static bool handleModifyOutput(Menu::Action_t action, Menu::Engine &engine) {
    if (action == Menu::Action_t::actionTrigger) {
        outputIndex = numOutputsConfigured() - engine.currentItemInfo.position;

        // For subsubmenus is a new engine needed!
        Menu::Engine modifyOutputEngine(Menu::Engine::getChild(&miChangeOutputExit));
        // Exits on goto parent
        showMenu(modifyOutputEngine, &miChangeOutputExit);

        // Apply changes!
        initOutputs();

        engine.forceNewRender = true;
    }

    return true;
}

MenuItemPtr(miModifyOutput, "Modify Output", miModifyOutputNext, miModifyOutputPrev, &miOutput, NULL, handleModifyOutput);
MenuItemPtr(miAddOutput, "Add Output", miAddOutputNext, miAddOutputPrev, &miOutput, NULL, handleModifyOutput);

MenuItemPtr(miOutputTypeUnused, "Unused", miOutputTypeUnusedNext, miOutputTypeUnusedPrev, &miSelectOutputType, NULL, handleSelection);
MenuItemPtr(miOutputTypeBottom, "Bottom Element", miOutputTypeBottomNext, miOutputTypeBottomPrev, &miSelectOutputType, NULL, handleSelection);
MenuItemPtr(miOutputTypeTop, "Top Element", miOutputTypeTopNext, miOutputTypeTopPrev, &miSelectOutputType, NULL, handleSelection);
MenuItemPtr(miOutputTypeBoost, "Boost Element", miOutputTypeBoostNext, miOutputTypeBoostPrev, &miSelectOutputType, NULL, handleSelection);
MenuItemPtr(miOutputTypeConvection, "Convection Fan", miOutputTypeConvectionNext, miOutputTypeConvectionPrev, &miSelectOutputType, NULL, handleSelection);
MenuItemPtr(miOutputTypeCooling, "Cooling Fan", miOutputTypeCoolingNext, miOutputTypeCoolingPrev, &miSelectOutputType, NULL, handleSelection);

#endif

#endif

#endif

// ===================================================================================================
