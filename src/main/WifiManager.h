#ifndef WIFI
#define WIFI

#include "Config.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#if DEBUG_WIFI_CONNECTION
#define debugWiFi(x) Serial.print(x)
#define debugWiFiln(x) Serial.println(x)
#else 
#define debugWiFi(x)
#define debugWiFiln(x)
#endif 

#if DEBUG_WEBSOCKET_COM
#define debugWebSocket(x) Serial.print(x)
#define debugWebSocketln(x) Serial.println(x)
#else 
#define debugWebSocket(x)
#define debugWebSocketln(x)
#endif 

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){
  switch (type) {                                  
  case WStype_DISCONNECTED:                          
    debugWebSocketln("Client " + String(num) + " disconnected");
    digitalWrite(Config::WIFI_LED_PIN,LOW);
    break;

  case WStype_CONNECTED:                             
    debugWebSocketln("Client " + String(num) + " connected");
    digitalWrite(Config::WIFI_LED_PIN,HIGH);
    break;

  case WStype_TEXT:  
    debugWebSocket("RECEIVED <<- ");

    for (int i=0; i<length; i++) {                   
      debugWebSocket((char)payload[i]);
    }

    String command = String((char*)payload); 
    if(command == "CMD_blade_on"){
      cutting = true;
    } else if (command == "CMD_blade_off") {
      cutting = false;
    }

    if (command == "CMD_backward_on") {
      direction = 2;
    } else if (command == "CMD_left_on") {
      direction = 3;
    } else if (command == "CMD_right_on") {
      direction = 4;
    } else if (command == "CMD_forward_on") {
      direction = 1;
    } else if (command == "CMD_motors_off") {
      direction = 0;
    }    
    debugWebSocketln("");  
    break;
  }
}
void handleWifiConnection(){
  WiFi.begin(SSID, PASSWORD); 
  debugWiFiln("Establishing connection to WiFi with SSID: " + String(SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  debugWiFi(".");
  }

  debugWiFi("Connected to network with IP address: ");
  digitalWrite(LED_BUILTIN,HIGH);
  debugWiFiln(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage);   // 200 is the http respond code
  });                                         // "text/html" lets it know we will be sending an html file

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent); 
}
String createJSON(){
  String jsonString = "";
  JsonDocument tx_doc;
  JsonObject jsonOBJ = tx_doc.to<JsonObject>(); // create object

  mutex_enter_blocking(&sensorMutex);
  jsonOBJ["STATUS"] = state;
  jsonOBJ["X"] = coordX;
  jsonOBJ["Y"] = coordY;
  jsonOBJ["OBJ_DISTANCE"] = objectDistance;
  jsonOBJ["STATUS"] = state;
  jsonOBJ["RPM_1"] = rpm1;
  jsonOBJ["RPM_2"] = rpm2;
  mutex_exit(&sensorMutex);  
  
  serializeJson(tx_doc, jsonString);

  #endif

  debugWebSocket("SENT ->> "); debugWebSocketln(jsonString);
    
  return jsonString;
}

