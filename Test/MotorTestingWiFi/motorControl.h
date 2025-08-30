#define DEBUG_MOTORS 1

  #if DEBUG_MOTORS == 1 
  #define debugMotors(x) Serial.print(x)
  #define debugMotorsln(x) Serial.println(x)
  #else 
  #define debugMotors(x)
  #define debugMotorsln(x)
  #endif 


extern int direction;
extern int motorSpeed;
extern bool obstacle;

void toggleBlade(int power){
  switch(power){
    case 0:
    digitalWrite(bdlcPin, 0);
    break;

    case 1:
    digitalWrite(bdlcPin, 1);
    break;
  }
}

void motorsOFF() { 
  analogWrite(PWM2,0);
  analogWrite(PWM1,0);
}

void handleMotorControl(int motorSpeed, int direction) {

  analogWrite(PWM2,motorSpeed);
  analogWrite(PWM1,motorSpeed);

  if (motorSpeed > 0) {
    obstacle = digitalRead(sightPin);
    if (obstacle) { 
      direction = 3;
      debugMotorsln("OBSTACLE DETECTED: Moving left");
    }
    switch (direction) {
      case 1: // Forward
        digitalWrite(DIR1,LOW);
        digitalWrite(DIR2,HIGH);
        //debug("Forward");
        break;

      case 2: // Backward
        digitalWrite(DIR1,HIGH);
        digitalWrite(DIR2,LOW);
        //debug("Backward");
        break;

      case 3: // Left
        digitalWrite(DIR1,HIGH);
        digitalWrite(DIR2,HIGH);
        //debug("Left");
        break; 

      case 4: // Right
        digitalWrite(DIR1,LOW);
        digitalWrite(DIR2,LOW);
        //debug("Right");
        break;
    }
  }
}