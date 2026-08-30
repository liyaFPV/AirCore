#include <Arduino.h>

// ==== ПИНЫ ====
#define CS_PIN    9
#define DC_PIN    8
#define RST_PIN   7
#define BUSY_PIN  6
#define CLK_PIN   10  // SCK
#define DIN_PIN   20  // MOSI

#define DHTPIN 1
#define DHTTYPE DHT11
#define MHZ_RX 4
#define MHZ_TX 5
#define buzzerPin 2
#define butonPin 0
#define led 3
// ==== Wi-Fi ====

#define WIFIAP_PASSWORD "123456789"

//===== meteo =====

#define LATITUDE 56.08
#define LONGITUDE 54.27
#define gmtOffset 5

//===== timers =====
#define main_update_time 5000
#define sensor_update_time 5000

#define graf_hour_update_time 18000
#define graf_day_update_time 432000

// ==== FW_VERSION ====
#define FW_VERSION "1.2B"

#define buzzerTimeout 30000