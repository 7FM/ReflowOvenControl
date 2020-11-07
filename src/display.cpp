#include <Arduino.h>

#include "config.h"

#include "display.hpp"
#include "pin.hpp"
#include "progmem_strings.hpp"

#ifdef USE_BUZZER
#include "buzzer.hpp"
#endif

#ifdef USE_DISPLAY

#ifndef ADAFRUIT_TFT_LIB
#include <PDQ_GFX/PDQ_GFX.h>       // PDQ: Core graphics library
#include <PDQ_ST7735/PDQ_ST7735.h> // PDQ: Hardware-specific driver library
#else
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Wire.h>
#endif

#include <ClickEncoder.h>
#include <FastPin.h>

#ifndef ADAFRUIT_TFT_LIB
// other PDQ library options
// template parameter is twice true to save & restore AVR SPI control register (to "play nice" when other SPI use, i.e. sd card is else broken)
static PDQ_ST7735<ST7735_INITR_GREENTAB, TFT_CS, TFT_DC, TFT_RST, true, true> tft;
#else
static Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);
#endif

Print *display = &tft;

#if defined(DEBUG) && defined(BENCHMARK)
#include "graphicstest.hpp"
#endif

#define ENCODER ClickEncoder<ENCODER_A_PIN, ENCODER_B_PIN, ENCODER_SW_PIN, ENCODER_IS_ACTIVE, ENCODER_STEPS_PER_NOTCH, ENCODER_ACCEL_TOP, ENCODER_ACCEL_INC, ENCODER_ACCEL_DEC>

// ----------------------------------------------------------------------------

const char DEFAULT_LEGEND[] PROGMEM = "[Child] Click [Back] DClick";
const char SAVE_ABORT_LEGEND[] PROGMEM = "[Save] Click [Abort] DClick";

bool receivedUserAction() {
    ButtonState state = ENCODER::getButtonState();
    return state == Clicked || state == DoubleClicked;
}

void waitForUserAction() {
    do {
        _delay_ms(50);
    } while (!receivedUserAction());
}

void renderMenuItem(const Menu::Engine &engine, const Menu::Item_t *selectedItem) {
    if (selectedItem == NULL) {
        // Some drastic change has happend(i.e. goto parent/child) so we have to update the status bar text!
        updateStatusBar(Menu::Engine::getLabel(Menu::Engine::getParent(engine.currentItemInfo.item)), Menu::Engine::progMemEntries);
        tft.fillRect(0, menuItemHeight, tft.width(), tft.height() - (menuItemHeight * 2), ST7735_WHITE);
        drawLegend(DEFAULT_LEGEND);
    } else {
        const Menu::Item_t *mi = engine.currentItemInfo.item;

        bool isCurrent = engine.currentItemInfo.item == selectedItem;
        // Leave the first row empty for possible status messages
        coord_t y = engine.currentItemInfo.position * menuItemHeight;

        // menu cursor bar
        coord_t barWidth = tft.width() - (MENU_BAR_XPOS << 1);
        if (isCurrent) {
            tft.setTextColor(ST7735_WHITE);
            tft.fillRect(MENU_BAR_XPOS, y, barWidth, menuItemHeight, ST7735_BLUE);
        } else {
            tft.setTextColor(ST7735_BLACK);
            tft.fillRect(MENU_BAR_XPOS, y, barWidth, menuItemHeight, ST7735_WHITE);
        }

        tft.setCursor(MENU_TEXT_XPOS, y + MENU_TEXT_Y_OFFSET);

        const char *str = Menu::Engine::getLabel(mi);
        if (str != NULL) {
            if (Menu::Engine::progMemEntries) {
                tft.print(reinterpret_cast<const __FlashStringHelper *>(str));
            } else {
                tft.print(str);
            }
        }

        // mark items that have children
        if (Menu::Engine::getChild(mi) != NULL) {
            tft.print(" \x10"); // 0x10 -> filled right arrow
        }
    }
}

