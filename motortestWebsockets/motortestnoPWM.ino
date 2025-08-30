#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "WebPage.h" //string of html file

#define PWM2 0
#define PWM1 1
#define DIR2 2
#define DIR1 3

#define bdlcPin 16
#define WiFiPin 17

WebServer server(80);  
WebSocketsServer webSocket = WebSocketsServer(81); 

void setup() {
  Serial.begin(115200);       
  pinMode(LED_BUILTIN ,OUTPUT); 
  pinMode(bdlcPin , OUTPUT);  
  pinMode(WiFiPin, OUTPUT);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);   

  digitalWrite(bdlcPin, 0); 
 
  WiFi.begin(SSID, PASSWORD); 
  Serial.println("Establishing connection to WiFi with SSID: " + String(SSID));
 
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000);
  Serial.print(".");
  digitalWrite(LED_BUILTIN,LOW);
  }

  Serial.print("Connected to network with IP address: ");
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println(WiFi.localIP());

  server.on("/", []() {
      server.send(200, "text/html", webpage);  
  });                                         

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}
 
void loop() {
  server.handleClient();
  webSocket.loop();
}


  void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){

    switch (type) {                                     
    case WStype_DISCONNECTED:                          
      Serial.println("Client " + String(num) + " disconnected");
      digitalWrite(LED_BUILTIN,LOW);
      break;

    case WStype_CONNECTED:                             
      Serial.println("Client " + String(num) + " connected");
      break;

    case WStype_TEXT:                                   
      for (int i=0; i<length; i++) {                   
        Serial.print((char)payload[i]);
      }

      String message = String((char*)payload); 

      if(message == "CMD_blade_on"){
        toggleBlade(2);
      } else if (message == "CMD_blade_off") {
        toggleBlade(1);
      }

      if (message == "CMD_backward_on") {
        motorControl(true,2);
      } else if (message == "CMD_left_on") {
        motorControl(true,3);
      } else if (message == "CMD_right_on") {
        motorControl(true,4);
      } else if (message == "CMD_forward_on") {
        motorControl(true,1);
      } else if (message == "CMD_motors_off") {
        motorsOFF();
      }

      Serial.println("");
      break;
    }
  }

  void toggleBlade(int a){
    switch(a){
      case 1:
        digitalWrite(bdlcPin, 0);
        break;
  
      case 2:
        digitalWrite(bdlcPin, 1);
        break;
      }
  }
  
  void motorsOFF() { 
    analogWrite(PWM2,0);
    analogWrite(PWM1,0);
  }

  void motorControl(bool power, int direction) {
    if (power) {

      analogWrite(PWM2,255);
      analogWrite(PWM1,255);

      switch(direction) {
        case 1: // Forward
          digitalWrite(DIR1,LOW);
          digitalWrite(DIR2,HIGH);
        break;

        case 2: // Backward
          digitalWrite(DIR1,HIGH);
          digitalWrite(DIR2,LOW);
        break;

        case 3: // Left
          digitalWrite(DIR1,HIGH);
          digitalWrite(DIR2,HIGH);
        break; 

        case 4: // Right
          digitalWrite(DIR1,LOW);
          digitalWrite(DIR2,LOW);
        break;
      }
    }
  }
