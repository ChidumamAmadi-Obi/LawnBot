#include <Arduino.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "Adafruit_VL53L0X.h"
#include "pico/mutex.h"
#include "pico/multicore.h"

#include "SSID.h"    // holds SSID information
#include "Config.h"
#include "WebPage.h"
#include "MotorControl.h"
#include "WifiManager.h"
#include "SensorReading.h"

unsigned long prevMillis = 0;

void core1_entry();

void setup() {
  Serial.begin(115200);
  mutex_init(&sensorMutex); // initialize mutex
  initTOFSensor();
  pinMode(Config::PWM1,OUTPUT);
  pinMode(Config::PWM2,OUTPUT);
  pinMode(Config::DIR1,OUTPUT);
  pinMode(Config::DIR2,OUTPUT);

  pinMode(Config::WIFI_LED_PIN,OUTPUT);
  pinMode(Config::BDLC_PWM_PIN,OUTPUT);

  multicore_reset_core1();
  multicore_launch_core1(core1_entry);

  analogWrite(Config::BDLC_PWM_PIN, 0); //makes sure the blade is not spinning  
  handleWifiConnection();
}
void loop() {
  currentMillis = millis();
  server.handleClient();
  webSocket.loop();

  //create json file
  json = createJSON();

  //send sensor readings
  currentMillis = millis();
  if ( currentMillis - prevMillis >= Config::JSON_SENDING_INVERVAL) {
    webSocket.broadcastTXT(json);
    prevMillis = currentMillis;
  }
  webSocket.broadcastTXT(json);

  //handle motor control
  handleMotorControl(motorSpeed,direction);
}
void core1_entry() {
  while (true) {
    //handles sensor readings
    readTOFSensor();
    delay(10);
  }
}
