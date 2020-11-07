#include "servo.hpp"
#include "prefs.hpp"

#ifdef SERVO_SUPPORT
#include <Servo.h>
static Servo servo;
#endif

void initServo() {
#ifdef SERVO_SUPPORT
    if (servo.attached()) {
        servo.detach();
    }
    if (prefs.servoPin != NO_SERVO) {
        servo.attach(prefs.servoPin, prefs.servoMinPulseWidth, prefs.servoMaxPulseWidth);
    }
#endif
}

void setServoPosition(uint8_t servoDegrees, uint16_t timeToTake) {
#ifdef SERVO_SUPPORT
    if (servo.attached()) {
        servo.write(servoDegrees, timeToTake);
    }
#endif
}