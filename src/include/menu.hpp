#pragma once

#include "config.h"

#include <Menu.h> // Library for managing menus

// ===================================================================================================
// Menu definitions

// ===================================================================================================
// Main menu entries
//TODO some helper makros would be really appreciated!

extern const Menu::Item_t miExit;

#define miReflowPrev NULL
#define miLearnPrev NULL
#define miSettingsPrev NULL
#define miServoPrev NULL
#define miOutputPrev NULL
#define miEjectPrev NULL
#define miSoundPrev NULL

#ifdef REFLOW_FEATURE

extern const Menu::Item_t miReflow;
#ifndef miMainMenu
#define miMainMenu &miReflow
#endif
#undef miLearnPrev
#define miLearnPrev &miReflow
#undef miSettingsPrev
#define miSettingsPrev &miReflow

#endif

#ifdef LEARNING_FEATURE

extern const Menu::Item_t miLearn;
#ifndef miMainMenu
#define miMainMenu &miLearn
#endif
#undef miSettingsPrev
#define miSettingsPrev &miLearn

#ifndef miReflowNext
#define miReflowNext &miLearn
#endif

#endif

extern const Menu::Item_t miSettings;
#ifndef miMainMenu
#define miMainMenu &miSettings
#endif
#undef miServoPrev
#define miServoPrev &miSettings
#undef miOutputPrev
#define miOutputPrev &miSettings
#undef miEjectPrev
#define miEjectPrev &miSettings
#undef miSoundPrev
#define miSoundPrev &miSettings
#ifndef miReflowNext
#define miReflowNext &miSettings
#endif
#ifndef miLearnNext
#define miLearnNext &miSettings
#endif

#ifdef PREF_SETTINGS

#ifdef SERVO_SUPPORT

extern const Menu::Item_t miServo;
#ifndef miMainMenu
#define miMainMenu &miServo
#endif
#undef miOutputPrev
#define miOutputPrev &miServo
#undef miEjectPrev
#define miEjectPrev &miServo
#undef miSoundPrev
#define miSoundPrev &miServo

#ifndef miReflowNext
#define miReflowNext &miServo
#endif
#ifndef miLearnNext
#define miLearnNext &miServo
#endif
#ifndef miSettingsNext
#define miSettingsNext &miServo
#endif

#endif

#ifdef OUTPUT_SETTINGS

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)

extern const Menu::Item_t miOutput;
#ifndef miMainMenu
#define miMainMenu &miOutput
#endif
#undef miEjectPrev
#define miEjectPrev &miOutput
#undef miSoundPrev
#define miSoundPrev &miOutput

#ifndef miReflowNext
#define miReflowNext &miOutput
#endif
#ifndef miLearnNext
#define miLearnNext &miOutput
#endif
#ifndef miSettingsNext
#define miSettingsNext &miOutput
#endif
#ifndef miServoNext
#define miServoNext &miOutput
#endif

#endif

#endif

#endif

#ifdef REFLOW_FEATURE

extern const Menu::Item_t miEject;
#ifndef miMainMenu
#define miMainMenu &miEject
#endif
#undef miSoundPrev
#define miSoundPrev &miEject

#ifndef miReflowNext
#define miReflowNext &miEject
#endif
#ifndef miLearnNext
#define miLearnNext &miEject
#endif
#ifndef miSettingsNext
#define miSettingsNext &miEject
#endif
#ifndef miServoNext
#define miServoNext &miEject
#endif
#ifndef miOutputNext
#define miOutputNext &miEject
#endif

#endif

#if defined(USE_BUZZER) && defined(BUZZER_SOUNDTRACKS)

extern const Menu::Item_t miSound;
#ifndef miMainMenu
#define miMainMenu &miSound
#endif
#ifndef miReflowNext
#define miReflowNext &miSound
#endif
#ifndef miLearnNext
#define miLearnNext &miSound
#endif
#ifndef miSettingsNext
#define miSettingsNext &miSound
#endif
#ifndef miServoNext
#define miServoNext &miSound
#endif
#ifndef miOutputNext
#define miOutputNext &miSound
#endif
#ifndef miEjectNext
#define miEjectNext &miSound
#endif

