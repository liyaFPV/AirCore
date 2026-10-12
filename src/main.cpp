#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include <GyverNTP.h>
#include <MHZ19.h>
#include <uButton.h>

#include "config.h"
#include "display_api.h"
#include "wifi.h"
#include "menu.h"
#include "led.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);
uButton button(buttonPin);
MHZ19 CO2;

double lat = 55.7558;
double lon = 37.6173;
int TIMEZONE = 5;

void setup() {
    Serial.begin(115200);
    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);

    dht.begin();
    CO2.begin(mhzSerial);
    setup_led();

    elink_init();
    elink_clear();
    elink_setCursor(0, 0);
    elink_print("AirCore v" FW_VERSION);
    elink_setCursor(0, 18);
    elink_print("подключение к Wi-Fi...");
    elink_update();

    wifi_init();
    TIMEZONE = getTimezone();
    getLocation();

    if(DEV_MODE){
        Serial.println("DEV MODE ENABLED");
        Serial.print("Lat: ");
        Serial.println(lat);
        Serial.print("Lon: ");
        Serial.println(lon);
        Serial.print("Timezone: ");
        Serial.println(TIMEZONE);
    }

    NTP.onError([]() {
        Serial.println(NTP.readError());
        Serial.print("online: ");
        Serial.println(NTP.online());
    });
    NTP.begin(TIMEZONE);
    elink_clear();
    elink_update();
    get_meteo();
}

void loop() {
    led_tick();
    if(NTP.tick()){
        menu_tick();
        delay(1000);
    }
}