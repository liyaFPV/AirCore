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
#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)
#define MeteoURL "https://api.open-meteo.com/v1/forecast?latitude=" STRINGIFY(LATITUDE) "&longitude=" STRINGIFY(LONGITUDE) "&hourly=temperature_2m,relative_humidity_2m,weather_code,wind_speed_10m,wind_direction_10m&wind_speed_unit=ms&timezone=auto&forecast_days=7"

// ==== FW_VERSION ====
#define FW_VERSION "1.1B"

#define buzzerTimeout 30000