static void prepareShowValue(const Menu::Engine &engine) {
    const uint8_t y = engine.currentItemInfo.position * menuItemHeight + MENU_TEXT_Y_OFFSET;

    const uint8_t xOffset = tft.width() - VALUE_LABEL_WIDTH - MENU_BAR_XPOS - (MENU_TEXT_Y_OFFSET >> 1);

    tft.fillRect(xOffset, y - (MENU_TEXT_Y_OFFSET >> 1), VALUE_LABEL_WIDTH, menuItemHeight - MENU_TEXT_Y_OFFSET, ST7735_RED);

    tft.setCursor(xOffset + MENU_TEXT_XPOS, y);

    tft.setTextColor(ST7735_WHITE);
}

void showStrLabel(const Menu::Engine &engine, const char *str) {
    prepareShowValue(engine);
    tft.print(reinterpret_cast<const __FlashStringHelper *>(str));
}

void showValue(const Menu::Engine &engine, uint16_t value) {
    prepareShowValue(engine);
    tft.print(value);
}

bool changeNum(Menu::Engine &engine, uint16_t currentValue, uint16_t lowerBound, uint16_t upperBound, void (*handleChange)(uint16_t newValue, Menu::Engine &engine)) {
    ENCODER::setAccelerationEnabled(true);

    // Print legend
    drawLegend(SAVE_ABORT_LEGEND);

    // Print value
    // assume that it is already printed? due to actionDisplay???
    //showValue(engine, currentValue);

    const uint16_t initValue = currentValue;

    bool save = true;

    for (;;) {
        const int16_t value = ENCODER::getValue();
        if (value) {

            const uint16_t prevValue = currentValue;

            currentValue += value;

            // Ensure the value is in the given bounds! Also handle under- & overflows
            if (value < 0) {
                if (currentValue < lowerBound || prevValue < currentValue) {
                    currentValue = lowerBound;
                }
            } else {
                if (currentValue > upperBound || prevValue > currentValue) {
                    currentValue = upperBound;
                }
            }

            // Update value if there was a change
            if (prevValue != currentValue) {
                showValue(engine, currentValue);
                handleChange(currentValue, engine);
            }
        } else {

            switch (ENCODER::getButtonState()) {

                case DoubleClicked:
                    // Reset value
                    showValue(engine, initValue);
                    handleChange(initValue, engine);
                    save = false;
                    // Intentionally fall through

                case Clicked:
                    goto endChangeNum;

                default:
                    break;
            }
        }
    }

endChangeNum:
    drawLegend(DEFAULT_LEGEND);
    ENCODER::setAccelerationEnabled(false);

    return save;
}

static constexpr uint8_t calculateExternalInterruptEnableMask() {
    return 0x00 |
           // external interrupt line 0 ist connected to D2
           ((ENCODER_A_PIN == 2 || ENCODER_B_PIN == 2) ? _BV(INT0) : 0x00) |
           // external interrupt line 1 ist connected to D3
           ((ENCODER_A_PIN == 3 || ENCODER_B_PIN == 3) ? _BV(INT1) : 0x00);
}

static constexpr uint8_t calculatePinChangeLine(uint8_t pin) {
    return _CAN_PIN_USE_INTX_INTERRUPT_LINE(pin) ? -1 : _GET_PIN_INTERRUPT_LINE(pin);
}

static constexpr uint8_t calculatePinChangeLineMask(uint8_t pin) {
    return pin <= 7 ? pin : pin <= 15 ? pin - 8 : pin - 16;
}

static constexpr uint8_t calculatePinChangeInterruptEnableMask(uint8_t pinChangeLine) {
    return 0x00 |
           // is pin A of the encoder on this interrupt line? if set it active!
           ((calculatePinChangeLine(ENCODER_A_PIN) == pinChangeLine) ? (1 << calculatePinChangeLineMask(ENCODER_A_PIN)) : 0x00) |
           // is pin B of the encoder on this interrupt line? if set it active!
           ((calculatePinChangeLine(ENCODER_B_PIN) == pinChangeLine) ? (1 << calculatePinChangeLineMask(ENCODER_B_PIN)) : 0x00);
}

