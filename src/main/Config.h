#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "pico/mutex.h"
#include <stdio.h>

#define DEBUG_MODE 2

#if DEBUG_MODE == 0 // no debuging
#define DEBUG_WIFI_CONNECTION false
#define DEBUG_WEBSOCKET_COM false
#define DEBUG_MOTOR_CONTROL false
#define DEBUG_SENSOR_READINGS false
#endif
#if DEBUG_MODE == 1 // basic debugging
#define DEBUG_WIFI_CONNECTION true
#define DEBUG_WEBSOCKET_COM false
#define DEBUG_MOTOR_CONTROL false
#define DEBUG_SENSOR_READINGS false
#endif
#if DEBUG_MODE == 2 // wifi and websocket debugging
#define DEBUG_WIFI_CONNECTION true
#define DEBUG_WEBSOCKET_COM true
#define DEBUG_MOTOR_CONTROL false
#define DEBUG_SENSOR_READINGS false
#endif
#if DEBUG_MODE == 3 // sensor debugging
#define DEBUG_WIFI_CONNECTION true
#define DEBUG_WEBSOCKET_COM false
#define DEBUG_MOTOR_CONTROL true
#define DEBUG_SENSOR_READINGS true
#endif
#if DEBUG_MODE == 4 // throughouh debugging (not reccomended)
#define DEBUG_WIFI_CONNECTION true
#define DEBUG_WEBSOCKET_COM true
#define DEBUG_MOTOR_CONTROL true
#define DEBUG_SENSOR_READINGS true
#endif

void isrEncoder1();
void isrEncoder2();

namespace Config {

  //PIN CONFIG
  constexpr uint8_t PWM2 = 0;
  constexpr uint8_t PWM1 = 1;
  constexpr uint8_t DIR2 = 2;
  constexpr uint8_t DIR1 = 3;
  constexpr uint8_t BDLC_PWM_PIN = 16;
  constexpr uint8_t WIFI_LED_PIN = 17; 

  constexpr uint8_t M1_PHASE_A = 6;
  constexpr uint8_t M1_PHASE_B = 7;
  constexpr uint8_t M2_PHASE_A = 8;
  constexpr uint8_t M2_PHASE_B = 9;

  //WEBSOCKET
  constexpr uint16_t JSON_SENDING_INVERVAL = 1000;

  //SENSOR READING CONSTANTS
  constexpr uint16_t TOF_SCAN_INTERVAL = 100;
  constexpr uint8_t TOF_THRESHOLD = 50;
  constexpr uint16_t TOF_MAX = 8190;

  constexpr uint8_t RPM_MEASURE_INTERVAL = 250;
  constexpr uint8_t ENCODER_PPR = 11;              // pulses per rotation found in the datasheet
  constexpr uint8_t WHEEL_RADIUS = 3.5;            // cm radius
  constexpr uint8_t WHEEL_BASE = 10.20;            // cm between wheels

}

extern volatile unsigned long currentMillis;
extern unsigned long prevTOFScan;

extern uint8_t direction;
extern uint8_t motorSpeed;
extern bool cutting;
extern bool control; // manual
extern uint8_t state, temp;
extern volatile uint8_t objectDistance;
extern volatile bool objectDetectionWarning;

extern volatile uint16_t rpm1, rpm2;
extern volatile int coordX, coordY;
extern volatile float distanceLeft, distanceRight; // in meters

extern WebServer server;  
extern WebSocketsServer webSocket; 

extern String json;
extern String pastjson;
extern mutex_t sensorMutex;
