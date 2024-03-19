#pragma once

#include "config.h"

// THANKS TO: https://github.com/robsoncouto/arduino-songs

#if defined(USE_BUZZER) && defined(BUZZER_SOUNDTRACKS)

#include "buzzer.hpp"
#include "makros.h"

// Star Wars
// Cantina BAnd - Star wars
static const int8_t cantinaBand[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 2) / 100), NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, 3, NOTE_E5, -2, NOTE_B4, 3, REST, 3, NOTE_AS4, 3, NOTE_B4, 3, NOTE_B4, 3, NOTE_AS4, 3, NOTE_B4, 3, NOTE_A4, 3, REST, 3, NOTE_GS4, 3, NOTE_A4, 3, NOTE_G4, 3, NOTE_G4, 2, NOTE_E4, -1, NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, 3, NOTE_E5, -2, NOTE_B4, 3, REST, 3, NOTE_AS4, 3, NOTE_B4, 3, NOTE_A4, -2, NOTE_A4, -2, NOTE_GS4, 3, NOTE_A4, -2, NOTE_D5, 3, NOTE_C5, -2, NOTE_B4, -2, NOTE_A4, -2, NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, -2, NOTE_E5, -2, NOTE_B4, 3, NOTE_E5, -2, NOTE_B4, 3, REST, 3, NOTE_AS4, 3, NOTE_B4, 3, NOTE_D5, 2, NOTE_D5, -2, NOTE_B4, 3, NOTE_A4, -2, NOTE_G4, -2, NOTE_E4, -1, NOTE_E4, 1, NOTE_G4, 1, NOTE_B4, 1, NOTE_D5, 1, NOTE_F5, -2, NOTE_E5, -2, NOTE_AS4, 3, NOTE_AS4, 3, NOTE_B4, 2, NOTE_G4, 2, TUNE_END};
// Star Wars Theme
static const int8_t starWars[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 108), NOTE_AS4, 3, NOTE_AS4, 3, NOTE_AS4, 3, NOTE_F5, 1, NOTE_C6, 1, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F6, 1, NOTE_C6, 2, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F6, 1, NOTE_C6, 2, NOTE_AS5, 3, NOTE_A5, 3, NOTE_AS5, 3, NOTE_G5, 1, NOTE_C5, 3, NOTE_C5, 3, NOTE_C5, 3, NOTE_F5, 1, NOTE_C6, 1, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F6, 1, NOTE_C6, 2, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F6, 1, NOTE_C6, 2, NOTE_AS5, 3, NOTE_A5, 3, NOTE_AS5, 3, NOTE_G5, 1, NOTE_C5, -3, NOTE_C5, 4, NOTE_D5, -2, NOTE_D5, 3, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F5, 3, NOTE_F5, 3, NOTE_G5, 3, NOTE_A5, 3, NOTE_G5, 2, NOTE_D5, 3, NOTE_E5, 2, NOTE_C5, -3, NOTE_C5, 4, NOTE_D5, -2, NOTE_D5, 3, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F5, 3, NOTE_C6, -3, NOTE_G5, 4, NOTE_G5, 1, REST, 3, NOTE_C5, 3, NOTE_D5, -2, NOTE_D5, 3, NOTE_AS5, 3, NOTE_A5, 3, NOTE_G5, 3, NOTE_F5, 3, NOTE_F5, 3, NOTE_G5, 3, NOTE_A5, 3, NOTE_G5, 2, NOTE_D5, 3, NOTE_E5, 2, NOTE_C6, -3, NOTE_C6, 4, NOTE_F6, 2, NOTE_DS6, 3, NOTE_CS6, 2, NOTE_C6, 3, NOTE_AS5, 2, NOTE_GS5, 3, NOTE_G5, 2, NOTE_F5, 3, NOTE_C6, 0, TUNE_END};
// Dart Vader theme (Imperial March) - Star wars
static const int8_t darthVaderTheme[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 120), NOTE_A4, -2, NOTE_A4, -2, NOTE_A4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_F4, 3, REST, 3, NOTE_A4, -2, NOTE_A4, -2, NOTE_A4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_F4, 3, REST, 3, NOTE_A4, 2, NOTE_A4, 2, NOTE_A4, 2, NOTE_F4, -3, NOTE_C5, 4, NOTE_A4, 2, NOTE_F4, -3, NOTE_C5, 4, NOTE_A4, 1, NOTE_E5, 2, NOTE_E5, 2, NOTE_E5, 2, NOTE_F5, -3, NOTE_C5, 4, NOTE_A4, 2, NOTE_F4, -3, NOTE_C5, 4, NOTE_A4, 1, NOTE_A5, 2, NOTE_A4, -3, NOTE_A4, 4, NOTE_A5, 2, NOTE_GS5, -3, NOTE_G5, 4, NOTE_DS5, 4, NOTE_D5, 4, NOTE_DS5, 3, REST, 3, NOTE_A4, 3, NOTE_DS5, 2, NOTE_D5, -3, NOTE_CS5, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_C5, 4, REST, 3, NOTE_F4, 3, NOTE_GS4, 2, NOTE_F4, -3, NOTE_A4, -4, NOTE_C5, 2, NOTE_A4, -3, NOTE_C5, 4, NOTE_E5, 1, NOTE_A5, 2, NOTE_A4, -3, NOTE_A4, 4, NOTE_A5, 2, NOTE_GS5, -3, NOTE_G5, 4, NOTE_DS5, 4, NOTE_D5, 4, NOTE_DS5, 3, REST, 3, NOTE_A4, 3, NOTE_DS5, 2, NOTE_D5, -3, NOTE_CS5, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_C5, 4, REST, 3, NOTE_F4, 3, NOTE_GS4, 2, NOTE_F4, -3, NOTE_A4, -4, NOTE_A4, 2, NOTE_F4, -3, NOTE_C5, 4, NOTE_A4, 1, TUNE_END};

