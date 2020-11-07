#pragma once

#include <stdint.h>

void initServo();
void setServoPosition(uint8_t servoDegrees, uint16_t timeToTake);