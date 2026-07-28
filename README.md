<img width="1277" height="684" alt="IMG_3056" src="https://github.com/user-attachments/assets/cb60c65d-9375-410d-a35c-a4b2c38aec18" />


<div align="center">

# 💊 Smart Pillbox: Embedded Medication Adherence System

[![Arduino](https://img.shields.io/badge/Hardware-Arduino%20Nano-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Status](https://img.shields.io/badge/Project%20Status-Completed-brightgreen?style=for-the-badge)]()
[![Domain](https://img.shields.io/badge/Domain-Embedded%20Systems-orange?style=for-the-badge)]()
[![License](https://img.shields.io/badge/License-MIT-blue?style=for-the-badge)](LICENSE)

*An automated, hardware-interrupt driven embedded system designed to track pill dosage, prevent missed or double dosing, and monitor patient adherence in real-time.*

[Overview](#-overview) • [Key Features](#-key-features) • [Tech Stack](#-technical-stack) • [System Architecture](#-core-system-operations) • [Hardware Wiring](#-getting-started)

---

### ⚡ Quick Feature Highlights

| ⏱️ Hardware Timers | ⚖️ High-Precision Scale | 🖥️ Real-time Display | 🔔 Smart Alerts |
| :---: | :---: | :---: | :---: |
| Non-blocking Timer1 CTC Interrupts (1Hz) | HX711 24-bit ADC + Load Cell | 128x64 SSD1306 OLED Screen | Active Buzzer & Serial Logging |

---

</div>

## 📖 Overview

Medication adherence is essential for maintaining optimal health, especially for individuals managing chronic diseases or complex medication regimens. However, forgetting doses or overdosing is a common challenge. 

The **Smart Pillbox** is an embedded hardware solution designed to solve this problem by providing automated reminders and physical tracking of pill consumption. By utilizing a high-precision load cell, an SSD1306 OLED display, and timer-based microcontroller interrupts, this system accurately counts remaining medication and logs patient adherence in real-time.

---

## ✨ Key Features

*   **Real-Time Pill Counting:** Uses a load cell and HX711 ADC to measure total weight and calculate the exact number of pills remaining.
*   **Hardware Interrupt-Driven Alerts:** Utilizes Arduino's Timer1 and internal clock interrupts to trigger timely medication reminders without blocking the main execution loop.
*   **Adherence Tracking & Logging:** Systematically compares previous and current pill counts to log missed or successfully taken doses via Serial communication.
*   **OLED User Interface:** Provides an at-a-glance, real-time display of the current pill count and system status.
*   **Dynamic Calibration (Taring):** Includes a physical hardware interrupt (button) to manually zero out the scale for accurate baseline readings.

---

## 🧰 Technical Stack

| Component | Description |
| :--- | :--- |
| **Microcontroller** | Arduino Nano (ATmega328P) |
| **Sensors** | Load Cell (0-1kg/5kg) with HX711 24-bit ADC module |
| **Display** | 128x64 OLED Display (I2C via Adafruit SSD1306) |
| **Actuators** | 5V Active Buzzer for auditory alerts |
| **Language** | C / C++ |
| **Key Libraries** | `HX711_ADC.h`, `Adafruit_GFX.h`, `Adafruit_SSD1306.h` |

---

## ⚙️ Core System Operations

### 1. Embedded Concept: Hardware Timers & ISR
Instead of relying on blocking `delay()` functions, this project configures the ATmega328P's **Timer1** in CTC (Clear Timer on Compare Match) mode. 
*   The timer prescaler is set to 1024, yielding a 15,625 Hz clock frequency. 
*   `OCR1A` is set to 15624, triggering an Interrupt Service Routine (ISR) exactly every 1 second.
*   This ensures the system can continuously poll the load cell while accurately tracking a 30-second dose reminder window and a 60-second adherence check window.

### 2. Algorithmic Pill Calculation
The system calculates the pill count dynamically based on the weight of a single unit. During startup, the scale is tared, and the total mass is divided by the pre-configured single pill weight (currently set to 0.9g).

*Pill Count = Total Weight / Weight of one pill*

### 3. State Machine & Adherence Logic
*   **T=30s:** The timer interrupt triggers the buzzer and prompts the user to take the pill via the Serial Monitor.
*   **T=40s (Internal check):** The system assesses if `currentCount < previousCount`. If true, the state updates to `pill_taken = true`.
*   **T=60s:** The timer resets. If the pill was taken, a success log is recorded; otherwise, a missed dose is flagged.

---

## 🧠 Engineering Trade-Offs & Limitations

A crucial part of embedded design is understanding system constraints. The current iteration includes the following design considerations:

*   **Single-Pill Type Limitation:** The system is calibrated for one specific type of pill at a time. The high sensitivity of the HX711 makes it computationally complex to differentiate between varying pill weights simultaneously.
*   **Manual Weight Initialization:** To avoid startup delays and improve baseline accuracy, the weight of a single pill is established as a manual input rather than relying on automated single-pill measurement during boot.
*   **Transient Load Filtering:** Because the load cell is sensitive enough to register human hand pressure, adherence checks (weight reductions) are measured over specific interval windows to ignore transient spikes.

---

## 🚀 Future Roadmap

To scale this proof-of-concept into a production-ready medical device, the following enhancements are planned:

*   **IoT & Cloud Integration:** Transitioning to an ESP32 to push adherence logs to a cloud database, allowing remote monitoring by healthcare providers.
*   **Companion Mobile App:** Developing a smartphone interface to allow users to remotely set medication schedules and view daily/weekly adherence analytics.
*   **Multi-Pill Algorithms:** Implementing multiple localized load cells or advanced DSP (Digital Signal Processing) to handle varying pill types.
*   **Enhanced UI:** Upgrading to a touchscreen interface to improve accessibility for elderly patients.

---

## 🛠️ Getting Started

### Hardware Wiring

| Device | Arduino Pin |
| :--- | :--- |
| HX711 DT (Data) | D2 |
| HX711 SCK (Clock) | D3 |
| OLED SDA | A4 |
| OLED SCL | A5 |
| Buzzer | D12 |
| Tare Button | D13 |

### Installation
1. Clone this repository to your local machine.
2. Open the `.ino` file in the Arduino IDE.
3. Install the required dependencies (`Adafruit GFX`, `Adafruit SSD1306`, `HX711_ADC`) via the Arduino Library Manager.
4. Adjust the calibration factor in `LoadCell.setCalFactor(-1850.26);` and the specific `weight` variable to match your physical hardware and pill type.
5. Flash to the Arduino Nano and open the Serial Monitor at **9600 baud**.
