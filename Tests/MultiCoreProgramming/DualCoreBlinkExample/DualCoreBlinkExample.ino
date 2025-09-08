#include <Arduino.h>
#include "pico/multicore.h"

#define LED1 15  
#define LED2 16  

#define CORE0_BLINK_INTERVAL 250
#define CORE1_BLINK_INTERVAL 100

void core1_entry();

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  
  multicore_reset_core1();
  multicore_launch_core1(core1_entry);
}

void loop() { // Core 0: Blink LED1 slowly
  digitalWrite(LED1, HIGH);
  delay(CORE0_BLINK_INTERVAL);
  Serial.println("CORE 0: LED ON");
  digitalWrite(LED1, LOW);
  delay(CORE0_BLINK_INTERVAL);
}

void core1_entry() { // Core 1: Blink LED2 faster
  while (true) {
    digitalWrite(LED2, HIGH);
    delay(CORE1_BLINK_INTERVAL);
    Serial.println("CORE 1: LED ON");
    digitalWrite(LED2, LOW);
    delay(CORE1_BLINK_INTERVAL);
  }
}