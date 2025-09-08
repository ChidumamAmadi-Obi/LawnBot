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