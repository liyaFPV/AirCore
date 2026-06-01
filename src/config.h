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

#define buzzerTimeout 30000

#define FW_VERSION "1.0B"