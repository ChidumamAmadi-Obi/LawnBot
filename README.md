# LawnBot

This repository documents the development of a **Wi-Fi-controlled lawn mowing robot** built around a **Raspberry Pi Pico 2 W**. The system uses the Pico's dual-core processor to efficiently handle real-time sensor data, motor control, and network communication concurrently.

The project combines **custom hardware (PCB design)**, **embedded software in C/C++**, and a lightweight **web UI** for control and testing.

## Preview 
Realtime WebSocket communication Demo

https://github.com/user-attachments/assets/61cf3d1b-3d2a-4180-a658-3717f7e33744

## 🔧 Current Features 
- Wi-Fi connectivity established through Pico 2 W  
- WebSocket communication pipeline between Pico and browser
- Basic motor control via DrRobot driver
- Obtacle detection with ToF sensors
- Test web interface for manual control (arrows, toggles, etc.)

## Hardware
- **Raspberry pi pico W2**
- **Custom PCB** (prototype + ongoing design)
- **Tof Sensor**
- **12V Motors with encoders** (planned)
- **DRI0044 DFrobot Dual motor driver**
- **12V Brushless DC motor with ESC**
- **Li-ion battery pack**

## How It Works

### Core 0: Communication and Control
 This core runs the main Arduino loop and handles all network and control tasks.
 1) On boot, core 0 **connects to Wifi and starts a web server** to serve te control interface *Webpage.h*, and initializes the WebSocket server.
 2) The *webSocketEvent()* function acts as the **main command parser** for incoming messages from the web UI.
 3) The *loop()* calls *handleMotorControl(motorSpeed, direction)*, which **translates the direction and speed into the correct PWM and digital signals for the motor driver**.
 4) Every second, the *loop()* calls *createJSON()*:
    * Locks Mutex to safely read the shared sensor data without risking corruption from Core 1.
    * Serializes the data into a JSON string.
    * **Broadcasts the JSON string to all connected web clients** via *webSocket.broadcastTXT()*.

 ### Core 1: Real-Time Sensor Reading
 This core is dedicated to reading sensors and calculating odometry with precise timing, independant of network latency.
 1) The core **initializes GPIO interrupts** on the encoder pins. Every change on these pins triggers an Interrupt Service Routine *(isrEncoder1(), isrEncoder2())*.
    * The **ISRs decode the quadrature signals** to determine the direction the motors are currently spinning. (happens in the background with minimal CPU usage).
2) In core 1's main loop, it **checks the ToF sensor every 100ms**, and if so:
   * Locks the Mutex and updates the shared *objectDistance* variable.
   * Sets the *objectDetectionWarning* flag if an object is closer than the *TOF_THRESHOLD*.
3) Every 250ms, *handleEncoders()* is called:
   * Temporarily disables interrupts to safely read and reser the *periodEncoderCount* variables.
   * **Calculates RPM of each motor**
   * Calls updateOdometry() to **calculate the total distance traveled by each wheel** using the *totalEncoderCount*.
```
RPM = (pulses_in_interval * 60) / (PULSES_PER_REVOLUTION * interval_in_seconds)
```

<img width="542" height="590" alt="Screenshot 2025-09-08 230700" src="https://github.com/user-attachments/assets/836259c3-f779-47ca-b192-80ea4b2cd4ad" />

*Flowchart of code flow*

## PCB Development
first PCB was successfully designed, manufactured, and tested.
- ✅ Verified core functionality: Pico W2, motor driver, and power delivery
- ❌ Issues identified:
  * Missing decoupling capacitors led to noise and unwanted voltage spikes
  * No support for motor encoders
  * No ToF sensor integration
  * Lack of test points and indicator LEDs for debugging
- A redesigned PCB id currently in progress with fixes and explained functionality.
- 📋 Full details: PCB Design and Schematic ![README.md](https://github.com/ChidumamAmadi-Obi/LawnBot/blob/main/PCB%20Design%20and%20Schematic/README.md)

## Setup Instructions 
1) Clone the repo:
```
git clone https://github.com/ChidumamAmadi-Obi/LawnBot.git
cd src/main
```
2) Open main.ino in ArduinoIDE with Raspberry Pi Pico board support (Raspberry Pi Pico/RP2040/RP2350 by Earle F. Philhower, III)
3) Make a new header file and name it *SSID.h*
4) In *SSID.h* copy these variables and fill out your wifi credentials:
```
const char* SSID = "YOUR_SSID";
const char* PASSWORD = "YOUR_PASSWORD";
```
5) Connect sensors and motors as per wiring diagram
```
  //PIN CONFIG
  constexpr uint8_t PWM2 = 0;
  constexpr uint8_t PWM1 = 1;
  constexpr uint8_t DIR2 = 2;
  constexpr uint8_t DIR1 = 3;
  constexpr uint8_t BDLC_PWM_PIN = 16;
  constexpr uint8_t WIFI_LED_PIN = 17;
```
<img width="785" height="591" alt="image" src="https://github.com/user-attachments/assets/3da6f02e-182f-440f-a1f8-668f92b3b850" />
7) Save this header file, and flash the code onto the Pico
8) Connect to the Pico's WiFi network and open the web interface for control

## 🚀 Road Map
- [x] **Wifi connectivity**
- [x] **Manual WebSocket control** - Full duplex communitation between client and robot
- [x] **Object Detection** - For future path finding and obstacle avoidance
- [x] **PCB Prototype** - For proof of concept
- [x] **Motor Encoder Integration** - For precise odometry
- [ ] **Full Telemetrics dashboard** — status, speed, battery level, connection quality
- [ ] **3D Printing Housing anc Chasis** - for a cost effective build and ease of use
- [ ] **Outdoor testing suite** — reliability, battery endurance, mower performance
- [ ] **Autonomous mowing mode** — full autonomy in addition to manual mode