// Game of Thrones
static const int8_t got[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 85), NOTE_G4, 3, NOTE_C4, 3, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 3, NOTE_C4, 3, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, -2, NOTE_C4, -2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 2, NOTE_C4, 2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_D4, TUNE_END, NOTE_F4, -2, NOTE_AS3, -2, NOTE_DS4, 4, NOTE_D4, 4, NOTE_F4, 2, NOTE_AS3, -2, NOTE_DS4, 4, NOTE_D4, 4, NOTE_C4, TUNE_END, NOTE_G4, -2, NOTE_C4, -2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 2, NOTE_C4, 2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_D4, TUNE_END, NOTE_F4, -2, NOTE_AS3, -2, NOTE_DS4, 4, NOTE_D4, 4, NOTE_F4, 2, NOTE_AS3, -2, NOTE_DS4, 4, NOTE_D4, 4, NOTE_C4, TUNE_END, NOTE_G4, -2, NOTE_C4, -2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_G4, 2, NOTE_C4, 2, NOTE_DS4, 4, NOTE_F4, 4, NOTE_D4, -1, NOTE_F4, -2, NOTE_AS3, -2, NOTE_D4, -3, NOTE_DS4, -3, NOTE_D4, -3, NOTE_AS3, -3, NOTE_C4, TUNE_END, NOTE_C5, -1, NOTE_AS4, -1, NOTE_C4, -1, NOTE_G4, -1, NOTE_DS4, -1, NOTE_DS4, -2, NOTE_F4, -2, NOTE_G4, TUNE_END, NOTE_C5, -1, NOTE_AS4, -1, NOTE_C4, -1, NOTE_G4, -1, NOTE_DS4, -1, NOTE_DS4, -2, NOTE_D4, -2, NOTE_C5, 3, NOTE_G4, 3, NOTE_GS4, 4, NOTE_AS4, 4, NOTE_C5, 3, NOTE_G4, 3, NOTE_GS4, 4, NOTE_AS4, 4, NOTE_C5, 3, NOTE_G4, 3, NOTE_GS4, 4, NOTE_AS4, 4, NOTE_C5, 3, NOTE_G4, 3, NOTE_GS4, 4, NOTE_AS4, 4, REST, 2, NOTE_GS5, 4, NOTE_AS5, 4, NOTE_C6, 3, NOTE_G5, 3, NOTE_GS5, 4, NOTE_AS5, 4, NOTE_C6, 3, NOTE_G5, 4, NOTE_GS5, 4, NOTE_AS5, 4, NOTE_C6, 3, NOTE_G5, 3, NOTE_GS5, 4, NOTE_AS5, 4, TUNE_END};

