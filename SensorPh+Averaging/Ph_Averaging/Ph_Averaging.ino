#define PH_PIN 34

// Konstanta hasil kalibrasi SENSOR KAMU
float m = -6.289;
float c = 15.307;

// Fungsi averaging ADC (filter noise)
int readADC() {
  long sum = 0;
  for (int i = 0; i < 10; i++) {     // ambil 10 sampel
    sum += analogRead(PH_PIN);
    delay(10);                      // jeda kecil antar sampel
  }
  return sum / 10;                  // rata-rata
}

void setup() {
  Serial.begin(115200);
  Serial.println("=== SENSOR pH ESP32 (KALIBRASI + AVERAGING) ===");
}

void loop() {
  int adc = readADC();                                // pakai averaging
  float voltage = adc * (3.3 / 4095.0);

  float pH = m * voltage + c;

  Serial.print("ADC(avg): ");
  Serial.print(adc);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);

  delay(1000);
}
