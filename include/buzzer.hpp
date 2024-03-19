// Written by Peter Easton
// Released under the MIT license
// Build a reflow oven: https://whizoo.com

#pragma once

#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE) || defined(USE_BUZZER)
#include <Arduino.h>

#include "notes.hpp"
#ifdef BUZZER_TRACKS_FROM_SD
#include "sd_wrapper.hpp"
#endif

// Tunes
enum Tunes : uint8_t {
    TUNE_STARTUP,
    TUNE_REFLOW_DONE,
    TUNE_REMOVE_BOARDS,
    TUNE_REFLOW_BEEP,

    TUNE_ERROR
};

// Play a tone
// Parameter: tone - an array containing alternating notes and note duration, terminated by TUNE_END
// needs to be stored in progmem
// if it begins with TUNE_SET_NOTE_LENGTH the next 2 values (big endian) are assumed to be the length of a note => controlls play speed
// you have to store log2 of the duration! i.e. a quarter note (4) would be saved as 2
// negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighth!!
// Durations must be smaller than 127 (to fit a int8_t) or must be split into repeating the same note!
void playTones(const int8_t *soundtrack);
#ifdef BUZZER_TRACKS_FROM_SD
void playTones(File &soundFile);
#endif
void playTones(Tunes tune);

void initBuzzer();

#ifdef BUZZER_SOUNDTRACKS
void playRandomTrack();
#endif
#endif