// Super Mario Bros
static const int8_t superMario[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 200), NOTE_E5, 3, NOTE_E5, 3, REST, 3, NOTE_E5, 3, REST, 3, NOTE_C5, 3, NOTE_E5, 3, NOTE_G5, 2, REST, 2, NOTE_G4, 3, REST, 2, NOTE_C5, -2, NOTE_G4, 3, REST, 2, NOTE_E4, -2, NOTE_A4, 2, NOTE_B4, 2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, -3, NOTE_E5, -3, NOTE_G5, -3, NOTE_A5, 2, NOTE_F5, 3, NOTE_G5, 3, REST, 3, NOTE_E5, 2, NOTE_C5, 3, NOTE_D5, 3, NOTE_B4, -2, NOTE_C5, -2, NOTE_G4, 3, REST, 2, NOTE_E4, -2, NOTE_A4, 2, NOTE_B4, 2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, -3, NOTE_E5, -3, NOTE_G5, -3, NOTE_A5, 2, NOTE_F5, 3, NOTE_G5, 3, REST, 3, NOTE_E5, 2, NOTE_C5, 3, NOTE_D5, 3, NOTE_B4, -2, REST, 2, NOTE_G5, 3, NOTE_FS5, 3, NOTE_F5, 3, NOTE_DS5, 2, NOTE_E5, 3, REST, 3, NOTE_GS4, 3, NOTE_A4, 3, NOTE_C4, 3, REST, 3, NOTE_A4, 3, NOTE_C5, 3, NOTE_D5, 3, REST, 2, NOTE_DS5, 2, REST, 3, NOTE_D5, -2, NOTE_C5, 1, REST, 1, REST, 2, NOTE_G5, 3, NOTE_FS5, 3, NOTE_F5, 3, NOTE_DS5, 2, NOTE_E5, 3, REST, 3, NOTE_GS4, 3, NOTE_A4, 3, NOTE_C4, 3, REST, 3, NOTE_A4, 3, NOTE_C5, 3, NOTE_D5, 3, REST, 2, NOTE_DS5, 2, REST, 3, NOTE_D5, -2, NOTE_C5, 1, REST, 1, NOTE_C5, 3, NOTE_C5, 2, NOTE_C5, 3, REST, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_C5, 3, NOTE_C5, 2, NOTE_C5, 3, REST, 3, NOTE_C5, 3, NOTE_D5, 3, NOTE_E5, 3, REST, 0, NOTE_C5, 3, NOTE_C5, 2, NOTE_C5, 3, REST, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_E5, 3, NOTE_E5, 3, REST, 3, NOTE_E5, 3, REST, 3, NOTE_C5, 3, NOTE_E5, 2, NOTE_G5, 2, REST, 2, NOTE_G4, 2, REST, 2, NOTE_C5, -2, NOTE_G4, 3, REST, 2, NOTE_E4, -2, NOTE_A4, 2, NOTE_B4, 2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, -3, NOTE_E5, -3, NOTE_G5, -3, NOTE_A5, 2, NOTE_F5, 3, NOTE_G5, 3, REST, 3, NOTE_E5, 2, NOTE_C5, 3, NOTE_D5, 3, NOTE_B4, -2, NOTE_C5, -2, NOTE_G4, 3, REST, 2, NOTE_E4, -2, NOTE_A4, 2, NOTE_B4, 2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, -3, NOTE_E5, -3, NOTE_G5, -3, NOTE_A5, 2, NOTE_F5, 3, NOTE_G5, 3, REST, 3, NOTE_E5, 2, NOTE_C5, 3, NOTE_D5, 3, NOTE_B4, -2, /*NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_D5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_G5, -3, NOTE_F5, -3, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_B4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_F5, -3, NOTE_E5, -3, NOTE_D5, -3, NOTE_C5, 3, NOTE_E4, 2, NOTE_E4, 3, NOTE_C4, 1, NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_D5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_G5, -3, NOTE_F5, -3, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_B4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_F5, -3, NOTE_E5, -3, NOTE_D5, -3, NOTE_C5, 3, NOTE_E4, 2, NOTE_E4, 3, NOTE_C4, 1, NOTE_C5, 3, NOTE_C5, 2, NOTE_C5, 3, REST, 3, NOTE_C5, 3, NOTE_D5, 3, NOTE_E5, 3, REST, 0, NOTE_C5, 3, NOTE_C5, 2, NOTE_C5, 3, REST, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_E5, 3, NOTE_E5, 3, REST, 3, NOTE_E5, 3, REST, 3, NOTE_C5, 3, NOTE_E5, 2, NOTE_G5, 2, REST, 2, NOTE_G4, 2, REST, 2, NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_D5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_A5, -3, NOTE_G5, -3, NOTE_F5, -3, NOTE_E5, 3, NOTE_C5, 2, NOTE_A4, 3, NOTE_G4, 1, NOTE_E5, 3, NOTE_C5, 2, NOTE_G4, 3, REST, 2, NOTE_GS4, 2, NOTE_A4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_A4, 1, NOTE_B4, 3, NOTE_F5, 2, NOTE_F5, 3, NOTE_F5, -3, NOTE_E5, -3, NOTE_D5, -3, NOTE_C5, 3, NOTE_E4, 2, NOTE_E4, 3, NOTE_C4, 1, NOTE_C5, -2, NOTE_G4, -2, NOTE_E4, 2, NOTE_A4, -3, NOTE_B4, -3, NOTE_A4, -3, NOTE_GS4, -3, NOTE_AS4, -3, NOTE_GS4, -3, NOTE_G4, 3, NOTE_D4, 3, NOTE_E4, -1,*/ TUNE_END};

