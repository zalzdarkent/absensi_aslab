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
  SPI.begin();
  mfrc522.PCD_Init();

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
  Serial.println("=== RFID REGISTRATION MODE ===");
  Serial.println("Tempelkan kartu RFID untuk melihat UID");
}

void loop() {
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

  // Cek status registrasi
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    // Data JSON yang dikirim
    String jsonData = "{\"rfid_code\":\"" + uid + "\"}";
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
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  Serial.println("=======================\n");
  delay(3000);  // biar ga spam kalo nempel terus
}
