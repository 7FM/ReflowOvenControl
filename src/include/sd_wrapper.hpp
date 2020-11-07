#pragma once

#include "config.h"

#if defined(REFLOW_FEATURE) || defined(SD_SETTINGS_FEATURE) || (defined(USE_BUZZER) && defined(BUZZER_TRACKS_FROM_SD))
#ifdef USE_SDFAT
#include <SdFat.h>

#undef FILE_WRITE
#define FILE_WRITE O_WRONLY | O_CREAT

extern SdFat SD;
#else
#include <SD.h>
#endif

bool initSD();
#endif