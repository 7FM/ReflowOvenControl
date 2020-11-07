#pragma once

#include <stdint.h>

// ===================================================================================================================
// Features

// Probably does not help that much...
//#define DEBUG

// If no space is left you might want to disable this feature to run a learning cycle
// but afterwards it does not really make sense because without this feature you can basically only edit preferences and play soundtracks (if enabled)
// This is needed to run reflow profiles
#define REFLOW_FEATURE

// Lear oven characteristics needed at least once
// after that you might disable the feature for more space and hardcode the learned values above
#define LEARNING_FEATURE

// Save/load preferences to/from eeprom
#define EEPROM_SETTINGS_FEATURE

// Export/Import preferences to/from sd card
#define SD_SETTINGS_FEATURE

// Allows editing of the preferences i.e. to add another heating element or a cooling fan
#define PREF_SETTINGS
// Edit output settings
#define OUTPUT_SETTINGS

// Reflow profile settings
// Use a profile array to load the profiles to avoid malloc & free
#define PROFILE_ARRAY
#ifdef PROFILE_ARRAY
// Dont set this value to big, because we need enough space for our stack!
#define PROFILE_ARRAY_SIZE 10
#endif

// Play tunes when done with a reflow or just play some soundtracks
#define USE_BUZZER
#ifdef USE_BUZZER
// Having timer collisions?
//#define TIMER_FREE_BUZZER

// Use timer2 (8 bit) instead of timer1 (16 bit)... timer1 is used for servos
#define NEWTONE_USE_TIMER2

// Have some spare flash space? Lets change that with some soundtracks!
#define BUZZER_SOUNDTRACKS
#ifdef BUZZER_SOUNDTRACKS
// Do not use PRNG (random functions) to safe progmem
// #define BUZZER_SOUNDTRACKS_NOT_RANDOM
#endif

// Currently not used/accessible but might enable even playing of soundtracks from the sd card (ugly binary format as the soundtrack arrays... enjoy precompiling)
//#define BUZZER_TRACKS_FROM_SD
#endif

// Uses less memory if you got the needed space you might even enable long file name support even through I do not see the use case here
#define USE_SDFAT

#ifdef __AVR
// Further Pin optimizations to avoid any arduino library functions regarding pins even those to determine the port
#define PIN_OPTIMIZATIONS
#endif

// TFT display + encoder else serial communication to computer is used
#define USE_DISPLAY
// Encoder Settings
#define ENCODER_STEPS_PER_NOTCH 4
#define ENCODER_IS_ACTIVE false
#define ENCODER_ACCEL_TOP 3072 // max. acceleration: *12 (val >> 8)

#ifdef USE_DISPLAY
#ifdef DEBUG
// Enable TFT benchmark
//#define BENCHMARK
#endif
// Slow and huge adafruit library but stable! Only enable if occurring not easy fixable bugs and having a lot of storage left.
//#define ADAFRUIT_TFT_LIB
// Default: heavyly optimized adafruit lib for AVR which might contain bugs but is way faster and uses a lot less flash and is therefore recommended
#endif

// Support for servo which can open & close the oven door
//
#define SERVO_SUPPORT
// Interactive settings to adjust servo parameters
#define SERVO_SETTINGS

// Servo Settings
#define SERVO_DEFAULT_OPEN_DEGREES 90
#define SERVO_DEFAULT_CLOSE_DEGREES 90
#define SERVO_DEFAULT_MIN_PULSE_WIDTH 1000
#define SERVO_DEFAULT_MAX_PULSE_WIDTH 2000

// Default learned values for oven characteristics
#define DEFAULT_LEARNED_POWER 13
#define DEFAULT_LEARNED_INERTIA 41
#define DEFAULT_LEARNED_INSULATION 124