#endif

#ifndef miMainMenu
#define miMainMenu NULL
#endif
#ifndef miReflowNext
#define miReflowNext NULL
#endif
#ifndef miLearnNext
#define miLearnNext NULL
#endif
#ifndef miReflowNext
#define miReflowNext NULL
#endif
#ifndef miSettingsNext
#define miSettingsNext NULL
#endif
#ifndef miServoNext
#define miServoNext NULL
#endif
#ifndef miOutputNext
#define miOutputNext NULL
#endif
#ifndef miEjectNext
#define miEjectNext NULL
#endif
#ifndef miSoundNext
#define miSoundNext NULL
#endif

// ===================================================================================================
// Submenu settings

#define miBypassPrev NULL
#define miSavePrev NULL
#define miExportPrev NULL
#define miImportPrev NULL
#define miResetPrev NULL

extern const Menu::Item_t miBypass;
#undef miSavePrev
#define miSavePrev &miBypass
#undef miExportPrev
#define miExportPrev &miBypass
#undef miImportPrev
#define miImportPrev &miBypass
#undef miResetPrev
#define miResetPrev &miBypass
#ifndef miSettingsSubMenu
#define miSettingsSubMenu &miBypass
#endif

extern const Menu::Item_t miSave;
#undef miExportPrev
#define miExportPrev &miSave
#undef miImportPrev
#define miImportPrev &miSave
#undef miResetPrev
#define miResetPrev &miSave
#ifndef miSettingsSubMenu
#define miSettingsSubMenu &miSave
#endif
#ifndef miBypassNext
#define miBypassNext &miSave
#endif

#ifdef SD_SETTINGS_FEATURE

extern const Menu::Item_t miExport;
#undef miImportPrev
#define miImportPrev &miExport
#undef miResetPrev
#define miResetPrev &miExport
#ifndef miSettingsSubMenu
#define miSettingsSubMenu &miExport
#endif
#ifndef miBypassNext
#define miBypassNext &miExport
#endif
#ifndef miSaveNext
#define miSaveNext &miExport
#endif
extern const Menu::Item_t miImport;
#undef miResetPrev
#define miResetPrev &miImport
#ifndef miSettingsSubMenu
#define miSettingsSubMenu &miImport
#endif
#ifndef miBypassNext
#define miBypassNext &miImport
#endif
#ifndef miSaveNext
#define miSaveNext &miImport
#endif
#ifndef miExportNext
#define miExportNext &miImport
#endif

#endif

extern const Menu::Item_t miReset;
#ifndef miSettingsSubMenu
#define miSettingsSubMenu &miReset
#endif
#ifndef miBypassNext
#define miBypassNext &miReset
#endif
#ifndef miSaveNext
#define miSaveNext &miReset
#endif
#ifndef miExportNext
#define miExportNext &miReset
#endif
#ifndef miImportNext
#define miImportNext &miReset
#endif

#ifndef miSettingsSubMenu
#define miSettingsSubMenu NULL
#endif
#ifndef miBypassNext
#define miBypassNext NULL
#endif
#ifndef miSaveNext
#define miSaveNext NULL
#endif
#ifndef miExportNext
#define miExportNext NULL
#endif
#ifndef miImportNext
#define miImportNext NULL
#endif
#ifndef miResetNext
#define miResetNext NULL
#endif

// ===================================================================================================
// Submenu servo settings

#ifdef PREF_SETTINGS
#ifdef SERVO_SUPPORT

#define miServoPinPrev NULL
#define miServoOpenPrev NULL
#define miServoClosePrev NULL
#define miServoMinPrev NULL
#define miServoMaxPrev NULL

extern const Menu::Item_t miServoPin;
#undef miServoOpenPrev
#define miServoOpenPrev &miServoPin
#undef miServoClosePrev
#define miServoClosePrev &miServoPin
#undef miServoMinPrev
#define miServoMinPrev &miServoPin
#undef miServoMaxPrev
#define miServoMaxPrev &miServoPin
#ifndef miServoSubMenu
#define miServoSubMenu &miServoPin
#endif

