
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "Global.h"
#include "WebPage.h"
#include "SSID.h"    // holds SSID information
#include "motorControl.h"

#define DEBUG 1

  #if DEBUG == 1 
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
  #else 
  #define debug(x)
  #define debugln(x)
  #endif 

WebServer server(80);  
WebSocketsServer webSocket = WebSocketsServer(81); 

void setup() {
  Serial.begin(115200);       
  pinMode(LED_BUILTIN , OUTPUT); 
  pinMode(bdlcPin , OUTPUT);  
  pinMode(WiFiPin , OUTPUT);
  pinMode(sightPin , INPUT);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);   

  digitalWrite(bdlcPin, 0); //makes sure the blade is not spinning   
 
  WiFi.begin(SSID, PASSWORD); 
  debugln("Establishing connection to WiFi with SSID: " + String(SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  debug(".");
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  debug(".");
  digitalWrite(LED_BUILTIN,LOW);
  }

  debug("Connected to network with IP address: ");
  digitalWrite(LED_BUILTIN,HIGH);
  debugln(WiFi.localIP());

  server.on("/", []() {
    server.send(200, "text/html", webpage);   // 200 is the http respond code
  });                                           // "text/html" lets it know we will be sending an html file

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent); //whenever something happens in the websocket, the function gets called
}
 
void loop() {
  server.handleClient();
  webSocket.loop();
  handleMotorControl(motorSpeed,direction);
}

//___________________________________________________________
  void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){
    //num = client number, identifies which client send you what
    //WStype_t = type of event you decieve
    //payload
    //size_t = length of array

    switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                           // if a client is disconnected, then type == WStype_DISCONNECTED
      debugln("Client " + String(num) + " disconnected");
      digitalWrite(LED_BUILTIN,LOW);
      break;

    case WStype_CONNECTED:                              // if a client is connected, then type == WStype_CONNECTED
      debugln("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      break;

    case WStype_TEXT:                                   // if a client has sent data to the server, then type == WStype_TEXT
      for (int i=0; i<length; i++) {                    // print received data from client
        debug((char)payload[i]);
      }

      String message = String((char*)payload); // holds the message in a variable 

      if(message == "CMD_blade_on"){
        toggleBlade(1);
      } else if (message == "CMD_blade_off") {
        toggleBlade(0);
      }

      if (message == "CMD_backward_on") {
        direction = 2;
        motorSpeed = 255;
      } else if (message == "CMD_left_on") {
        direction = 3;
        motorSpeed = 255;
      } else if (message == "CMD_right_on") {
        direction = 4;
        motorSpeed = 255;
      } else if (message == "CMD_forward_on") {
        direction = 1;
        motorSpeed = 255;
      } else if (message == "CMD_motors_off") {
        motorSpeed = 0;
      }

      Serial.println("");
      break;
    }
  }
