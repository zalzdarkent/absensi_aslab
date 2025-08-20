#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

// WiFi
const char* ssid = "Asisten Laboratorium";  // ganti SSID WiFi kamu
const char* password = "2025Labkomp:3";      // ganti password WiFi kamu

// API Endpoint Laravel
String serverName = "http://192.168.10.6:8000/api/rfid/scan-for-registration";  // endpoint untuk registrasi

// RFID
#define RST_PIN 22
#define SS_PIN 21
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  delay(1000); // Wait for serial to initialize

  Serial.println("\n=== ESP32 RFID REGISTRATION STARTED ===");
  Serial.println("Initializing RFID reader...");

  SPI.begin();
  mfrc522.PCD_Init();

  // Test RFID reader
  if (mfrc522.PCD_PerformSelfTest()) {
    Serial.println("✓ RFID Reader OK");
  } else {
    Serial.println("✗ RFID Reader FAILED");
  }
  mfrc522.PCD_Init(); // Re-init after self test

  // Connect WiFi
  Serial.println("Connecting to WiFi...");
  Serial.println("SSID: " + String(ssid));

  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.println("IP Address: " + WiFi.localIP().toString());
    Serial.println("Signal Strength: " + String(WiFi.RSSI()) + " dBm");
  } else {
    Serial.println("\n✗ WiFi Connection FAILED!");
    Serial.println("Status: " + String(WiFi.status()));
  }

  Serial.println("\n=== RFID REGISTRATION MODE ===");
  Serial.println("Tempelkan kartu RFID untuk melihat UID");
  Serial.println("=====================================");
}

void loop() {
  // Status check setiap 10 detik
  static unsigned long lastStatusCheck = 0;
  if (millis() - lastStatusCheck > 10000) {
    Serial.println("[INFO] System running... Waiting for RFID card...");
    Serial.println("WiFi Status: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected"));
    lastStatusCheck = millis();
  }

  // Cek kartu RFID
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Ambil UID RFID
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uid += "0";  // Tambahkan leading zero untuk single digit hex
    }
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  Serial.println("\n=== KARTU TERDETEKSI ===");
  Serial.println("UID: " + uid);
  Serial.println("Length: " + String(uid.length()) + " characters");

  // Cek status registrasi
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Sending to server...");
    HTTPClient http;
    http.setTimeout(5000); // 5 second timeout
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Data JSON yang dikirim
    String jsonData = "{\"rfid_code\":\"" + uid + "\"}";
    Serial.println("JSON Data: " + jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Response Code: " + String(httpResponseCode));
      Serial.println("Response: " + response);

      if (httpResponseCode == 200) {
        Serial.println("✓ RFID tersedia untuk registrasi");
        Serial.println("Gunakan UID ini untuk registrasi: " + uid);
      } else {
        Serial.println("✗ RFID sudah terdaftar atau error");
      }
    } else {
      Serial.print("HTTP Error code: ");
      Serial.println(httpResponseCode);
      Serial.println("Possible causes: Server offline, wrong URL, network issue");
    }
    http.end();
  } else {
    Serial.println("✗ WiFi Disconnected - Reconnecting...");
    WiFi.reconnect();
  }

  Serial.println("=======================\n");

  // Halt PICC and stop encryption
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(3000);  // biar ga spam kalo nempel terus
}