extern const Menu::Item_t miServoOpen;
#undef miServoClosePrev
#define miServoClosePrev &miServoOpen
#undef miServoMinPrev
#define miServoMinPrev &miServoOpen
#undef miServoMaxPrev
#define miServoMaxPrev &miServoOpen
#ifndef miServoSubMenu
#define miServoSubMenu &miServoOpen
#endif
#ifndef miServoPinNext
#define miServoPinNext &miServoOpen
#endif

extern const Menu::Item_t miServoClose;
#undef miServoMinPrev
#define miServoMinPrev &miServoClose
#undef miServoMaxPrev
#define miServoMaxPrev &miServoClose
#ifndef miServoSubMenu
#define miServoSubMenu &miServoClose
#endif
#ifndef miServoPinNext
#define miServoPinNext &miServoClose
#endif
#ifndef miServoOpenNext
#define miServoOpenNext &miServoClose
#endif

extern const Menu::Item_t miServoMin;
#undef miServoMaxPrev
#define miServoMaxPrev &miServoMin
#ifndef miServoSubMenu
#define miServoSubMenu &miServoMin
#endif
#ifndef miServoPinNext
#define miServoPinNext &miServoMin
#endif
#ifndef miServoOpenNext
#define miServoOpenNext &miServoMin
#endif
#ifndef miServoCloseNext
#define miServoCloseNext &miServoMin
#endif

extern const Menu::Item_t miServoMax;
#ifndef miServoSubMenu
#define miServoSubMenu &miServoMax
#endif
#ifndef miServoPinNext
#define miServoPinNext &miServoMax
#endif
#ifndef miServoOpenNext
#define miServoOpenNext &miServoMax
#endif
#ifndef miServoCloseNext
#define miServoCloseNext &miServoMax
#endif
#ifndef miServoMinNext
#define miServoMinNext &miServoMax
#endif

#ifndef miServoSubMenu
#define miServoSubMenu NULL
#endif
#ifndef miServoPinNext
#define miServoPinNext NULL
#endif
#ifndef miServoOpenNext
#define miServoOpenNext NULL
#endif
#ifndef miServoCloseNext
#define miServoCloseNext NULL
#endif
#ifndef miServoMinNext
#define miServoMinNext NULL
#endif
#ifndef miServoMaxNext
#define miServoMaxNext NULL
#endif

#endif
#endif

// ===================================================================================================
// Submenu output settings

#ifdef PREF_SETTINGS
#ifdef OUTPUT_SETTINGS

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE)

#define miAddOutputPrev NULL
#define miModifyOutputPrev NULL

extern const Menu::Item_t miModifyOutput;
#undef miAddOutputPrev
#define miAddOutputPrev &miModifyOutput
#ifndef miOutputSubMenu
#define miOutputSubMenu &miModifyOutput
#endif

extern const Menu::Item_t miAddOutput;
#ifndef miOutputSubMenu
#define miOutputSubMenu &miAddOutput
#endif
#ifndef miModifyOutputNext
#define miModifyOutputNext &miAddOutput
#endif

#ifndef miOutputSubMenu
#define miOutputSubMenu NULL
#endif
#ifndef miAddOutputNext
#define miAddOutputNext NULL
#endif
#ifndef miModifyOutputNext
#define miModifyOutputNext NULL
#endif

// Output Types

#define miOutputTypeUnusedPrev NULL
#define miOutputTypeBottomPrev NULL
#define miOutputTypeTopPrev NULL
#define miOutputTypeBoostPrev NULL
#define miOutputTypeConvectionPrev NULL
#define miOutputTypeCoolingPrev NULL

extern const Menu::Item_t miOutputTypeUnused;
#undef miOutputTypeBottomPrev
#define miOutputTypeBottomPrev &miOutputTypeUnused
#undef miOutputTypeTopPrev
#define miOutputTypeTopPrev &miOutputTypeUnused
#undef miOutputTypeBoostPrev
#define miOutputTypeBoostPrev &miOutputTypeUnused
#undef miOutputTypeConvectionPrev
#define miOutputTypeConvectionPrev &miOutputTypeUnused
#undef miOutputTypeCoolingPrev
#define miOutputTypeCoolingPrev &miOutputTypeUnused
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeUnused
#endif

