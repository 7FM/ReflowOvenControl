#include "config.h"

#if defined(REFLOW_FEATURE) || defined(LEARNING_FEATURE) || defined(USE_BUZZER)
#include "buzzer.hpp"
#include "utils.hpp"
#include "makros.h"

#ifdef BUZZER_TRACKS_FROM_SD
#include "sd_wrapper.hpp"
#endif

#ifdef USE_BUZZER
#include "pin.hpp"
#ifdef BUZZER_SOUNDTRACKS
#include "soundtracks.hpp"
#endif
#ifdef TIMER_FREE_BUZZER
#include <TimerFreeTone.h>
#else
#include <NewTone.h>
#endif
#endif

#ifdef USE_BUZZER
static const int8_t tuneStartup[] PROGMEM = {NOTE_C5, 3, NOTE_G2, 3, TUNE_END};
static const int8_t tuneReflowDone[] PROGMEM = {NOTE_C5, 2, NOTE_G2, 3, NOTE_G2, 3, NOTE_A2, 2, NOTE_G2, 2, NOTE_B2, 2, NOTE_C5, 2, TUNE_END};
static const int8_t tuneRemoveBoards[] PROGMEM = {NOTE_C5, 2, NOTE_B2, 2, NOTE_E2, 1, TUNE_END};
static const int8_t tuneReflowBeep[] PROGMEM = {NOTE_C2, 4, NOTE_B5, 3, TUNE_END};
static const int8_t tuneError[] PROGMEM = {NOTE_FS6, 3, NOTE_B5, 3, NOTE_B6, 3, TUNE_END};

const int8_t *const listOfTunes[] PROGMEM = {tuneStartup, tuneReflowDone, tuneRemoveBoards, tuneReflowBeep, tuneError};

#define MAX_TUNES NUMELEMS(listOfTunes)

#else
#define MAX_TUNES 0
#endif

#ifdef USE_BUZZER

typedef int8_t (*TrackReader)(void *arg);

// Use lookup table to map enum values to their frequency!
static inline uint16_t lookup(uint8_t reference) {
    return pgm_read_word(NOTE_LOOKUP + reference);
}

void playTones(TrackReader reader, void *arg) {
    int16_t wholenote = 1000;
    uint8_t frequencyToLookUp = reader(arg);
    if (frequencyToLookUp == TUNE_SET_NOTE_LENGTH) {
        // Handle special case: 16 bit big endian note length
        wholenote = reader(arg) << 8 | (((uint16_t)reader(arg)) & 0x00FF);
        frequencyToLookUp = reader(arg);
    }
    do {
        int16_t frequency = lookup(frequencyToLookUp);

        // Note durations: 2^2 = 4 = quarter note, 2^3 = 8 = eighth note, etc.:
        int16_t duration = reader(arg);
        if (duration < 0) {
            duration = wholenote >> (-duration);
            duration = duration + (duration >> 1);
        } else {
            duration = wholenote >> (duration);
        }
#ifdef TIMER_FREE_BUZZER
        TimerFreeTone(BUZZER_PIN, frequency, duration);
        _delay_ms(50);
#else
        // we only play the note for 90% of the duration, leaving 10% as a pause
#ifndef NEWTONE_STATIC_PIN
        NewTone<BUZZER_PIN>(frequency, (duration * 9) / 10);
#else
        NewTone(frequency, (duration * 9) / 10);
#endif

        // Wait for the specified duration before playing the next note.
        delay(duration);

        // stop the waveform generation before the next note.
        //noNewTone();
#endif
        frequencyToLookUp = reader(arg);
    } while (frequencyToLookUp != TUNE_END);
}

int8_t localTrackReader(void *soundtrackPtr) {
    return pgm_read_byte((*(const int8_t **)soundtrackPtr)++);
}
#endif

#ifdef BUZZER_TRACKS_FROM_SD
#ifdef USE_BUZZER
int8_t sdTrackReader(void *soundtrackPtr) {
    return ((File *)soundtrackPtr)->read();
}
#endif

void playTones(File &soundFile) {
#ifdef USE_BUZZER
    playTones(sdTrackReader, &soundFile);
#endif
}
#endif

void playTones(const int8_t *soundtrack) {
#ifdef USE_BUZZER
    playTones(localTrackReader, &soundtrack);
#endif
}

void playTones(Tunes tune) {
#ifdef USE_BUZZER
    if (tune >= MAX_TUNES)
        return;

    playTones((const int8_t *const)pgm_read_ptr(listOfTunes + tune));
#endif
}

void initBuzzer() {
#ifdef USE_BUZZER
#if defined(BUZZER_GND_PIN) && BUZZER_GND_PIN != -1
    initPinLOWConstExpr(BUZZER_GND_PIN);
#endif
    initPinLOWConstExpr(BUZZER_PIN);

#if defined(BUZZER_SOUNDTRACKS) && !defined(BUZZER_SOUNDTRACKS_NOT_RANDOM)
    randomSeed(analogRead(RANDOM_INIT_ANALOG_PIN));
#endif
#endif
}

void playRandomTrack() {
#if defined(USE_BUZZER) && defined(BUZZER_SOUNDTRACKS)
    uint8_t offset;
#ifdef BUZZER_SOUNDTRACKS_NOT_RANDOM
    static uint8_t lastOffset = 0;

    // equivalent short version offset = lastOffset = (lastOffset + 1) % SOUNDTRACK_COUNT;
    offset = lastOffset + 1;
    if (offset == SOUNDTRACK_COUNT) {
        offset = 0;
    }
    lastOffset = offset;
#else
    offset = random(0, SOUNDTRACK_COUNT);
#endif
    playTones((const int8_t *const)pgm_read_ptr(soundtracks + offset));
#endif
}

#endif
