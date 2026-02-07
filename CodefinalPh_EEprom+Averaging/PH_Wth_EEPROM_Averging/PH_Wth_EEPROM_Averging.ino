#include <EEPROM.h>

#define PH_PIN 34

// Alamat EEPROM
#define ADDR_M 0
#define ADDR_C 4

float m = -6.289;   // default awal (kalibrasi kamu)
float c = 15.187;

// Fungsi averaging ADC
int readADC() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {
    sum += analogRead(PH_PIN);
    delay(10);
  }
  return sum / 10;
}

void saveCalibration(float newM, float newC) {
  EEPROM.put(ADDR_M, newM);
  EEPROM.put(ADDR_C, newC);
  EEPROM.commit();
  Serial.println(">>> Kalibrasi DISIMPAN ke EEPROM");
}

void loadCalibration() {
  EEPROM.get(ADDR_M, m);
  EEPROM.get(ADDR_C, c);

  // Kalau EEPROM kosong / rusak, pakai default
  if (isnan(m) || isnan(c)) {
    m = -6.289;
    c = 15.307;
    saveCalibration(m, c);
    Serial.println(">>> EEPROM kosong, pakai kalibrasi default");
  }

  Serial.println("=== KALIBRASI DARI EEPROM ===");
  Serial.print("m = "); Serial.println(m, 6);
  Serial.print("c = "); Serial.println(c, 6);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(32);          // wajib di ESP32

  loadCalibration();

  Serial.println("=== SENSOR pH ESP32 (EEPROM + AVERAGING) ===");
  Serial.println("Perintah Serial (opsional):");
  Serial.println("SETM=nilai   contoh: SETM=-6.300");
  Serial.println("SETC=nilai   contoh: SETC=15.250");
  Serial.println("SAVE         simpan ke EEPROM");
}

void loop() {
  int adc = readADC();
  float voltage = adc * (3.3 / 4095.0);
  float pH = m * voltage + c;

  Serial.print("ADC(avg): ");
  Serial.print(adc);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);

  // ---- BACA PERINTAH DARI SERIAL (OPSIONAL) ----
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.startsWith("SETM=")) {
      m = cmd.substring(5).toFloat();
      Serial.print("m diubah jadi: ");
      Serial.println(m, 6);
    }
    else if (cmd.startsWith("SETC=")) {
      c = cmd.substring(5).toFloat();
      Serial.print("c diubah jadi: ");
      Serial.println(c, 6);
    }
    else if (cmd == "SAVE") {
      saveCalibration(m, c);
    }
  }

  delay(1000);
}
