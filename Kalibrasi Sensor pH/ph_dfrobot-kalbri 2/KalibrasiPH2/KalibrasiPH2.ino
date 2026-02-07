#define PH_PIN 34

// Konstanta hasil kalibrasi SENSOR KAMU
float m = -6.289;
float c = 15.187;

void setup() {
  Serial.begin(115200);
  Serial.println("=== TEST SENSOR pH ESP32 (KALIBRASI MANUAL) ===");
}

void loop() {
  int adc = analogRead(PH_PIN);
  float voltage = adc * (3.3 / 4095.0);

  float pH = m * voltage + c;

  Serial.print("ADC: ");
  Serial.print(adc);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | pH: ");
  Serial.println(pH, 2);

  delay(1000);
}
