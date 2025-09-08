# LawnBot (In Progress)

![In Progress Badge](https://img.shields.io/badge/status-in%20progress-orange)  
This repository documents the development of a **Wi-Fi-controlled lawn mowing robot** built around a **Raspberry Pi Pico 2 W** and a **DrRobot motor driver**. The robot is designed to mow lawns autonomously or via manual control over a web interface. 

The project combines **custom hardware (PCB design)**, **embedded software in C/C++**, and a lightweight **web UI** for control and testing.

## 🔧 Current Features 
- Wi-Fi connectivity established through Pico 2 W  
- WebSocket communication pipeline between Pico and browser
- Basic motor control via DrRobot driver
- Obtacle detection with ToF sensors
- Test web interface for manual control (arrows, toggles, etc.)

## 🖼️ Preview
<img width="771" height="401" alt="Screenshot 2025-08-20 164518" src="https://github.com/user-attachments/assets/2472b3a8-05fd-46f1-a19e-66e5a8c821b2" />
Prototype PCB 3d render

## 🚀 Road Map
- [x] **Wifi connectivity**
- [x] **Manual WebSocket control** - Full duplex communitation between client and robot
- [x] **Object Detection** - For future path finding and obstacle avoidance
- [x] **PCB Prototype** - For proof of concept 
- [ ] **Full Telemetrics dashboard** — status, speed, battery level, connection quality
- [ ] **3D Printing Housing anc Chasis** - for a cost effective build and ease of use
- [ ] **Outdoor testing suite** — reliability, battery endurance, mower performance
- [ ] **Autonomous mowing mode** — full autonomy in addition to manual mode
