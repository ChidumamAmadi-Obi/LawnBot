#ifndef CONFIG
#define CONFIG 

#include "Config.h"

#if DEBUG_MOTORS
#define debugMotors(x) Serial.print(x)
#define debugMotorsln(x) Serial.println(x)
#else 
#define debugMotors(x)
#define debugMotorsln(x)
#endif 

void toggleBlade(uint8_t power){
  switch(power){
    case 0:
      digitalWrite(Config::BDLC_PWM_PIN, 0);
      break;
    case 1:
      digitalWrite(Config::BDLC_PWM_PIN, 255);
      break;
  }
}
void motorsOFF() { 
  analogWrite(Config::PWM2,0);
  analogWrite(Config::PWM1,0);
  digitalWrite(Config::DIR1,LOW);
  digitalWrite(Config::DIR2,LOW);
}
void handleMotorControl(uint8_t motorSpeed, uint8_t direction) {
  if (direction > 0) {
    analogWrite(Config::PWM2,motorSpeed);
    analogWrite(Config::PWM1,motorSpeed);    
    switch (direction) {
      case 1: // Forward
        digitalWrite(Config::DIR1,LOW);
        digitalWrite(Config::DIR2,HIGH);
        debugMotorsln("Forward");
        break;

      case 2: // Backward
        digitalWrite(Config::DIR1,HIGH);
        digitalWrite(Config::DIR2,LOW);
        debugMotorsln("Backward");
        break;

      case 3: // Left
        digitalWrite(Config::DIR1,HIGH);
        digitalWrite(Config::DIR2,HIGH);
        debugMotorsln("Left");
        break; 

      case 4: // Right
        digitalWrite(Config::DIR1,LOW);
        digitalWrite(Config::DIR2,LOW);
        debugMotorsln("Right");
        break;
    }
  } else {
    motorsOFF();
  }
  if (cutting) {
    toggleBlade(true);
  } else {
    toggleBlade(false);
  }

}
#endif