// Tetris
static const int8_t tetris[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 144), NOTE_E5, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_C5, 3, NOTE_B4, 3, NOTE_A4, 2, NOTE_A4, 3, NOTE_C5, 3, NOTE_E5, 2, NOTE_D5, 3, NOTE_C5, 3, NOTE_B4, -2, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 2, NOTE_C5, 2, NOTE_A4, 2, NOTE_A4, 3, NOTE_A4, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, -2, NOTE_F5, 3, NOTE_A5, 2, NOTE_G5, 3, NOTE_F5, 3, NOTE_E5, -2, NOTE_C5, 3, NOTE_E5, 2, NOTE_D5, 3, NOTE_C5, 3, NOTE_B4, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 2, NOTE_C5, 2, NOTE_A4, 2, NOTE_A4, 2, REST, 2, NOTE_E5, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_C5, 3, NOTE_B4, 3, NOTE_A4, 2, NOTE_A4, 3, NOTE_C5, 3, NOTE_E5, 2, NOTE_D5, 3, NOTE_C5, 3, NOTE_B4, -2, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 2, NOTE_C5, 2, NOTE_A4, 2, NOTE_A4, 3, NOTE_A4, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, -2, NOTE_F5, 3, NOTE_A5, 2, NOTE_G5, 3, NOTE_F5, 3, NOTE_E5, -2, NOTE_C5, 3, NOTE_E5, 2, NOTE_D5, 3, NOTE_C5, 3, NOTE_B4, 2, NOTE_B4, 3, NOTE_C5, 3, NOTE_D5, 2, NOTE_E5, 2, NOTE_C5, 2, NOTE_A4, 2, NOTE_A4, 2, REST, 2, NOTE_E5, 1, NOTE_C5, 1, NOTE_D5, 1, NOTE_B4, 1, NOTE_C5, 1, NOTE_A4, 1, NOTE_GS4, 1, NOTE_B4, 2, REST, 3, NOTE_E5, 1, NOTE_C5, 1, NOTE_D5, 1, NOTE_B4, 1, NOTE_C5, 2, NOTE_E5, 2, NOTE_A5, 1, NOTE_GS5, 1, TUNE_END};

