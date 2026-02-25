#include <Arduino.h>

// ==== ПИНЫ ====
#define SDA_PIN 8
#define SCL_PIN 9
#define DHTPIN 4
#define DHTTYPE DHT11   // если DHT11 → поменяй
#define MHZ_RX 5
#define MHZ_TX 6
#define buzzerPin 7
#define butonPin 10
#define ledRedPin 3
#define ledGreenPin 2
#define ledBluePin 1

// ==== Wi-Fi ====
#define WIFIAP_PASSWORD "123456789"

#define buzzerTimeout 60000 // 1 минута

#define FW_VERSION "0.2B"

// ==== Shared globals ====
extern int co2ppm;
extern float temperature;
extern float humidity;
extern String deviceName;