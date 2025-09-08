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
bool cutting = 0;
uint8_t state = 0;
volatile uint8_t objectDistance;
volatile bool objectDetectionWarning = false;
bool control = false; // manual
uint8_t temp = 0;

uint16_t rpm1 = 0;
uint16_t rpm2 = 0;
int coordX = 0;
int coordY = 0;

WebServer server(80);  
WebSocketsServer webSocket = WebSocketsServer(81); 

String json;
String pastjson;
mutex_t sensorMutex;