// Star Trek intro
static const int8_t starTrekIntro[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 80), NOTE_D4, -3, NOTE_G4, 4, NOTE_C5, -2, NOTE_B4, 3, NOTE_G4, -4, NOTE_E4, -4, NOTE_A4, -4, NOTE_D5, 1, TUNE_END};

// Pacman
static const int8_t pacman[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 105), NOTE_B4, 4, NOTE_B5, 4, NOTE_FS5, 4, NOTE_DS5, 4, NOTE_B5, 5, NOTE_FS5, -4, NOTE_DS5, 3, NOTE_C5, 4, NOTE_C6, 4, NOTE_G6, 4, NOTE_E6, 4, NOTE_C6, 5, NOTE_G6, -4, NOTE_E6, 3, NOTE_B4, 4, NOTE_B5, 4, NOTE_FS5, 4, NOTE_DS5, 4, NOTE_B5, 5, NOTE_FS5, -4, NOTE_DS5, 3, NOTE_DS5, 5, NOTE_E5, 5, NOTE_F5, 5, NOTE_F5, 5, NOTE_FS5, 5, NOTE_G5, 5, NOTE_G5, 5, NOTE_GS5, 5, NOTE_A5, 4, NOTE_B5, 3, TUNE_END};

// Pink Panther
static const int8_t pinkPanther[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 120), REST, 1, REST, 2, REST, 3, NOTE_DS4, 3, NOTE_E4, -2, REST, 3, NOTE_FS4, 3, NOTE_G4, -2, REST, 3, NOTE_DS4, 3, NOTE_E4, -3, NOTE_FS4, 3, NOTE_G4, -3, NOTE_C5, 3, NOTE_B4, -3, NOTE_E4, 3, NOTE_G4, -3, NOTE_B4, 3, NOTE_AS4, 1, NOTE_A4, -4, NOTE_G4, -4, NOTE_E4, -4, NOTE_D4, -4, NOTE_E4, 1, REST, 2, REST, 3, NOTE_DS4, 2, NOTE_E4, -2, REST, 3, NOTE_FS4, 3, NOTE_G4, -2, REST, 3, NOTE_DS4, 3, NOTE_E4, -3, NOTE_FS4, 3, NOTE_G4, -3, NOTE_C5, 3, NOTE_B4, -3, NOTE_G4, 3, NOTE_B4, -3, NOTE_E5, 3, NOTE_DS5, 0, NOTE_D5, 1, REST, 2, REST, 3, NOTE_DS4, 3, NOTE_E4, -2, REST, 3, NOTE_FS4, 3, NOTE_G4, -2, REST, 3, NOTE_DS4, 3, NOTE_E4, -3, NOTE_FS4, 3, NOTE_G4, -3, NOTE_C5, 3, NOTE_B4, -3, NOTE_E4, 3, NOTE_G4, -3, NOTE_B4, 3, NOTE_AS4, 1, NOTE_A4, -4, NOTE_G4, -4, NOTE_E4, -4, NOTE_D4, -4, NOTE_E4, -2, REST, 2, REST, 2, NOTE_E5, -3, NOTE_D5, 3, NOTE_B4, -3, NOTE_A4, 3, NOTE_G4, -3, NOTE_E4, -3, NOTE_AS4, 4, NOTE_A4, -3, NOTE_AS4, 4, NOTE_A4, -3, NOTE_AS4, 4, NOTE_A4, -3, NOTE_AS4, 4, NOTE_A4, -3, NOTE_G4, -4, NOTE_E4, -4, NOTE_D4, -4, NOTE_E4, 4, NOTE_E4, 4, NOTE_E4, 1, TUNE_END};