// Default reflow max duty cycles
#define DEFAULT_REFLOW_BOTTOM_ELEMENT_MAX_DUTY 100
#define DEFAULT_REFLOW_TOP_ELEMENT_MAX_DUTY 75
#define DEFAULT_REFLOW_BOOST_ELEMENT_MAX_DUTY 65
// Default reflow element bias
#define DEFAULT_REFLOW_BOTTOM_ELEMENT_BIAS 100
#define DEFAULT_REFLOW_TOP_ELEMENT_BIAS 80
#define DEFAULT_REFLOW_BOOST_ELEMENT_BIAS 50
// Default reflow max temperature
#define DEFAULT_REFLOW_MAX_TEMP 260
// Default reflow max deviation
#define DEFAULT_REFLOW_MAX_DEVIATION 20

#if defined(USE_DISPLAY) && !defined(DEBUG)
// Disable Serial communication to allow usage of Pin 0 and 1 only usefull if no gpio pins are left and using a display!
// Ensure to disconnect the devices connected to pin 0 and 1 when uploading a sketch!
#define DISABLE_SERIAL
#endif

// ===================================================================================================================
// Pins
// All pins except already used ones possible because software pwm was implemented!
// VCC and GND pins can be set to -1 if you do not want to use GPIO pins for that
// Hardware PWM pins: 3,5,6,9
// Pins A6 and A7 are not usable for GPIO
// A4 and A5 are used for Hardware I2C Communication (A4 SDA, A5 SCL)
// Hardware SPI uses pins 11, 12, 13 + 10(CS) (Used for sd card reader and tft display)
// keep in mind that currently no hardware PWM is used for controlling outputs

// Pin to get a seed for random functions this one should not be used else we will probably get always the same seed
// Needs to support analogRead: i.e. A6, A7
#if defined(BUZZER_SOUNDTRACKS) && !defined(BUZZER_SOUNDTRACKS_NOT_RANDOM)
#define RANDOM_INIT_ANALOG_PIN A7
#endif

// Bottom Element/Whole Oven
#define SSR_EN_PIN 6
// For example when using a PNP transistor for controlling SSR
#define SSR_EN_INVERSE_LOGIC

// Thermocouple sensor Pins
#define THERMO_SO A2
#define THERMO_CS A0
#define THERMO_CLK A1
// Might improve stability else use other power source/pins to have more free GPIO pins
#define THERMO_VCC_PIN -1
#define THERMO_GND_PIN -1

#define THERMO_SAMPLE_DELAY 250

// Buzzer (passive Piezo Buzzer... Do NOT forget a resistor (100/220 ohm)!)
#define BUZZER_PIN A3
// I strongly recommend using GND pins/power source when using all features you will need pins!
#define BUZZER_GND_PIN -1

// Servo
#define SERVO_PIN 5

// Display + SD Card
#ifdef USE_DISPLAY
#define TFT_CS 10 // TFT select pin
#define SD_CS 7   // SD card select pin
#define TFT_DC 8  // TFT display/command pin
#define TFT_RST 9 // Or set to -1 and connect to Arduino RESET pin

// Encoder Pins
#define ENCODER_B_PIN 4
#define ENCODER_A_PIN 3
#define ENCODER_SW_PIN 2
// Again I strongly recommend not using GPIO pins because we need them
#define ENCODER_VCC_PIN -1
#define ENCODER_GND_PIN -1

#else
#define SD_CS 10
#endif

// ===================================================================================================================
// Misc

#define NEWTONE_STATIC_PIN BUZZER_PIN

// Servo library conflicts with timer 1 so we need to force switching to timer 2 for sounds!
#if defined(SERVO_SUPPORT) && !defined(NEWTONE_USE_TIMER2)
#define NEWTONE_USE_TIMER2
#endif

#ifdef USE_DISPLAY
// Force use of rcall this might break compilation
// #define FORCE_RCALL

// If compiling with Wl,--relax and it fails with FORCE_RCALL enabled you should enable this option
// Else you will probably break some timing and get unexpected results with the display
// This has the implication that sometimes the delay is 1 cycle longer than needed, to be compatible with calling via call and rcall!
#define SAVE_DELAYS

