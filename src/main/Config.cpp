#include <stdio.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include "pico/mutex.h"
#include "Config.h"

volatile unsigned long currentMillis;
unsigned long prevTOFScan = 0;

uint8_t direction;
uint8_t motorSpeed = 255;
uint8_t state = 0, temp = 0;
bool cutting = false;
bool control = false; // manual
volatile uint8_t objectDistance;
volatile bool objectDetectionWarning = false;

volatile uint16_t rpm1 = 0,         rpm2 = 0;
volatile int coordX = 0,            coordY = 0;
volatile float distanceLeft = 0.0,  distanceRight= 0.0;

WebServer server(80);  
WebSocketsServer webSocket = WebSocketsServer(81); 

String json;
String pastjson;
mutex_t sensorMutex;