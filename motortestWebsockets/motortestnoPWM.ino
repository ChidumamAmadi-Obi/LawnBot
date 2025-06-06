#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>

#include "index.h" //string of html file

#define PWM2 0 //pins that control motor driver
#define PWM1 1
#define DIR2 2
#define DIR1 3

#define bdlcPin 16 //pins that control brushless motor
#define WiFiPin    //pin that controls LED that signals WiFi connection

// #define LED_BUILTIN 2 //when using esp32

WebServer server(80);  
WebSocketsServer webSocket = WebSocketsServer(81); 

// using websockets, we can have we lightning fast full duplex communication 
// between the pico and client 🔥🔥, no need to reload the page after every update

void setup() {
  Serial.begin(115200);       
  pinMode(LED_BUILTIN ,OUTPUT); 
  pinMode(bdlcPin , OUTPUT);  
  pinMode(WiFiPin, OUTPUT);

  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);   

  digitalWrite(bdlcPin, 0); //makes sure the blade is not spinning   
 
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
      server.send(200, "text/html", webpage);   // 200 is the http respond code
  });                                           // "text/html" lets it know we will be sending an html file

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent); //whenever something happens in the websocket, the function gets called
}
 
void loop() {
  server.handleClient();
  webSocket.loop();
}

// webSocketEvent function
  void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length){
    //num = client number, identifies which client send you what
    //WStype_t = type of event you decieve
    //payload
    //size_t = length of array

    switch (type) {                                     // switch on the type of information sent
    case WStype_DISCONNECTED:                           // if a client is disconnected, then type == WStype_DISCONNECTED
      Serial.println("Client " + String(num) + " disconnected");
      digitalWrite(LED_BUILTIN,LOW);
      break;

    case WStype_CONNECTED:                              // if a client is connected, then type == WStype_CONNECTED
      Serial.println("Client " + String(num) + " connected");
      // optionally you can add code here what to do when connected
      break;

    case WStype_TEXT:                                   // if a client has sent data to the server, then type == WStype_TEXT
      for (int i=0; i<length; i++) {                    // print received data from client
        Serial.print((char)payload[i]);
      }

      String message = String((char*)payload); // holds the message in a variable 

      if(message == "CMD_blade_on"){
        toggleBlade(2);
      } else if (message == "CMD_blade_off") {
        toggleBlade(1);
      }

      if (message == "CMD_backward_on") {
        motorsON();
        backwardDrive();
      }
      else if (message == "CMD_left_on") {
        motorsON();
        leftTurn();
      }
      else if (message == "CMD_right_on") {
        motorsON();
        rightTurn();
      }
      else if (message == "CMD_forward_on") {
        motorsON();
        forwardDrive();
      }
      else if (message == "CMD_motors_off") {
        motorsOFF();
      }

      Serial.println("");
      break;
    }
  }

// custom brushless motor function
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
// custom motor functions

  void motorsOFF() { 
    analogWrite(PWM2,0);
    analogWrite(PWM1,0);
  }
    void motorsON() { 
    analogWrite(PWM2,255);
    analogWrite(PWM1,255);
  }

  void forwardDrive() { 
    digitalWrite(DIR1,LOW);
    digitalWrite(DIR2,HIGH);
  }

  void backwardDrive(){
    digitalWrite(DIR1,HIGH);
    digitalWrite(DIR2,LOW);
  }

  void rightTurn(){
    digitalWrite(DIR1,LOW);
    digitalWrite(DIR2,LOW);
  }

  void leftTurn(){
    digitalWrite(DIR1,HIGH);
    digitalWrite(DIR2,HIGH);
  }
