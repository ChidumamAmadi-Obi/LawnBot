#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>

extern int direction;
extern int motorSpeed;
extern bool obstacle;

//PIN CONFIGURATIONS
extern const uint8_t PWM2;
extern const uint8_t PWM1;
extern const uint8_t DIR2;
extern const uint8_t DIR1;

extern const uint8_t bdlcPin;
extern const uint8_t WiFiPin;
extern const uint8_t sightPin;

#endif