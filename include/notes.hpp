#pragma once

#include <avr/pgmspace.h>

#define _NOTE_B0 31
#define _NOTE_C1 33
#define _NOTE_CS1 35
#define _NOTE_D1 37
#define _NOTE_DS1 39
#define _NOTE_E1 41
#define _NOTE_F1 44
#define _NOTE_FS1 46
#define _NOTE_G1 49
#define _NOTE_GS1 52
#define _NOTE_A1 55
#define _NOTE_AS1 58
#define _NOTE_B1 62
#define _NOTE_C2 65
#define _NOTE_CS2 69
#define _NOTE_D2 73
#define _NOTE_DS2 78
#define _NOTE_E2 82
#define _NOTE_F2 87
#define _NOTE_FS2 93
#define _NOTE_G2 98
#define _NOTE_GS2 104
#define _NOTE_A2 110
#define _NOTE_AS2 117
#define _NOTE_B2 123
#define _NOTE_C3 131
#define _NOTE_CS3 139
#define _NOTE_D3 147
#define _NOTE_DS3 156
#define _NOTE_E3 165
#define _NOTE_F3 175
#define _NOTE_FS3 185
#define _NOTE_G3 196
#define _NOTE_GS3 208
#define _NOTE_A3 220
#define _NOTE_AS3 233
#define _NOTE_B3 247
#define _NOTE_C4 262
#define _NOTE_CS4 277
#define _NOTE_D4 294
#define _NOTE_DS4 311
#define _NOTE_E4 330
#define _NOTE_F4 349
#define _NOTE_FS4 370
#define _NOTE_G4 392
#define _NOTE_GS4 415
#define _NOTE_A4 440
#define _NOTE_AS4 466
#define _NOTE_B4 494
#define _NOTE_C5 523
#define _NOTE_CS5 554
#define _NOTE_D5 587
#define _NOTE_DS5 622
#define _NOTE_E5 659
#define _NOTE_F5 698
#define _NOTE_FS5 740
#define _NOTE_G5 784
#define _NOTE_GS5 831
#define _NOTE_A5 880
#define _NOTE_AS5 932
#define _NOTE_B5 988
#define _NOTE_C6 1047
#define _NOTE_CS6 1109
#define _NOTE_D6 1175
#define _NOTE_DS6 1245
#define _NOTE_E6 1319
#define _NOTE_F6 1397
#define _NOTE_FS6 1480
#define _NOTE_G6 1568
#define _NOTE_GS6 1661
#define _NOTE_A6 1760
#define _NOTE_AS6 1865
#define _NOTE_B6 1976
#define _NOTE_C7 2093
#define _NOTE_CS7 2217
#define _NOTE_D7 2349
#define _NOTE_DS7 2489
#define _NOTE_E7 2637
#define _NOTE_F7 2794
#define _NOTE_FS7 2960
#define _NOTE_G7 3136
#define _NOTE_GS7 3322
#define _NOTE_A7 3520
#define _NOTE_AS7 3729
#define _NOTE_B7 3951
#define _NOTE_C8 4186
#define _NOTE_CS8 4435
#define _NOTE_D8 4699
#define _NOTE_DS8 4978
#define _REST 0

