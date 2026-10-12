#pragma once
#include <Arduino.h>

#if defined(DEVBUILD)
    #define DEV_MODE 1
#else
    #define DEV_MODE 0
#endif


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
#define buttonPin 0
#define LED_PIN_DATA 3

//===== LED =====
#define LED_COUNT 4

//===== METEO =====
extern double lat;
extern double lon;
extern int TIMEZONE;

// ==== FW_VERSION ====
#define FW_VERSION "2.0A"