static constexpr uint8_t calculatePinChangeEnable() {
    return 0x00 |
           // is pin A or B of the encoder on this interrupt line? if so we need to enable the interrupts for this line!
           ((calculatePinChangeLine(ENCODER_A_PIN) == 0 || calculatePinChangeLine(ENCODER_B_PIN) == 0) ? _BV(PCIE0) : 0x00) |
           ((calculatePinChangeLine(ENCODER_A_PIN) == 1 || calculatePinChangeLine(ENCODER_B_PIN) == 1) ? _BV(PCIE1) : 0x00) |
           ((calculatePinChangeLine(ENCODER_A_PIN) == 2 || calculatePinChangeLine(ENCODER_B_PIN) == 2) ? _BV(PCIE2) : 0x00);
}

static inline void setupEncoderRoutine() {
#if !defined(ROTARY_ISR_SERVICE) || DEBOUNCE_TIME > 0
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function below

    // Enusre Timer0 is setup as expected by millis even if millis is not used!
    TCCR0A = _BV(WGM01) | _BV(WGM00); // Fast PWM mode
    TCCR0B = _BV(CS01) | _BV(CS00);   // Prescale 64 for ~1kHz Interrupt rate

    OCR0A = 0x7F;

#ifndef ROTARY_ISR_SERVICE
    // set interrupt enable flag
    TIMSK0 |= _BV(OCIE0A);
#endif
#endif

#ifdef ROTARY_ISR_SERVICE
    // Configure external interrupts if needed
    if (calculateExternalInterruptEnableMask()) {
        //clear pending interrupt requests
        EIFR = _BV(INTF0) | _BV(INTF1);

        // We want an interrupt on any change!
        // ISCx1    ISCx0   Description
        // 0        0       The low level of INTx generates an interrupt request.
        // 0        1       Any logical change on INTx generates an interrupt request.
        // 1        0       The falling edge of INTx generates an interrupt request.
        // 1        1       The rising edge of INTx generates an interrupt request.
        EICRA = _BV(ISC00) /*| _BV(ISC01)*/ | _BV(ISC10) /*| _BV(ISC11)*/;

        // Enable needed external interrupts
        EIMSK |= calculateExternalInterruptEnableMask();
    }

    // Configure toggle interrupts if needed:
    if (calculatePinChangeEnable()) {
        //clear pending interrupt requests
        PCIFR = _BV(PCIF0) | _BV(PCIF1) | _BV(PCIF2);

        PCMSK0 = calculatePinChangeInterruptEnableMask(0);
        PCMSK1 = calculatePinChangeInterruptEnableMask(1);
        PCMSK2 = calculatePinChangeInterruptEnableMask(2);

        // PCIE0: Any change on any enabled PCINT7..0 PCINT7..0 pin will cause an interrupt. Pins are enabled individually by the PCMSK0 register.
        // PCIE1: Any change on any enabled PCINT14..8 pin will cause an interrupt. Pins are enabled individually by the PCMSK1 register.
        // PCIE2: Any change on any enabled PCINT23..16 pin will cause an interrupt. Pins are enabled individually by the PCMSK2 register.

        // Enable pin change interrupts
        PCICR |= calculatePinChangeEnable();
    }
#endif
}

#ifdef ROTARY_ISR_SERVICE
#if DEBOUNCE_TIME > 0
static volatile uint8_t debounceTime = 0;
#endif
#endif