// Harry Potter
static const int8_t harryPotter[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 144), REST, 1, NOTE_D4, 2, NOTE_G4, -2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, 1, NOTE_D5, 2, NOTE_C5, -1, NOTE_A4, -1, NOTE_G4, -2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_F4, 1, NOTE_GS4, 2, NOTE_D4, TUNE_END, NOTE_D4, 2, NOTE_G4, -2, NOTE_AS4, 3, NOTE_A4, 2, NOTE_G4, 1, NOTE_D5, 2, NOTE_F5, 1, NOTE_E5, 2, NOTE_DS5, 1, NOTE_B4, 2, NOTE_DS5, -2, NOTE_D5, 3, NOTE_CS5, 2, NOTE_CS4, 1, NOTE_B4, 2, NOTE_G4, TUNE_END, NOTE_AS4, 2, NOTE_D5, 1, NOTE_AS4, 2, NOTE_D5, 1, NOTE_AS4, 2, NOTE_DS5, 1, NOTE_D5, 2, NOTE_CS5, 1, NOTE_A4, 2, NOTE_AS4, -2, NOTE_D5, 3, NOTE_CS5, 2, NOTE_CS4, 1, NOTE_D4, 2, NOTE_D5, TUNE_END, REST, 2, NOTE_AS4, 2, NOTE_D5, 1, NOTE_AS4, 2, NOTE_D5, 1, NOTE_AS4, 2, NOTE_F5, 1, NOTE_E5, 2, NOTE_DS5, 1, NOTE_B4, 2, NOTE_DS5, -2, NOTE_D5, 3, NOTE_CS5, 2, NOTE_CS4, 1, NOTE_AS4, 2, NOTE_G4, TUNE_END, TUNE_END};

// Doom
static const int8_t doom[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 255), NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_FS3, -4, NOTE_D3, -4, NOTE_B2, -4, NOTE_A3, -4, NOTE_FS3, -4, NOTE_B2, -4, NOTE_D3, -4, NOTE_FS3, -4, NOTE_A3, -4, NOTE_FS3, -4, NOTE_D3, -4, NOTE_B2, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B3, -4, NOTE_G3, -4, NOTE_E3, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_E4, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_E4, -4, NOTE_B3, -4, NOTE_G4, -4, NOTE_B4, -4, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_DS3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_E3, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_DS3, -1, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_DS3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_E3, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_C4, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_CS3, 3, NOTE_CS3, 3, NOTE_CS4, 3, NOTE_CS3, 3, NOTE_CS3, 3, NOTE_B3, 3, NOTE_CS3, 3, NOTE_CS3, 3, NOTE_A3, 3, NOTE_CS3, 3, NOTE_CS3, 3, NOTE_G3, 3, NOTE_CS3, 3, NOTE_CS3, 3, NOTE_GS3, 3, NOTE_A3, 3, NOTE_B2, 3, NOTE_B2, 3, NOTE_B3, 3, NOTE_B2, 3, NOTE_B2, 3, NOTE_A3, 3, NOTE_B2, 3, NOTE_B2, 3, NOTE_G3, 3, NOTE_B2, 3, NOTE_B2, 3, NOTE_F3, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B3, -4, NOTE_G3, -4, NOTE_E3, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_E4, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_E4, -4, NOTE_B3, -4, NOTE_G4, -4, NOTE_B4, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_FS3, -4, NOTE_DS3, -4, NOTE_B2, -4, NOTE_FS3, -4, NOTE_DS3, -4, NOTE_B2, -4, NOTE_G3, -4, NOTE_D3, -4, NOTE_B2, -4, NOTE_DS4, -4, NOTE_DS3, -4, NOTE_B2, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E4, -4, NOTE_B3, -4, NOTE_G3, -4, NOTE_G4, -4, NOTE_E4, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_D4, -4, NOTE_E4, -4, NOTE_G4, -4, NOTE_E4, -4, NOTE_G3, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_DS3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_E3, 3, NOTE_F3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_G3, 3, NOTE_A2, 3, NOTE_A2, 3, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_C4, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_A3, -4, NOTE_F3, -4, NOTE_D3, -4, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, -1, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_AS2, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B2, 3, NOTE_C3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_E3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_D3, 3, NOTE_E2, 3, NOTE_E2, 3, NOTE_B3, -4, NOTE_G3, -4, NOTE_E3, -4, NOTE_B2, -4, NOTE_E3, -4, NOTE_G3, -4, NOTE_C4, -4, NOTE_B3, -4, NOTE_G3, -4, NOTE_B3, -4, NOTE_G3, -4, NOTE_E3, -4, TUNE_END};

