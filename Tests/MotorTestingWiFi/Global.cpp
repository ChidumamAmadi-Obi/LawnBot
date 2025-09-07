#include "Global.h"

int direction;
int motorSpeed;
int state = 0;
bool obstacle;


//PIN CONFIGURATIONS
const uint8_t PWM2 = 0;
const uint8_t PWM1 = 1;
const uint8_t DIR2 = 2;
const uint8_t DIR1 = 3;

const uint8_t bdlcPin = 16;
const uint8_t WiFiPin = 17; 
const uint8_t sightPin = 15;