#if !defined(ROTARY_ISR_SERVICE) || DEBOUNCE_TIME > 0
ISR(TIMER0_COMPA_vect) {
#ifdef ROTARY_ISR_SERVICE
#if DEBOUNCE_TIME > 0
    // Lets use this callback for debouncing

    // Debounce time over?
    if (--debounceTime == 0) {
        // disable this interrupt
        TIMSK0 &= ~_BV(OCIE0A);

        // enable external interrupts again!
        if (calculateExternalInterruptEnableMask()) {
            //clear pending interrupt requests
            EIFR = _BV(INTF0) | _BV(INTF1);

            EIMSK |= calculateExternalInterruptEnableMask();
        }
        if (calculatePinChangeEnable()) {
            //clear pending interrupt requests
            PCIFR = _BV(PCIF0) | _BV(PCIF1) | _BV(PCIF2);

            PCICR |= calculatePinChangeEnable();
        }
    }
#endif
#else
    ENCODER::service();
#endif
}
#endif

#ifdef ROTARY_ISR_SERVICE

void encoderService() {
    ENCODER::service();
}

static inline __attribute__((always_inline)) void debounce() {
#if DEBOUNCE_TIME > 0
    debounceTime = DEBOUNCE_TIME;

    // disable external interrupts again!
    if (calculateExternalInterruptEnableMask()) {
        EIMSK &= ~calculateExternalInterruptEnableMask();
    }
    if (calculatePinChangeEnable()) {
        PCICR &= ~calculatePinChangeEnable();
    }

    // enable interrupt for handling debounce time
    TIMSK0 |= _BV(OCIE0A);
#endif
}

#ifdef SPLIT_ROTARY_ISR_SERVICE
ISR(ENCODER_A_PIN_ISR_VECTOR) {
    if (ENCODER::servicePinA()) {
        // debounce is only needed on a detected step
        debounce();
    }
}

ISR(ENCODER_B_PIN_ISR_VECTOR) {
    if (ENCODER::servicePinB()) {
        // debounce is only needed on a detected step
        debounce();
    }
}
#else
// Use aliasing so we have a handler for all possible related callbacks
ISR(ENCODER_A_PIN_ISR_VECTOR) {
    // We have an interrupt caused by a changing pin of the encoder => lets read the new value!
    if (ENCODER::rotaryService()) {
        // debounce is only needed on a detected step
        debounce();
    }
}

#ifdef ENCODER_PINS_USE_DIFFERENT_LINES
ISR(ENCODER_B_PIN_ISR_VECTOR, ISR_ALIASOF(ENCODER_A_PIN_ISR_VECTOR));
#endif

#endif

#endif

void initDisplay() {
#if ENCODER_VCC_PIN != -1
    initPinHIGHConstExpr(ENCODER_VCC_PIN);
#endif

#if ENCODER_GND_PIN != -1
    initPinLOWConstExpr(ENCODER_GND_PIN);
#endif

    ENCODER::init();
    ENCODER::setAccelerationEnabled(false);

    setupEncoderRoutine();

#ifndef ADAFRUIT_TFT_LIB
    tft.begin();
#else
    tft.initR();
#endif

#if defined(DEBUG) && defined(BENCHMARK)
    benchmark();
#endif

    tft.setTextWrap(false);
    tft.setRotation(3);
    tft.setTextSize(1);

    // Initially it is needed to fill the whole status bar
    initStatusBar();
}

void showMenu(Menu::Engine &engine, const Menu::Item_t *exitItem) {
    // Set the status bar to the current exit label
    updateStatusBar(Menu::Engine::getLabel(exitItem), Menu::Engine::progMemEntries);

    engine.render(renderMenuItem, menuItemsVisible);

    for (;;) {

        const int16_t value = ENCODER::getValue();
        const Menu::Item_t *target = NULL;
        if (value) {
            if (value > 0) {
                target = Menu::Engine::getNext(engine.currentItemInfo.item);
            } else {
                target = Menu::Engine::getPrev(engine.currentItemInfo.item);
            }
        } else {

            switch (ENCODER::getButtonState()) {
                case Clicked: {
                    engine.invoke();
                    break;
                }

                case DoubleClicked: {
                    target = Menu::Engine::getParent(engine.currentItemInfo.item);

                    // The parent may not be NULL so we can always execute this!
                    goto navigate;

                    break;
                }

                default:
                    break;
            }
        }

        if (target != NULL) {
        navigate:
            engine.navigate(target);
            goto needRender;
        }

        if (engine.forceNewRender) {
        needRender:
            if (engine.currentItemInfo.item != exitItem) {
                engine.render(renderMenuItem, menuItemsVisible);
            } else {
                // Set back to the previous element
                engine.currentItemInfo.item = engine.previousItem;
                // Exit as requested
                return;
            }
        }
    }
}

