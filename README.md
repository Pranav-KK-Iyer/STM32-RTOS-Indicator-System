![Platform](https://img.shields.io/badge/Platform-STM32-blue)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS-green)
![Language](https://img.shields.io/badge/Language-C-orange)
![Status](https://img.shields.io/badge/Status-Complete-success)


# 🚗 RTOS-Based Indicator System (STM32 + FreeRTOS)

<p align="center">
  <b>Event-driven automotive indicator system using STM32 & FreeRTOS</b><br>
  State Machine • Multitasking • Queue Communication
</p>

---

## 📌 Overview
This project implements a real-time automotive indicator system using **STM32F411RE** and **FreeRTOS**.  
It demonstrates **task scheduling, inter-task communication, and deterministic state machine behavior**.

---

## ⚙️ Key Features
- 🧵 FreeRTOS-based multitasking
- 🔄 Event-driven architecture (message queues)
- 🧠 Finite State Machine (FSM)
- ⏱️ Software debouncing + long press detection
- 🚨 Hazard mode priority handling
- ⚡ Different blink rates for different states

---

## 🧠 System Architecture
    +-------------+
    | Button Task |
    +-------------+
           |
           v
    +------------------+
    | Message Queue    |
    +------------------+
           |
           v
    +------------------+
    | Indicator Task   |
    | (State Machine)  |
    +------------------+
           |
           v
    +-------------+
    | LED Task    |
    +-------------+

---

## 🔄 State Machine

| Current State | Input Event | Next State |
|-------------- |------------ |----------- |
| IDLE          | LEFT        | LEFT       |
| IDLE          | RIGHT       | RIGHT      |
| IDLE          | BOTH        | HAZARD     |
| LEFT          | LEFT        | IDLE       |
| RIGHT         | RIGHT       | IDLE       |
| ANY           | BOTH        | HAZARD     |
| HAZARD        | LEFT/RIGHT  | IDLE       |

---

## 🛠️ Hardware Used
- STM32F411RE (Nucleo Board)
- Onboard LED (PA5)
- Push Buttons (PC0, PC1)

---

## ⚡ Behavior

| Mode  | LED Behavior|
|------ |-------------|
| LEFT  | Slow Blink  |
| RIGHT | Slow Blink  |
| HAZARD| Fast Blink  |
| IDLE  | OFF         |

---

## 📂 Project Structure
Core/
├── Src/
│ ├── main.c # RTOS tasks + logic
│
Drivers/ # HAL drivers
RTOS_Indicator.ioc # CubeMX config

---

## 🚀 Getting Started

1. Open project in STM32CubeIDE
2. Connect STM32 Nucleo board
3. Build & Flash
4. Press buttons to control indicator modes

---

## 🎯 Learning Outcomes
- RTOS scheduling & task prioritization
- Queue-based inter-task communication
- Embedded state machine design
- Hardware interfacing with STM32 HAL

---

## 📸 Demo (Add screenshots here)
- <img width="1710" height="1112" alt="Screenshot 2026-04-01 at 11 24 50 PM" src="https://github.com/user-attachments/assets/d1e70fda-5318-48e8-8af9-9e99d8b876bf" />

- <img width="1710" height="1112" alt="Screenshot 2026-04-01 at 11 25 50 PM" src="https://github.com/user-attachments/assets/ba3b32bd-d544-41a5-9247-a5a30370f884" />

- <img width="1710" height="1112" alt="Screenshot 2026-04-01 at 11 36 35 PM" src="https://github.com/user-attachments/assets/89cda281-6072-47fd-96fa-0143212fd008" />

---

## 🧠 Author
Pranav Krishnakumar Iyer
