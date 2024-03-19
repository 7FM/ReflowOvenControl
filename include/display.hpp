#pragma once

#include "config.h"

#include <Arduino.h>
#include <Menu.h>

#include "thermo.hpp"

// Formatting settings for the display
#define GRAPH_TEMP_OFFSET 50

#define menuItemHeight 12
#define menuItemsVisible 8
#define MENU_TEXT_XPOS 5
#define MENU_BAR_XPOS 3
#define MENU_TEXT_Y_OFFSET 2
#define VALUE_LABEL_WIDTH 40

void initDisplay();

void prepareShowMsg();
void afterShowMsg();
void showErrorMsg(const char *msg);
void showErrorMsgP(const char *msg);

void showMenu(Menu::Engine &engine, const Menu::Item_t *exitItem);

void showStrLabel(const Menu::Engine &engine, const char *str);
void showValue(const Menu::Engine &engine, uint16_t value);
bool changeNum(Menu::Engine &engine, uint16_t currentValue, uint16_t lowerBound, uint16_t upperBound, void (*handleChange)(uint16_t newValue, Menu::Engine &engine));

Menu::RenderCallback_t renderMenuItem;

void restartDrawProgressDisplay();
void drawInitialProgressDisplay(uint16_t peakTemp);
void updateProgressDisplay(Temperature temperature, Temperature expectedTemp);
void updateProgressDisplaySetPoint(Temperature setpoint);
void updateProgressDisplayState(const char *stateStr, bool isProgmem);

void initStatusBar();
void updateStatusBar(const char *status, bool isProgmem);
void updateTemperature(Temperature temperature);
void drawLegend(const char *legendStr);

bool receivedUserAction();

void waitForUserAction();

bool handleSelection(Menu::Action_t action, Menu::Engine &engine);

extern Print* display;

#define DisplayPrint(x) display->print(x)
#define DisplayPrintLn(x) display->println(x)
#define DisplayProgmemPrint(x) display->print(reinterpret_cast<const __FlashStringHelper *>(x))
#define DisplayProgmemPrintLn(x) display->println(reinterpret_cast<const __FlashStringHelper *>(x))

#if defined(USE_DISPLAY) && defined(ROTARY_ISR_SERVICE)
void encoderService();
#endif