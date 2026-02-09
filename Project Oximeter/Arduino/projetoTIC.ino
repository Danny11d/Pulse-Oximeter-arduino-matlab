#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "Waveshare_LCD1602_RGB.h"

MAX30105 particleSensor;
Waveshare_LCD1602_RGB lcd(16, 2);

#define BUFFER_SIZE 50
uint16_t irBuffer[BUFFER_SIZE]; 
uint16_t redBuffer[BUFFER_SIZE]; 

const uint32_t FINGER_THRESHOLD = 50000;
const int READ_DELAY = 300;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.setRGB(0, 0, 250);
  lcd.clear();

  if (!particleSensor.begin(Wire, I2C_SPEED_STANDARD, 0x57)) {
    lcd.setCursor(0, 0);
    lcd.send_string("Erro Sensor");
    while (1);
  }

  particleSensor.setup(40, 4, 2, 800, 411, 4096);
  // particleSensor.setPulseAmplitudeGreen(0);
}

void loop() {
  static bool reading = false;
  uint32_t irValue = particleSensor.getIR();

  if (!reading) {
    if (irValue > FINGER_THRESHOLD) {
      reading = true;
      lcd.setCursor(0, 0);
      lcd.send_string("Lendo...       ");
      lcd.setCursor(0, 1);
      lcd.send_string("               ");
      delay(500);
      takeMeasurement();
      reading = false;
    } else {
      lcd.setCursor(0, 0);
      lcd.send_string("Coloque dedo   ");
      lcd.setCursor(0, 1);
      lcd.send_string("               ");
    }
  }

  delay(READ_DELAY);
}

void takeMeasurement() {
  for (int i = 0; i < BUFFER_SIZE; i++) {
    while (!particleSensor.available()) particleSensor.check();
    irBuffer[i] = particleSensor.getFIFOIR();
    redBuffer[i] = particleSensor.getFIFORed();
    particleSensor.nextSample();
    delay(10);
  }

  int32_t spo2, heartRate;
  int8_t validSPO2, validHeartRate;

  maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_SIZE, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

  // Buffer compartilhado para otimizar memória
  char displayBuffer[17]; 

  // Linha 1: BPM
  lcd.setCursor(0, 0);
  if (validHeartRate) {
    snprintf(displayBuffer, sizeof(displayBuffer), "BPM: %d       ", heartRate);
  } else {
    strncpy(displayBuffer, "BPM: ---       ", sizeof(displayBuffer));
  }
  lcd.send_string(displayBuffer);

  // Linha 2: SpO2
  lcd.setCursor(0, 1);
  if (validSPO2 && spo2 <= 100) {
    snprintf(displayBuffer, sizeof(displayBuffer), "SpO2: %d%%     ", spo2);
    Serial.print(heartRate);
    Serial.print(" ");
    Serial.println(spo2);
  } else {
    strncpy(displayBuffer, "SpO2: ---      ", sizeof(displayBuffer));
  }
  lcd.send_string(displayBuffer);
}
