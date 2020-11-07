#include "config.h"

#ifdef REFLOW_FEATURE
#include <string.h>
#ifndef PROFILE_ARRAY
#include <stdlib.h>
#endif

#include "profile.hpp"
#include "sd_wrapper.hpp"

#include "utils.hpp"

//TODO maybe instead a ringBuffer?
#ifdef PROFILE_ARRAY
static profiles profileRingBuf[PROFILE_ARRAY_SIZE];
static uint8_t nextUnused = 0;
#endif

profiles *createNewProfile() {

    static_assert(offsetof(profiles, item) == 0, "Expected different alignment of profile attributes!");

#ifdef PROFILE_ARRAY
    if (nextUnused == PROFILE_ARRAY_SIZE)
        return NULL;

    // Zero initialied profile struct
    profiles *newProfile = profileRingBuf + nextUnused;
    memset(newProfile, 0, sizeof(*newProfile));
#else
    // Zero initialied profile struct
    profiles *newProfile = (profiles *)calloc(1, sizeof(profiles));
#endif

    // Initialize the label with a pointer to name field
    newProfile->item.Label = newProfile->name;
    // Set the handleSelection as default callback
    newProfile->item.Callback = handleSelection;
    // Set the parent to the extern defined exit item
    newProfile->item.Parent = &profileExitItem;

#ifdef PROFILE_ARRAY
    nextUnused = (nextUnused + 1);
#endif

    return newProfile;
}

#ifndef PROFILE_ARRAY
void releaseProfile(profiles *profile) {
    free(profile);
}
#else
void releaseProfile(profiles *profile) {
    profile->item.Next = NULL;
    profile->item.Previous = NULL;
    profile->item.Child = NULL;
}
#endif

void freeProfileList(profiles **profileList) {
    profiles *current = *profileList;
    *profileList = NULL;
    while (current != NULL) {
        profiles *next = (profiles *)current->item.Next;
        releaseProfile(current);
        current = next;
    }

#ifdef PROFILE_ARRAY
    nextUnused = 0;
#endif
}

static inline uint8_t sanitizeProfileName(char *buffer, const char *profileName) {
    uint8_t i;
    char c;
    for (i = 0; i < PRECOMPILE_NAME_LEN && (c = profileName[i]) != '\0'; ++i) {
        if (c <= ' ' || c == '.' || c == '+' || c == '<' || c == '>' || c == ':' || c == '"' || c == '/' || c == '\\' || c == '|' || c == '?' || c == '*') {
            c = '_';
        }
        buffer[i] = c;
    }
    //TODO we could also append the suffix here directly!

    return i;
}

static const char *getPrecompiledProfilePath(const char *profileName) {
    static char path[PRECOMPILE_FOLDER_LEN + PRECOMPILE_SUFFIX_LEN + PRECOMPILE_NAME_LEN + 1] = PRECOMPILE_FOLDER;
    // Append suffix with its zero character termination
    memcpy(path + PRECOMPILE_FOLDER_LEN + sanitizeProfileName(path + PRECOMPILE_FOLDER_LEN, profileName), PRECOMPILE_SUFFIX, PRECOMPILE_SUFFIX_LEN + 1);
    return path;
}

File openPrecompiledProfile(const char *profileName, bool read) {
    const char *path = getPrecompiledProfilePath(profileName);
    if (!read && SD.exists(path)) {
        SD.remove(path);
    }

    return SD.open(path, read ? FILE_READ : FILE_WRITE);
}

void deleteProfileFile(const char *profileName) {
    const char *path = getPrecompiledProfilePath(profileName);
    if (SD.exists(path)) {
        SD.remove(path);
    }
}

#endif
