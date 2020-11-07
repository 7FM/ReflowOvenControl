#pragma once

#include <stdint.h>

#define NUMELEMS(x) (sizeof(x) / sizeof((x)[0]))
#define CONST_STR_LEN(x) (sizeof(x) / sizeof((x)[0]) - 1)

#define MAX_VALUE_UNSIGNED(a) (((unsigned long long)-1) >> ((sizeof(unsigned long long) - sizeof(a)) * 8))
#define MAX_VALUE_SIGNED(a) (MAX_VALUE_UNSIGNED(a) >> 1)

#define MAX_SINGLE_BIT_VALUE_UNSIGNED(a) (((unsigned long long)1 << (sizeof(a) * 8)) - 1)
#define MAX_SINGLE_BIT_VALUE_SIGNED(a) (MAX_SINGLE_BIT_VALUE_UNSIGNED(a) >> 1)

#define PROGMEM_STR_ARRAY_END(x) (x + NUMELEMS(x))

#define UPPER_BYTE(x) ((int8_t)((((int16_t)(x)) >> 8) & 0x00FF))
#define LOWER_BYTE(x) ((int8_t)(((int16_t)(x)) & 0x00FF))
#define INCLUDE_INTO_ARRAY(...) __VA_ARGS__
#define SPLIT_NOTE_LENGTH(x) INCLUDE_INTO_ARRAY(UPPER_BYTE(x), LOWER_BYTE(x))
