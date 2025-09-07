#include "Adafruit_VL53L0X.h"

#define TOF_SCAN_INTERVAL 100
#define TOF_THRESHOLD 50
#define TOF_MAX 8190

#define LED 16

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
uint8_t objectDistance;
unsigned long prevTOFScan = 0;
unsigned long currentMillis = millis();

void setup() {
  Serial.begin(115200);
  while (! Serial) {
    delay(1);
  }
  pinMode(LED,OUTPUT);

  initTOFSensor();
}

void loop() {
  readTOFSensor();
}

void initTOFSensor() {
  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));
  lox.startRangeContinuous();
}

void readTOFSensor() {
  currentMillis = millis();
  if (currentMillis - prevTOFScan >= TOF_SCAN_INTERVAL) {
    prevTOFScan = currentMillis;
    if (lox.isRangeComplete()) {
      objectDistance = lox.readRange();
      Serial.print("Distance in mm: ");
      if (objectDistance != 8190) {
        Serial.println(objectDistance);
      } else {
        Serial.println(" ( out of range )");
      }

      if (objectDistance <= TOF_THRESHOLD) {
        digitalWrite(LED,HIGH);
      } else {
        digitalWrite(LED,LOW);
      }
    }
  }  
}
 