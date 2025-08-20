# Arduino Sketches untuk Sistem Absensi RFID

## Struktur Folder

Setiap sketch Arduino harus berada di folder terpisah dengan nama yang sama dengan file `.ino`:

```
public/
├── absen_aslab/
│   └── absen_aslab.ino          # Sketch untuk absensi harian
└── rfid_registration/
    └── rfid_registration.ino    # Sketch untuk registrasi RFID
```

## Cara Penggunaan

### 1. Registrasi RFID (Setup Awal)

1. **Buka Arduino IDE**
2. **File → Open → pilih `rfid_registration/rfid_registration.ino`**
3. **Upload ke ESP32**
4. **Buka Serial Monitor (Ctrl+Shift+M)**
5. **Tempelkan kartu RFID** → Akan muncul UID asli
6. **Catat UID yang muncul** untuk registrasi ke sistem

### 2. Absensi Harian (Operasional)

1. **Buka Arduino IDE**
2. **File → Open → pilih `absen_aslab/absen_aslab.ino`**
3. **Upload ke ESP32**
4. **Tempelkan kartu RFID** → Sistem akan proses check-in/check-out

## Konfigurasi

Pastikan konfigurasi WiFi dan server IP sama di kedua sketch:

```cpp
const char* ssid = "Asisten Laboratorium";
const char* password = "2025Labkomp:3";
String serverName = "http://192.168.10.6:8000/api/rfid/...";
```

## Pin Configuration ESP32

```cpp
#define RST_PIN 22  // Reset pin
#define SS_PIN 21   // SDA/SS pin
```

## Library Dependencies

- WiFi (ESP32 built-in)
- HTTPClient (ESP32 built-in)
- SPI (Arduino built-in)
- MFRC522 (Install via Library Manager)

## Troubleshooting

- **Compilation Error "redefinition"**: Pastikan hanya satu sketch yang dibuka di Arduino IDE
- **WiFi Connection Failed**: Periksa SSID dan password WiFi
- **HTTP Error**: Pastikan Laravel server berjalan dan IP address benar