void drawLegend(const char *legendStr) {
    // Calculate legend position
    const uint8_t y = tft.height() - menuItemHeight;
    tft.fillRect(0, y, tft.width(), menuItemHeight, ST7735_GREEN);
    tft.setCursor(MENU_BAR_XPOS, y + MENU_TEXT_Y_OFFSET);
    tft.setTextColor(ST7735_BLACK);
    tft.print(reinterpret_cast<const __FlashStringHelper *>(legendStr));
}

void initStatusBar() {
    tft.fillRect(0, 0, tft.width(), menuItemHeight, ST7735_BLUE);
}

static void updateLeftAlignedText(const char *text, bool isProgmem, coord_t textXoffset, coord_t y, coord_t xBorder, color_t textColor, color_t bgColor) {
    // Ensure we clear the whole writable area
    tft.fillRect(0, y, xBorder, menuItemHeight, bgColor);

    y += MENU_TEXT_Y_OFFSET;

    // Lets write the text
    for (; textXoffset < xBorder; textXoffset += X_PIXEL_PER_CHAR, ++text) {
        char c;
        if (isProgmem) {
            c = pgm_read_byte(text);
        } else {
            c = *text;
        }
        if (c) {
            tft.drawChar(textXoffset, y, c, textColor, textColor);
        } else {
            break;
        }
    }
}

void updateStatusBar(const char *status, bool isProgmem) {
    updateLeftAlignedText(status, isProgmem, MENU_TEXT_Y_OFFSET, 0,
                          tft.width() - ((X_PIXEL_PER_CHAR * 5) + MENU_BAR_XPOS), ST7735_WHITE, ST7735_BLUE);
}

void updateProgressDisplayState(const char *stateStr, bool isProgmem) {
    // Set the text box for the current state/message
    // X_PIXEL_PER_CHAR pixel per char * 8 chars = 48 pixel needed for the set point
    updateLeftAlignedText(stateStr, isProgmem, MENU_BAR_XPOS, menuItemHeight,
                          tft.width() - ((X_PIXEL_PER_CHAR * 8) + MENU_BAR_XPOS * 2), ST7735_BLACK, ST7735_GREEN);
}

static void alignRightPrefix(uint16_t v) {
    if (v < 100)
        tft.print(' ');
    if (v < 10)
        tft.print(' ');
    tft.print(v);
}

static const char DEGREE_CELSIUS_TFT_STR[] PROGMEM = "\367C";

void updateTemperature(Temperature temperature) {
    tft.setCursor(tft.width() - ((X_PIXEL_PER_CHAR * 5) + MENU_BAR_XPOS), MENU_TEXT_Y_OFFSET);
    tft.setTextColor(ST7735_RED, ST7735_BLUE);

    temperature = UNDO_ADJUST_TEMP(temperature);

    // temperature
    alignRightPrefix((uint16_t)temperature);
    tft.print(reinterpret_cast<const __FlashStringHelper *>(DEGREE_CELSIUS_TFT_STR));
}

#ifdef MAX6675_USE_FIX_PT
static coord_t cPerPx;
#else
static float cPerPx;
#endif
static uint16_t xOffset;
static uint16_t elapsed;
static uint16_t legendPeakTemp;

#define GRAPH_Y_AXIS_X_OFFSET (X_PIXEL_PER_CHAR * 4)

