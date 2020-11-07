#pragma once

#include "config.h"
#include <stdint.h>

#ifdef REFLOW_FEATURE
#include "menu.hpp"
#include "profile.hpp"
#include "sd_wrapper.hpp"

TOKEN getNextToken(File &profile, char *str, uint16_t *num);

void initProfiles();
Menu::Callback_t ejectSD;

#ifdef DEBUG
void dumpProfile(const char *profileName);
#endif
#endif