// Rotary encoder performance optimizations:
// Use external interrupts for handling the encoder, this will probably reduce cpu usage but is less tested!!!
#define ROTARY_ISR_SERVICE
// Further reduce cpu usage by splitting the ISR code for the pins => assumes different interrupt vectors for each pin => we already know for the interrupt vector which pin toggled!
//TODO maybe try to remove safe logic to check for real toggles?
#define SPLIT_ROTARY_ISR_SERVICE
// if we use external interrupts for the encoder we might need debouncing to avoid crazy jitter during switching positions
#define DEBOUNCE_TIME 0
//#define DEBOUNCE_TIME 2
// we can further optimize ISR execution time by outsourcing the acceleration increment code to the getValue Method
#define ROTARY_ACCEL_OPTIMIZATION

// The encoder settings are highly dependent on the polling rate of the button
// Those values are for ~32ms polling period
#define ENCODER_ACCEL_INC 10
#define ENCODER_ACCEL_DEC 30
#endif

// Avoid float and doubles by using fixed point, because we have a precision of only 0.25 so its useless to use float/doubles besides wasteing resources
#define MAX6675_USE_FIX_PT

// ===================================================================================================================
// Sanity checks
#define _GET_PIN_INTERRUPT_LINE(pin) ((pin) <= 7 ? 2 : (pin) <= 15 ? 0 : 1)
#define _CAN_PIN_USE_INTX_INTERRUPT_LINE(pin) ((pin) == 2 || (pin) == 3)

#if _CAN_PIN_USE_INTX_INTERRUPT_LINE(ENCODER_A_PIN) || _CAN_PIN_USE_INTX_INTERRUPT_LINE(ENCODER_B_PIN) || _GET_PIN_INTERRUPT_LINE(ENCODER_A_PIN) != _GET_PIN_INTERRUPT_LINE(ENCODER_B_PIN)
#define ENCODER_PINS_USE_DIFFERENT_LINES
#endif

#if defined(USE_DISPLAY) && defined(ROTARY_ISR_SERVICE) && defined(SPLIT_ROTARY_ISR_SERVICE) && !defined(ENCODER_PINS_USE_DIFFERENT_LINES)
#error "SPLIT_ROTARY_ISR_SERVICE is not possible if ENCODER_A_PIN and ENCODER_B_PIN share the same interrupt vector!"
#endif

#if _CAN_PIN_USE_INTX_INTERRUPT_LINE(ENCODER_A_PIN)

#if ENCODER_A_PIN == 2
#define ENCODER_A_PIN_ISR_VECTOR INT0_vect
#else
#define ENCODER_A_PIN_ISR_VECTOR INT1_vect
#endif

#else

#if _GET_PIN_INTERRUPT_LINE(ENCODER_A_PIN) == 2
#define ENCODER_A_PIN_ISR_VECTOR PCINT2_vect
#elif _GET_PIN_INTERRUPT_LINE(ENCODER_A_PIN) == 1
#define ENCODER_A_PIN_ISR_VECTOR PCINT1_vect
#elif _GET_PIN_INTERRUPT_LINE(ENCODER_A_PIN) == 0
#define ENCODER_A_PIN_ISR_VECTOR PCINT0_vect
#else
#error "Can not determine interrupt vector for ENCODER_A_PIN"
#endif

#endif

#if _CAN_PIN_USE_INTX_INTERRUPT_LINE(ENCODER_B_PIN)

#if ENCODER_B_PIN == 2
#define ENCODER_B_PIN_ISR_VECTOR INT0_vect
#else
#define ENCODER_B_PIN_ISR_VECTOR INT1_vect
#endif

#else

#if _GET_PIN_INTERRUPT_LINE(ENCODER_B_PIN) == 2
#define ENCODER_B_PIN_ISR_VECTOR PCINT2_vect
#elif _GET_PIN_INTERRUPT_LINE(ENCODER_B_PIN) == 1
#define ENCODER_B_PIN_ISR_VECTOR PCINT1_vect
#elif _GET_PIN_INTERRUPT_LINE(ENCODER_B_PIN) == 0
#define ENCODER_B_PIN_ISR_VECTOR PCINT0_vect
#else
#error "Can not determine interrupt vector for ENCODER_B_PIN"
#endif

#endif
