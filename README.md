
# Pulse-Oximeter-arduino-matlab

**Pulse Oximeter with Arduino UNO & MATLAB App Designer**

This project implements a pulse oximeter capable of measuring:

- Blood Oxygen Saturation (SpO₂)  
- Heart Rate (BPM)  
- PPG (Photoplethysmography) signal  

The system uses a MAX30102 optical sensor, an Arduino Uno, and a MATLAB GUI built in App Designer for real‑time visualization and data logging.

---

## Hardware:

- Arduino Uno  
- MAX30102 Pulse Oximeter & Heart Rate Sensor  
- Breadboard  
- LCD 16x2 RGB display  
- USB connection to PC  

---

## Software:

- Arduino IDE  
- MATLAB (App Designer)  
- I²C communication (Arduino ←→ MAX30102)  
- Serial communication (Arduino ←→ MATLAB)  

---

## 📁 Project Structure

```
oximeter-arduino-matlab/
│
├── arduino/
│   └── oximeter.ino             → Arduino firmware
│
├── matlab_app/
│   └── Oximeter.mlapp           → MATLAB GUI (.mlapp file)
│
├── dados_utilizadores/
│   └── user_data_example.mat    → user data
│
├── imagens/
│   └── interface.png            → App interface images
│
└── README.md                    → This document
```

---

## ⚙️ System Overview

### 1. Hardware Operation

The MAX30102 uses red and infrared LEDs to measure light absorption through the user’s finger. A photodiode senses the reflected light. The sensor internally filters noise and sends digital samples to the Arduino via I²C.

Arduino is responsible for:

- Reading IR and Red values  
- Detecting finger placement  
- Calculating BPM and SpO₂  
- Sending values to MATLAB via serial port (115200 baud)  
- Displaying values on the 16x2 LCD  

---

### 2. Algorithms Used

#### Heart Rate (BPM)

The BPM calculation is based on:

- DC component removal  
- Signal inversion (to convert valleys → peaks)  
- 4‑point moving average filter  
- Peak detection  
- Interval measurement between peaks  

**Formula**

```
BPM = (SamplingRate × 60) / PeakInterval
```

---

#### SpO₂

Using the standard ratio:

```
R = (ACred / DCred) / (ACir / DCir)
```

SpO₂ is then determined using an empirically validated lookup table from the Maxim Integrated algorithm.

---

## 💻 MATLAB Application

The MATLAB App Designer interface provides:

- Real‑time plots (PPG IR & Red signals)  
- Display of BPM and SpO₂  
- User login system  
- Data storage per user (.mat files)  
- Automatic detection of serial port  
- Serial callback for streaming data  

---

### User System

Each user has a dedicated `.mat` file storing:

- Password  
- Historical BPM  
- Historical SpO₂  

If a new user logs in, a file is created.  
If an existing user logs in, their data is loaded automatically.

---

## *How to Run*

### 1. Arduino

- Open `oximeter.ino`  
- Upload to Arduino Uno  
- Ensure MAX30102 wiring  

### 2. MATLAB App

- Open MATLAB  
- Launch App Designer  
- Load `Oximeter.mlapp`  
- Run the app  
- Login or create a new user  

---

## Data Storage

All user data is stored (non‑encrypted) inside:

```
dados_utilizadores/
   └── username.mat
```

---

## Added clarification:

The `images/` folder contains the images used inside the MATLAB App (App Designer interface).

### Example:

https://github.com/user-attachments/assets/79e94a3f-ae76-4db3-80a5-fdd98a1bea7b

---
