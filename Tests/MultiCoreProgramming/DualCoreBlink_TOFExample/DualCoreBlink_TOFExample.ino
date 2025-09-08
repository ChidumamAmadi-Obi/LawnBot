#include <Arduino.h>
#include "Adafruit_VL53L0X.h"
#include "pico/multicore.h"

#define LED1 15  
#define LED2 20

#define CORE0_BLINK_INTERVAL 250
#define CORE1_TOF_SENSOR_INTERVAL 100

#define TOF_SCAN_INTERVAL 100
#define TOF_THRESHOLD 50
#define TOF_OUT_OF_RANGE 8190

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
volatile uint8_t objectDistance; // shared variable between cores

void core1_entry();

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  multicore_reset_core1();
  multicore_launch_core1(core1_entry);
}

void loop() { // Core 0: Blinks LED1 and prints object distance
  blinkLED();
  Serial.print("CORE 0: Distance = ");
  if (objectDistance >= 254) {
    Serial.println("out of range");
  } else {
    Serial.print(objectDistance); Serial.println(" mm");
  }
}
void core1_entry() { //Core 1: Gathers tof sensor data
  initTOFSensor();
  while (true) {
    handleTOFSensor();
    delay(CORE1_TOF_SENSOR_INTERVAL);
    Serial.println("CORE 1: Scanned ToF sensor");
  }
}

void blinkLED() {
  digitalWrite(LED1, HIGH);
  delay(CORE0_BLINK_INTERVAL);
  digitalWrite(LED1, LOW);
  delay(CORE0_BLINK_INTERVAL);
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
void handleTOFSensor() {
  if (lox.isRangeComplete()) {
    objectDistance = lox.readRange();
    if (objectDistance <= TOF_THRESHOLD) {
      digitalWrite(LED2,HIGH);
    } else {
      digitalWrite(LED2,LOW);
    }
  }
}