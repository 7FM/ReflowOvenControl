#include "config.h"

#if defined(REFLOW_FEATURE) || defined(SD_SETTINGS_FEATURE) || (defined(USE_BUZZER) && defined(BUZZER_TRACKS_FROM_SD))
#include "progmem_strings.hpp"
#include "sd_wrapper.hpp"
#include "display.hpp"

#ifdef USE_SDFAT
SdFat SD;
#endif

bool initSD() {
    // Might be needed if SD reading fails due to too fast clock speeds: wire lengths on breakout board too long
    if (!SD.begin(SD_CS/*, SD_SCK_MHZ(10)*/)) {
        showErrorMsgP(PSTR("Card mount failed or not present!"));
        afterShowMsg();
        waitForUserAction();
        return false;
    }
    return true;
}
#endif