const uint16_t NOTE_LOOKUP[] PROGMEM = {
    _NOTE_B0,
    _NOTE_C1,
    _NOTE_CS1,
    _NOTE_D1,
    _NOTE_DS1,
    _NOTE_E1,
    _NOTE_F1,
    _NOTE_FS1,
    _NOTE_G1,
    _NOTE_GS1,
    _NOTE_A1,
    _NOTE_AS1,
    _NOTE_B1,
    _NOTE_C2,
    _NOTE_CS2,
    _NOTE_D2,
    _NOTE_DS2,
    _NOTE_E2,
    _NOTE_F2,
    _NOTE_FS2,
    _NOTE_G2,
    _NOTE_GS2,
    _NOTE_A2,
    _NOTE_AS2,
    _NOTE_B2,
    _NOTE_C3,
    _NOTE_CS3,
    _NOTE_D3,
    _NOTE_DS3,
    _NOTE_E3,
    _NOTE_F3,
    _NOTE_FS3,
    _NOTE_G3,
    _NOTE_GS3,
    _NOTE_A3,
    _NOTE_AS3,
    _NOTE_B3,
    _NOTE_C4,
    _NOTE_CS4,
    _NOTE_D4,
    _NOTE_DS4,
    _NOTE_E4,
    _NOTE_F4,
    _NOTE_FS4,
    _NOTE_G4,
    _NOTE_GS4,
    _NOTE_A4,
    _NOTE_AS4,
    _NOTE_B4,
    _NOTE_C5,
    _NOTE_CS5,
    _NOTE_D5,
    _NOTE_DS5,
    _NOTE_E5,
    _NOTE_F5,
    _NOTE_FS5,
    _NOTE_G5,
    _NOTE_GS5,
    _NOTE_A5,
    _NOTE_AS5,
    _NOTE_B5,
    _NOTE_C6,
    _NOTE_CS6,
    _NOTE_D6,
    _NOTE_DS6,
    _NOTE_E6,
    _NOTE_F6,
    _NOTE_FS6,
    _NOTE_G6,
    _NOTE_GS6,
    _NOTE_A6,
    _NOTE_AS6,
    _NOTE_B6,
    _NOTE_C7,
    _NOTE_CS7,
    _NOTE_D7,
    _NOTE_DS7,
    _NOTE_E7,
    _NOTE_F7,
    _NOTE_FS7,
    _NOTE_G7,
    _NOTE_GS7,
    _NOTE_A7,
    _NOTE_AS7,
    _NOTE_B7,
    _NOTE_C8,
    _NOTE_CS8,
    _NOTE_D8,
    _NOTE_DS8,
    _REST};

enum NOTES : uint8_t {
    NOTE_B0,
    NOTE_C1,
    NOTE_CS1,
    NOTE_D1,
    NOTE_DS1,
    NOTE_E1,
    NOTE_F1,
    NOTE_FS1,
    NOTE_G1,
    NOTE_GS1,
    NOTE_A1,
    NOTE_AS1,
    NOTE_B1,
    NOTE_C2,
    NOTE_CS2,
    NOTE_D2,
    NOTE_DS2,
    NOTE_E2,
    NOTE_F2,
    NOTE_FS2,
    NOTE_G2,
    NOTE_GS2,
    NOTE_A2,
    NOTE_AS2,
    NOTE_B2,
    NOTE_C3,
    NOTE_CS3,
    NOTE_D3,
    NOTE_DS3,
    NOTE_E3,
    NOTE_F3,
    NOTE_FS3,
    NOTE_G3,
    NOTE_GS3,
    NOTE_A3,
    NOTE_AS3,
    NOTE_B3,
    NOTE_C4,
    NOTE_CS4,
    NOTE_D4,
    NOTE_DS4,
    NOTE_E4,
    NOTE_F4,
    NOTE_FS4,
    NOTE_G4,
    NOTE_GS4,
    NOTE_A4,
    NOTE_AS4,
    NOTE_B4,
    NOTE_C5,
    NOTE_CS5,
    NOTE_D5,
    NOTE_DS5,
    NOTE_E5,
    NOTE_F5,
    NOTE_FS5,
    NOTE_G5,
    NOTE_GS5,
    NOTE_A5,
    NOTE_AS5,
    NOTE_B5,
    NOTE_C6,
    NOTE_CS6,
    NOTE_D6,
    NOTE_DS6,
    NOTE_E6,
    NOTE_F6,
    NOTE_FS6,
    NOTE_G6,
    NOTE_GS6,
    NOTE_A6,
    NOTE_AS6,
    NOTE_B6,
    NOTE_C7,
    NOTE_CS7,
    NOTE_D7,
    NOTE_DS7,
    NOTE_E7,
    NOTE_F7,
    NOTE_FS7,
    NOTE_G7,
    NOTE_GS7,
    NOTE_A7,
    NOTE_AS7,
    NOTE_B7,
    NOTE_C8,
    NOTE_CS8,
    NOTE_D8,
    NOTE_DS8,
    REST,
    TUNE_END,
    TUNE_SET_NOTE_LENGTH,
};