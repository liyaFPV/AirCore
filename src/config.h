#include <Arduino.h>

// ==== ПИНЫ ====
#define CS_PIN    11
#define DC_PIN    10
#define RST_PIN   9
#define BUSY_PIN  8
#define CLK_PIN   12  // SCK
#define DIN_PIN   13  // MOSI

#define DHTPIN 4
#define DHTTYPE DHT11
#define MHZ_RX 5
#define MHZ_TX 6
#define buzzerPin 7
#define butonPin 10
#define ledRedPin 3
#define ledGreenPin 2
#define ledBluePin 1

// ==== Wi-Fi ====
#define WIFIAP_PASSWORD "123456789"

#define buzzerTimeout 30000

#define FW_VERSION "0.3B"

// ==== Shared globals ====
extern int co2ppm;
extern float temperature;
extern float humidity;
extern String deviceName;

// ==== LED settings ====
// Глобальная яркость (0-255). Меняйте в одном месте.
#define LED_BRIGHTNESS 128
extern uint8_t ledBrightness;
void led_set(uint8_t r, uint8_t g, uint8_t b);
void led_init();