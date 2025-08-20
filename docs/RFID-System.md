# RFID System Documentation

## Alur Kerja Sistem RFID yang Benar

### 1. **Permasalahan Awal**
- RFID card memiliki UID unik dari pabrik (contoh: `04A1B2C3D5`)
- Database menggunakan RFID code manual (`RFID001`, `RFID002`)
- Arduino mengirim UID asli yang tidak cocok dengan database

### 2. **Solusi yang Diimplementasikan**

#### **A. Proses Registrasi RFID (Sekali Setup)**

1. **Upload `rfid_registration.ino` ke Arduino**
2. **Buka Serial Monitor**
3. **Tempelkan kartu RFID** → Akan muncul UID asli (contoh: `04A1B2C3D5`)
4. **Registrasi via API atau Web Interface**:
   ```bash
   POST /api/rfid/register
   {
     "rfid_code": "04A1B2C3D5",
     "user_id": 1
   }
   ```

#### **B. Proses Absensi (Operasional Harian)**

1. **Upload `absen_aslab.ino` ke Arduino**
2. **Tempelkan kartu RFID** → Arduino baca UID
3. **Arduino kirim ke API**:
   ```bash
   POST /api/rfid/scan
   {
     "rfid_code": "04A1B2C3D5"
   }
   ```
4. **Laravel cari user** berdasarkan UID tersebut
5. **Proses check-in/check-out**

### 3. **API Endpoints**

#### **Registrasi RFID**
- `POST /api/rfid/scan-for-registration` - Cek UID tersedia untuk registrasi
- `POST /api/rfid/register` - Daftarkan UID ke user
- `GET /api/rfid/users` - List user untuk registrasi

#### **Absensi**
- `POST /api/rfid/scan` - Proses absensi
- `GET /api/rfid/status` - Cek status absensi

### 4. **Arduino Sketches**

#### **Struktur Folder yang Benar**
```
public/
├── absen_aslab/
│   └── absen_aslab.ino          # Sketch untuk absensi harian
└── rfid_registration/
    └── rfid_registration.ino    # Sketch untuk registrasi RFID
```

⚠️ **PENTING**: Setiap sketch harus di folder terpisah karena Arduino IDE menggabungkan semua file `.ino` dalam satu folder.

#### **Cara Compile**
1. **Buka Arduino IDE**
2. **File → Open** 
3. **Pilih salah satu**: 
   - `absen_aslab/absen_aslab.ino` untuk operasional harian
   - `rfid_registration/rfid_registration.ino` untuk registrasi
4. **Upload ke ESP32**

#### **Format UID RFID**
```cpp
// Format UID dengan leading zero
String uid = "";
for (byte i = 0; i < mfrc522.uid.size; i++) {
  if (mfrc522.uid.uidByte[i] < 0x10) {
    uid += "0";  // Tambahkan leading zero
  }
  uid += String(mfrc522.uid.uidByte[i], HEX);
}
uid.toUpperCase();
```

#### **Contoh UID yang Benar**
- ✅ `04A1B2C3` (8 karakter, dengan leading zero)
- ❌ `4A1B2C3` (7 karakter, tanpa leading zero)

### 5. **Testing**

#### **Untuk Testing dengan Data Sample**
Jalankan seeder yang menyediakan sample UID:
```bash
php artisan db:seed --class=AslabWithRfidSeeder
```

#### **Untuk Testing dengan Kartu Asli**
1. Gunakan `rfid_registration.ino` untuk dapat UID
2. Registrasi manual via API atau buat web interface

### 6. **Web Interface untuk Registrasi (Opsional)**

Bisa dibuat halaman admin untuk:
- List semua user yang belum punya RFID
- Scan kartu dan assign ke user
- Manage RFID registrations

### 7. **Implementasi Next Steps**

1. ✅ **API Controller sudah dibuat**
2. ✅ **Arduino scripts sudah diperbaiki**  
3. ✅ **Seeder sudah diupdate**
4. 🔄 **Testing dengan kartu asli**
5. 🔄 **Buat web interface registrasi (opsional)**

### 8. **Cara Testing**

1. **Reset database dan run seeder**:
   ```bash
   php artisan migrate:fresh --seed
   ```

2. **Test dengan sample data** (Alif & Aditya punya RFID):
   ```bash
   # Test scan untuk Alif
   curl -X POST http://localhost:8000/api/rfid/scan \
     -H "Content-Type: application/json" \
     -d '{"rfid_code":"04A1B2C3"}'
   ```

3. **Test registrasi untuk user lain**:
   ```bash
   # Daftarkan UID ke Bintang (user_id: 3)
   curl -X POST http://localhost:8000/api/rfid/register \
     -H "Content-Type: application/json" \
     -d '{"rfid_code":"04F1F2F3","user_id":3}'
   ```

## Kesimpulan

Sistem sekarang menggunakan **UID asli** dari RFID card, bukan kode manual. Ini lebih secure dan praktis karena setiap kartu memiliki UID unik dari pabrik.