// We assume drawInitialProgressDisplay was already called
// This function redraws the right axis and horizontal lines, the xOffset is adjusted to start drawing from left again!
void restartDrawProgressDisplay() {
    // Set the xOffset to the elapsed time to ensure drawing starts from left again
    xOffset = elapsed;

    const coord_t tftWidth = tft.width();
    const coord_t tftHeight = tft.height();

    // clear the draw space!
    tft.fillRect(0, menuItemHeight * 2, tftWidth, tftHeight - (menuItemHeight * 3), ST7735_WHITE);

    // leave last menuItem empty for a legend
    uint16_t l = tftHeight - menuItemHeight;
    //uint16_t lDec = pxPerC * 50;
    uint16_t lDec = (uint16_t)(ADJUST_TEMP(50) / cPerPx);

    // 50°C grid
    tft.setTextColor(tft.color565(0xa0, 0xa0, 0xa0));

    for (uint16_t tg = GRAPH_TEMP_OFFSET; tg < legendPeakTemp; tg += 50, l -= lDec) {
        tft.drawFastHLine(0, l, tftWidth, tft.color565(0xe0, 0xe0, 0xe0));
        // We want the text to be just above the horizontal line
        tft.setCursor(tftWidth - GRAPH_Y_AXIS_X_OFFSET, l - (Y_PIXEL_PER_CHAR - 1));
        alignRightPrefix(tg);
        tft.print(reinterpret_cast<const __FlashStringHelper *>(DEGREE_CELSIUS_TFT_STR));
    }
}

// The peak temperature is in normal integer representation, not fixed point!
void drawInitialProgressDisplay(uint16_t peakTemp) {

    const coord_t tftHeight = tft.height();

    legendPeakTemp = (peakTemp += peakTemp >> 3);

    // Available space to draw the chart
    const uint8_t h = tftHeight - (menuItemHeight * 3);
    //pxPerC = (float)h / (peakTemp - GRAPH_TEMP_OFFSET);
    // We want to keep precision + we do not need to convert the incoming temperatures back to normal integers
    cPerPx = ADJUST_TEMP(peakTemp - GRAPH_TEMP_OFFSET) / h;

    // reset elapsed time
    elapsed = 0;

    restartDrawProgressDisplay();
}

void updateProgressDisplaySetPoint(Temperature setpoint) {
    // Redraw the set point
    // X_PIXEL_PER_CHAR pixel per char * 8 chars = 48 pixel needed
    const coord_t stateBoxMaxWidth = tft.width() - ((X_PIXEL_PER_CHAR * 8) + MENU_BAR_XPOS);

    tft.setCursor(stateBoxMaxWidth, MENU_TEXT_Y_OFFSET + menuItemHeight);
    tft.setTextColor(ST7735_BLACK, ST7735_WHITE);
    tft.print(reinterpret_cast<const __FlashStringHelper *>(PSTR("Sp:")));
    // We do not need the fraction
    alignRightPrefix((uint16_t)UNDO_ADJUST_TEMP(setpoint));
    tft.print(reinterpret_cast<const __FlashStringHelper *>(DEGREE_CELSIUS_TFT_STR));
}

void updateProgressDisplay(Temperature temperature, Temperature expectedTemp) {

    const coord_t tftWidth = tft.width();
    const coord_t tftHeight = tft.height();

    // update the current temperature
    //TODO we might want to call this as rendering routine instead of only on progress update?
    updateTemperature(temperature);

    // print elapsed time: to the left of the current temperature
    tft.setTextColor(ST7735_WHITE, ST7735_BLUE);
    tft.setCursor(tftWidth - (((X_PIXEL_PER_CHAR * 5) + MENU_BAR_XPOS) * 2), MENU_TEXT_Y_OFFSET);
    if (elapsed < 1000)
        tft.print(' ');
    alignRightPrefix(elapsed);
    tft.print("s");

    const uint8_t w = tftWidth - GRAPH_Y_AXIS_X_OFFSET;
    coord_t dx = (elapsed - xOffset);
    // If screen would be exceeded start from left again!
    if (dx > w) {
        // clear draw space again!
        restartDrawProgressDisplay();
        dx = 0;
    }

    const uint8_t h = tftHeight - menuItemHeight;

    if (expectedTemp >= ADJUST_TEMP(GRAPH_TEMP_OFFSET)) {
        expectedTemp -= ADJUST_TEMP(GRAPH_TEMP_OFFSET);
        // temperature expectedTemp
        //coord_t dy = h - UNDO_ADJUST_TEMP(expectedTemp * pxPerC);
        coord_t dy = h - expectedTemp / cPerPx;
        tft.drawPixel(dx, dy, ST7735_BLUE);
    }

    if (temperature >= ADJUST_TEMP(GRAPH_TEMP_OFFSET)) {
        temperature -= ADJUST_TEMP(GRAPH_TEMP_OFFSET);
        // actual temperature
        //coord_t dy = h - UNDO_ADJUST_TEMP(temperature * pxPerC);
        coord_t dy = h - temperature / cPerPx;
        tft.drawPixel(dx, dy, ST7735_RED);
    }

    // finally increment the elapsed time
    ++elapsed;
}

