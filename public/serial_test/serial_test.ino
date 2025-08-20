void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("=== ESP32 SERIAL TEST ===");
}

void loop() {
  Serial.println("Hello from ESP32! " + String(millis()));
  delay(2000);
}