extern const Menu::Item_t miOutputTypeBottom;
#undef miOutputTypeTopPrev
#define miOutputTypeTopPrev &miOutputTypeBottom
#undef miOutputTypeBoostPrev
#define miOutputTypeBoostPrev &miOutputTypeBottom
#undef miOutputTypeConvectionPrev
#define miOutputTypeConvectionPrev &miOutputTypeBottom
#undef miOutputTypeCoolingPrev
#define miOutputTypeCoolingPrev &miOutputTypeBottom
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeBottom
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext &miOutputTypeBottom
#endif

extern const Menu::Item_t miOutputTypeTop;
#undef miOutputTypeBoostPrev
#define miOutputTypeBoostPrev &miOutputTypeTop
#undef miOutputTypeConvectionPrev
#define miOutputTypeConvectionPrev &miOutputTypeTop
#undef miOutputTypeCoolingPrev
#define miOutputTypeCoolingPrev &miOutputTypeTop
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeTop
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext &miOutputTypeTop
#endif
#ifndef miOutputTypeBottomNext
#define miOutputTypeBottomNext &miOutputTypeTop
#endif

extern const Menu::Item_t miOutputTypeBoost;
#undef miOutputTypeConvectionPrev
#define miOutputTypeConvectionPrev &miOutputTypeBoost
#undef miOutputTypeCoolingPrev
#define miOutputTypeCoolingPrev &miOutputTypeBoost
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeBoost
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext &miOutputTypeBoost
#endif
#ifndef miOutputTypeBottomNext
#define miOutputTypeBottomNext &miOutputTypeBoost
#endif
#ifndef miOutputTypeTopNext
#define miOutputTypeTopNext &miOutputTypeBoost
#endif

extern const Menu::Item_t miOutputTypeConvection;
#undef miOutputTypeCoolingPrev
#define miOutputTypeCoolingPrev &miOutputTypeConvection
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeConvection
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext &miOutputTypeConvection
#endif
#ifndef miOutputTypeBottomNext
#define miOutputTypeBottomNext &miOutputTypeConvection
#endif
#ifndef miOutputTypeTopNext
#define miOutputTypeTopNext &miOutputTypeConvection
#endif
#ifndef miOutputTypeBoostNext
#define miOutputTypeBoostNext &miOutputTypeConvection
#endif

extern const Menu::Item_t miOutputTypeCooling;
#ifndef miOutputTypeStart
#define miOutputTypeStart &miOutputTypeCooling
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext &miOutputTypeCooling
#endif
#ifndef miOutputTypeBottomNext
#define miOutputTypeBottomNext &miOutputTypeCooling
#endif
#ifndef miOutputTypeTopNext
#define miOutputTypeTopNext &miOutputTypeCooling
#endif
#ifndef miOutputTypeBoostNext
#define miOutputTypeBoostNext &miOutputTypeCooling
#endif
#ifndef miOutputTypeConvectionNext
#define miOutputTypeConvectionNext &miOutputTypeCooling
#endif

#ifndef miOutputTypeStart
#define miOutputTypeStart NULL
#endif
#ifndef miOutputTypeUnusedNext
#define miOutputTypeUnusedNext NULL
#endif
#ifndef miOutputTypeBottomNext
#define miOutputTypeBottomNext NULL
#endif
#ifndef miOutputTypeTopNext
#define miOutputTypeTopNext NULL
#endif
#ifndef miOutputTypeBoostNext
#define miOutputTypeBoostNext NULL
#endif
#ifndef miOutputTypeConvectionNext
#define miOutputTypeConvectionNext NULL
#endif
#ifndef miOutputTypeCoolingNext
#define miOutputTypeCoolingNext NULL
#endif

#endif
#endif
#endif

// ===================================================================================================