static void prepareShowMsg(color_t bg, color_t txt) {
    tft.fillRect(0, menuItemHeight, tft.width(), tft.height() - (menuItemHeight * 2), bg);

    tft.setCursor(MENU_BAR_XPOS, (menuItemHeight * 2));
    tft.setTextColor(txt);
    tft.setTextWrap(true);
}

void prepareShowMsg() {
    prepareShowMsg(ST7735_WHITE, ST7735_BLACK);
}

void afterShowMsg() {
    // Restore default settings!
    tft.setTextWrap(false);
    tft.setTextSize(1);
}

static void showError(const char *msg, bool progmem) {
    tft.setTextSize(2);

    prepareShowMsg(ST7735_RED, ST7735_WHITE);

    if (progmem) {
        tft.println(reinterpret_cast<const __FlashStringHelper *>(msg));
    } else {
        tft.println(msg);
    }

    playTones(TUNE_ERROR);
}

void showErrorMsg(const char *msg) {
    showError(msg, false);
}

void showErrorMsgP(const char *msg) {
    showError(msg, true);
}
#else
#include "makros.h"

Print *display = &Serial;

void prepareShowMsg() {
    //TODO
}
void afterShowMsg() {
    //TODO
}
bool receivedUserAction(){
    //TODO
    return true;
}
void waitForUserAction() {
    //TODO
}
void restartDrawProgressDisplay() {
    //TODO
}
void drawInitialProgressDisplay(uint16_t peakTemp) {
    //TODO
}
void updateProgressDisplay(Temperature temperature, Temperature expectedTemp) {
    //TODO
}
void updateProgressDisplaySetPoint(Temperature setpoint) {
    //TODO
}
void updateProgressDisplayState(const char *stateStr, bool isProgmem) {
    //TODO
}
void drawLegend(const char *legendStr) {
    //TODO
}

static uint16_t readInt(uint16_t lowerBound, uint16_t upperBound, bool abortOnOutOfRange) {
select_option:
    SerialPrintP(PSTR("Choose wisely("));
    SERIALprint(lowerBound);
    SERIALprint('-');
    SERIALprint(upperBound);
    SERIALprint("): ");
    do {
        _delay_ms(50);
    } while (!SERIALavailable());
    uint8_t value = SERIALparseInt();
    SERIALprintln();
    if (value < lowerBound || value > upperBound) {
        if (abortOnOutOfRange) {
            SerialPrintlnP(PSTR("Aborting..."));
            return value;
        }
        showErrorMsgP(PSTR("Out of range!"));
        goto select_option;
    }
    return value;
}

static uint8_t readIndex(uint8_t maxIndex, bool abortOnOutOfRange) {
    uint8_t value = readInt(1, maxIndex, abortOnOutOfRange);
    if (value > maxIndex) {
        return 0;
    }
    return value;
}

void initDisplay() {
}

void updateTemperature(Temperature temperature) {
    SerialPrintP(PSTR("Temp: "));
    SERIALprintln(UNDO_ADJUST_TEMP(temperature));
}

