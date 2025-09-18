#ifndef SENSOR_READINGS 
#define SENSOR_READINGS 

#include "Config.h"

#if DEBUG_SENSOR_READINGS
#define debugSensors(x) Serial.print(x)
#define debugSensorsln(x) Serial.println(x)
#else 
#define debugSensors(x)
#define debugSensorsln(x)
#endif 

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
extern unsigned long prevTOFScan;
unsigned long prevRPMMillis = 0;

uint8_t prevState1 = 0,           prevState2 = 0;
int32_t periodEncoderCount1 = 0,  periodEncoderCount2 = 0; // For RPM (reset every period)
int32_t totalEncoderCount1 = 0,   totalEncoderCount2 = 0; 
int32_t netCount1 = 0,            netCount2 = 0; 

//Tof Readings
void initTOFSensor() {
  if (!lox.begin()) {
    debugSensorsln(F("FAILED TO BOOT VL53L0X"));
    while(1);
  }
  debugSensorsln(F("VL53L0X SUCCESSFULLY CONNECTED\n\n"));
  lox.startRangeContinuous();
}
void readTOFSensor() {
  currentMillis = millis();
  if (currentMillis - prevTOFScan >= Config::TOF_SCAN_INTERVAL) {
    prevTOFScan = currentMillis;
    if (lox.isRangeComplete()) {

      mutex_enter_blocking(&sensorMutex); // protects shared variable
      objectDistance = lox.readRange();
      objectDetectionWarning = (objectDistance <= Config::TOF_THRESHOLD);
      mutex_exit(&sensorMutex);

      debugSensors("Distance in mm: ");
      if (objectDistance != 8190) {
        debugSensorsln(objectDistance);
      } else if (objectDistance <= 0) {
        // error
      }
    }
  }  
}

//Encoder Readings
void isrEncoder1() { // isrs get called every encoder pulse
  uint8_t state = (digitalRead(Config::M1_PHASE_B) << 1) | digitalRead(Config::M1_PHASE_A); 
  switch ((prevState1 << 2) | state) {
    case 0b0001: case 0b0111: case 0b1110: case 0b1000:
      totalEncoderCount1++; periodEncoderCount1++; break; // Forward
    case 0b0010: case 0b1011: case 0b1101: case 0b0100:
      totalEncoderCount1--; periodEncoderCount1--; break; // Backward
  }
  prevState1 = state;
}
void isrEncoder2() {
  uint8_t state = (digitalRead(Config::M2_PHASE_B) << 1) | digitalRead(Config::M2_PHASE_A);
  switch ((prevState2 << 2) | state) {
    case 0b0001: case 0b0111: case 0b1110: case 0b1000:
      totalEncoderCount2++; periodEncoderCount2++; break; // Forward
    case 0b0010: case 0b1011: case 0b1101: case 0b0100:
      totalEncoderCount2--; periodEncoderCount2--; break; // Backward
  }
  prevState2 = state;
}
void initEncoders() {
  prevState1 = (digitalRead(Config::M1_PHASE_B) << 1) | digitalRead(Config::M1_PHASE_A);
  prevState2 = (digitalRead(Config::M2_PHASE_B) << 1) | digitalRead(Config::M2_PHASE_A);
  
  attachInterrupt(digitalPinToInterrupt(Config::M1_PHASE_A), isrEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Config::M1_PHASE_B), isrEncoder1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Config::M2_PHASE_A), isrEncoder2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(Config::M2_PHASE_B), isrEncoder2, CHANGE);
}
void updateOdometry() {
  static int32_t lastTotal1 = 0, lastTotal2 = 0;// Calculate net counts (handles overflow by using difference)
  int32_t delta1 = totalEncoderCount1 - lastTotal1;
  int32_t delta2 = totalEncoderCount2 - lastTotal2;
  lastTotal1 = totalEncoderCount1;
  lastTotal2 = totalEncoderCount2;
  
  netCount1 += delta1;
  netCount2 += delta2;
  
  distanceLeft = (2 * PI * Config::WHEEL_RADIUS) * (netCount1 / float(Config::ENCODER_PPR));// Calculate distance traveled by each wheel (in meters)
  distanceRight = (2 * PI * Config::WHEEL_RADIUS) * (netCount2 / float(Config::ENCODER_PPR));
}
void handleEncoders() {
  currentMillis = millis();
  if ( currentMillis - prevRPMMillis >= Config::RPM_MEASURE_INTERVAL) {
    noInterrupts();
    int32_t count1 = periodEncoderCount1;
    int32_t count2 = periodEncoderCount2;
    periodEncoderCount1 = 0;
    periodEncoderCount2 = 0;
    interrupts();

    mutex_enter_blocking(&sensorMutex);
    rpm1 = abs(count1) * 60 / (Config::ENCODER_PPR * (Config::RPM_MEASURE_INTERVAL / 1000.0)); // Calculate RPM (absolute value for speed display)
    rpm2 = abs(count2) * 60 / (Config::ENCODER_PPR * (Config::RPM_MEASURE_INTERVAL / 1000.0));
    mutex_exit(&sensorMutex);

    updateOdometry();
    prevRPMMillis = currentMillis;
  }
}

#endif