// Für Elise
static const int8_t fuerElise[] PROGMEM = {TUNE_SET_NOTE_LENGTH, SPLIT_NOTE_LENGTH((60000 * 4) / 80), NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 2, REST, 3, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 3, REST, 4, NOTE_B4, 4, NOTE_C5, 4, NOTE_D5, 4, NOTE_E5, -3, NOTE_G4, 4, NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, -3, NOTE_F4, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_C5, -3, NOTE_E4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_E5, 4, NOTE_E6, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 3, REST, 4, NOTE_B4, 4, NOTE_C5, 4, NOTE_D5, 4, NOTE_E5, -3, NOTE_G4, 4, NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, -3, NOTE_F4, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_C5, -3, NOTE_E4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_E5, 4, NOTE_E6, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 3, REST, 4, NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, 4, NOTE_C5, 2, NOTE_F5, -4, NOTE_E5, 5, NOTE_E5, 3, NOTE_D5, 3, NOTE_AS5, -4, NOTE_A5, 5, NOTE_A5, 4, NOTE_G5, 4, NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_AS4, 3, NOTE_A4, 3, NOTE_A4, 5, NOTE_G4, 5, NOTE_A4, 5, NOTE_B4, 5, NOTE_C5, 2, NOTE_D5, 4, NOTE_DS5, 4, NOTE_E5, -3, NOTE_E5, 4, NOTE_F5, 4, NOTE_A4, 4, NOTE_C5, 2, NOTE_D5, -4, NOTE_B4, 5, NOTE_C5, 5, NOTE_G5, 5, NOTE_G4, 5, NOTE_G5, 5, NOTE_A4, 5, NOTE_G5, 5, NOTE_B4, 5, NOTE_G5, 5, NOTE_C5, 5, NOTE_G5, 5, NOTE_D5, 5, NOTE_G5, 5, NOTE_E5, 5, NOTE_G5, 5, NOTE_C6, 5, NOTE_B5, 5, NOTE_A5, 5, NOTE_G5, 5, NOTE_F5, 5, NOTE_E5, 5, NOTE_D5, 5, NOTE_G5, 5, NOTE_F5, 5, NOTE_D5, 5, NOTE_C5, 5, NOTE_G5, 5, NOTE_G4, 5, NOTE_G5, 5, NOTE_A4, 5, NOTE_G5, 5, NOTE_B4, 5, NOTE_G5, 5, NOTE_C5, 5, NOTE_G5, 5, NOTE_D5, 5, NOTE_G5, 5, NOTE_E5, 5, NOTE_G5, 5, NOTE_C6, 5, NOTE_B5, 5, NOTE_A5, 5, NOTE_G5, 5, NOTE_F5, 5, NOTE_E5, 5, NOTE_D5, 5, NOTE_G5, 5, NOTE_F5, 5, NOTE_D5, 5, NOTE_E5, 5, NOTE_F5, 5, NOTE_E5, 5, NOTE_DS5, 5, NOTE_E5, 5, NOTE_B4, 5, NOTE_E5, 5, NOTE_DS5, 5, NOTE_E5, 5, NOTE_B4, 5, NOTE_E5, 5, NOTE_DS5, 5, NOTE_E5, -3, NOTE_B4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, -3, NOTE_B4, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 3, REST, 4, NOTE_B4, 4, NOTE_C5, 4, NOTE_D5, 4, NOTE_E5, -3, NOTE_G4, 4, NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, -3, NOTE_F4, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_C5, -3, NOTE_E4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_E5, 4, NOTE_E6, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_D5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, 3, REST, 4, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, 3, REST, 4, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 3, REST, 4, REST, 4, REST, 3, NOTE_CS5, -2, NOTE_D5, 2, NOTE_E5, 4, NOTE_F5, 4, NOTE_F5, 2, NOTE_F5, 3, NOTE_E5, -2, NOTE_D5, 2, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, 2, NOTE_A4, 3, NOTE_A4, 3, NOTE_C5, 3, NOTE_B4, 3, NOTE_A4, -2, NOTE_CS5, -2, NOTE_D5, 2, NOTE_E5, 4, NOTE_F5, 4, NOTE_F5, 2, NOTE_F5, 3, NOTE_F5, -2, NOTE_DS5, 2, NOTE_D5, 4, NOTE_C5, 4, NOTE_AS4, 2, NOTE_A4, 3, NOTE_GS4, 2, NOTE_G4, 3, NOTE_A4, -2, NOTE_B4, 2, REST, 3, NOTE_A3, -5, NOTE_C4, -5, NOTE_E4, -5, NOTE_A4, -5, NOTE_C5, -5, NOTE_E5, -5, NOTE_D5, -5, NOTE_C5, -5, NOTE_B4, -5, NOTE_A4, -5, NOTE_C5, -5, NOTE_E5, -5, NOTE_A5, -5, NOTE_C6, -5, NOTE_E6, -5, NOTE_D6, -5, NOTE_C6, -5, NOTE_B5, -5, NOTE_A4, -5, NOTE_C5, -5, NOTE_E5, -5, NOTE_A5, -5, NOTE_C6, -5, NOTE_E6, -5, NOTE_D6, -5, NOTE_C6, -5, NOTE_B5, -5, NOTE_AS5, -5, NOTE_A5, -5, NOTE_GS5, -5, NOTE_G5, -5, NOTE_FS5, -5, NOTE_F5, -5, NOTE_E5, -5, NOTE_DS5, -5, NOTE_D5, -5, NOTE_CS5, -5, NOTE_C5, -5, NOTE_B4, -5, NOTE_AS4, -5, NOTE_A4, -5, NOTE_GS4, -5, NOTE_G4, -5, NOTE_FS4, -5, NOTE_F4, -5, NOTE_E4, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, -3, REST, -3, REST, -3, NOTE_G4, 4, NOTE_F5, 4, NOTE_E5, 4, NOTE_D5, 2, REST, 3, REST, -3, NOTE_E4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_E5, 3, NOTE_E5, 3, NOTE_E6, -3, NOTE_DS5, 4, NOTE_E5, 4, REST, 4, REST, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_GS4, 4, NOTE_B4, 4, NOTE_C5, 3, REST, 4, NOTE_E4, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_DS5, 4, NOTE_E5, 4, NOTE_B4, 4, NOTE_D5, 4, NOTE_C5, 4, NOTE_A4, -3, NOTE_C4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_B4, -3, NOTE_E4, 4, NOTE_C5, 4, NOTE_B4, 4, NOTE_A4, -2, TUNE_END};

const int8_t *const soundtracks[] PROGMEM = {
    cantinaBand,
    starWars,
    //TODO undo
    /*
    darthVaderTheme,
    got,
    superMario,
    tetris,
    starTrekIntro,
    pacman,
    pinkPanther,
    harryPotter,
    */
    // doom,
    //fuerElise
};

#define SOUNDTRACK_COUNT NUMELEMS(soundtracks)

#endif