void showErrorMsg(const char *msg) {
    SERIALprintln(msg);
#ifdef USE_BUZZER
    playTones(TUNE_ERROR);
#endif
}

void showErrorMsgP(const char *msg) {
    SerialPrintlnP(msg);
#ifdef USE_BUZZER
    playTones(TUNE_ERROR);
#endif
}

static uint8_t menuElementCount = 0;

// void renderMenuItem(const Menu::Item_t *mi, uint8_t pos) {
void renderMenuItem(const Menu::Engine &engine, const Menu::Item_t *selectedItem) {
    if (selectedItem == NULL) {
        menuElementCount = engine.currentItemInfo.siblings;
    } else {
        const Menu::Item_t *mi = engine.currentItemInfo.item;
        const char *progmemStr = Menu::Engine::getLabel(mi);
        if (progmemStr != NULL) {
            SerialPrintP(progmemStr);
        }

        // mark items that have children
        if (Menu::Engine::getChild(mi) != NULL) {
            SERIALprint(" ->");
        }
        SERIALprintln();
    }
}

void showMenu(Menu::Engine &engine, const Menu::Item_t *exitItem) {
    for (;;) {
        engine.forceNewRender = true;
        engine.render(renderMenuItem, MAX_VALUE_UNSIGNED(uint8_t));

        SERIALprintln();
        uint8_t optionSelected = readIndex(menuElementCount, true);

        const Menu::Item_t *target = NULL;
        bool needsInvoke = false;

        const Menu::Item_t *parent = Menu::Engine::getParent(engine.currentItemInfo.item);
        if (optionSelected) {
            const Menu::Item_t *firstElement = Menu::Engine::getChild(parent);

            // Iterate to the target index aka optionSelected
            while ((--optionSelected) > 0) {
                firstElement = Menu::Engine::getNext(firstElement);
            }

            target = firstElement;
            needsInvoke = true;
        } else {
            target = parent;
        }

        if (target != NULL) {
            engine.navigate(target);

            if (needsInvoke) {
                engine.invoke();
            }

            if (engine.currentItemInfo.item == exitItem) {
                // Set back to the previous element
                engine.currentItemInfo.item = engine.previousItem;
                // Exit as requested
                return;
            }
        }
    }
}

void showValue(const Menu::Engine &engine, uint16_t value) {
    SerialPrintP(PSTR("Current Value: "));
    SERIALprintln(value);
}

void showStrLabel(const Menu::Engine &engine, const char *str) {
    SerialPrintP(PSTR("Current Label: "));
    SerialPrintlnP(str);
}

bool changeNum(Menu::Engine &engine, uint16_t currentValue, uint16_t lowerBound, uint16_t upperBound, void (*handleChange)(uint16_t newValue, Menu::Engine &engine)) {
    // Print value
    // assume that it is already printed? due to actionDisplay???
    //showValue(engine, currentValue);

    const uint16_t initValue = currentValue;

    bool save = true;

    for (;;) {
        const uint16_t value = readInt(lowerBound, upperBound, true);
        if (value < lowerBound || value > upperBound) {
            //TODO there is currently no way to signal to reset the value to the initvalue! Maybe ask? We need a dialog option also in other places so this might be used here as well after implemented.
            break;
        } else {

            const uint16_t prevValue = currentValue;

            currentValue = value;

            // Update value if there was a change
            if (prevValue != currentValue) {
                showValue(engine, currentValue);
                handleChange(currentValue, engine);
            }
        }
    }

    return save;
}

#endif

bool handleSelection(Menu::Action_t action, Menu::Engine &engine) {
    if (action == Menu::Action_t::actionTrigger) {
        // Emulate to go back to parent because we have selected what we wanted!
        engine.navigate(Menu::Engine::getParent(engine.currentItemInfo.item));

        // Explicit call of parent function to handle the selection and be able to differentiate between normal go back!
        engine.executeCallbackAction(Menu::Action_t::actionCustom);
    }

    return true;
}
