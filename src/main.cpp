#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include "config.h"
#include "meteo.h"
#include "web.h"
#include <uButton.h>
#include "display_api.h"
#include "menu.h"
#include "graf.h"
#include "debug.h"
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);
uButton button(butonPin);

int page = 0;
#define maxpage 1
int co2ppm = 0;
float temperature = 0;
float humidity = 0;

unsigned long buzzerTimer = 0;
unsigned long lastSensorRead = 0;
static unsigned long lastDisplayUpdate = 0;

enum Co2State {
    CO2_STATE_UNKNOWN,
    CO2_STATE_OK,
    CO2_STATE_WARMUP,
    CO2_STATE_ERROR
};

Co2State co2State = CO2_STATE_UNKNOWN;

const char* co2StateName(Co2State state) {
    switch (state) {
        case CO2_STATE_OK: return "WORKING";
        case CO2_STATE_WARMUP: return "WARMUP";
        case CO2_STATE_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// ==== Чтение MH-Z19C ====
void readMHZ19() {
    static byte cmd[] = {0xFF,0x01,0x86,0,0,0,0,0,0x79};
    byte response[9];

    while (mhzSerial.available()) {
        mhzSerial.read();
    }

    mhzSerial.write(cmd, 9);
    delay(10);

    unsigned long start = millis();
    while (mhzSerial.available() < 9 && millis() - start < 100) {
        delay(1);
    }

    if (mhzSerial.available() >= 9) {
        mhzSerial.readBytes(response, 9);
        if (response[0] == 0xFF && response[1] == 0x86) {
            byte checksum = 0;
            for (int i = 1; i < 8; i++) {
                checksum += response[i];
            }
            checksum = 0xFF - checksum + 1;

            if (checksum == response[8]) {
                int value = (response[2] << 8) + response[3];
                if (value > 0) {
                    co2ppm = value;
                    co2State = CO2_STATE_OK;
                    if (value == 5000) {
                        Serial.println("MHZ reports 5000 ppm (max range or sensor error)");
                    }
                } else {
                    co2ppm = 0;
                    co2State = CO2_STATE_WARMUP;
                }
                return;
            }
            Serial.println("MHZ checksum failed");
        } else {
            Serial.println("MHZ wrong header");
        }
    } else {
        Serial.println("MHZ no response");
    }

    co2ppm = 0;
    co2State = CO2_STATE_ERROR;
}

void setup() {
    Serial.begin(115200);

    #ifdef DEV_BUILD
        delay(500);
        printDebugInfo();
    #endif

    dht.begin();
    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);
    pinMode(buzzerPin, OUTPUT);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    elink_init();
    elink_clear();
    elink_setCursor(0, 0);
    elink_print("Подключение к Wi-Fi...");
    elink_update();
    initWiFi();
    MeteoInit();
    Serial.println("");
    getWeatherForecast(gmtOffset);
    Serial.println("");
}

void loop() {
    if(button.tick()){
        if (button.click()){ 
            page++;
        }
        lastDisplayUpdate=-1;
    }

    if(page<0) page=maxpage;
    if(page>maxpage) page=0;

    if (millis() - lastSensorRead >= sensor_update_time) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        readMHZ19();
        lastSensorRead = millis();
        write_data(int(co2ppm), int(humidity), int(temperature));
    }

    if (millis() - lastDisplayUpdate >= display_update_time) {
        switch (page){
        case 0:
            lastDisplayUpdate = millis();
            main_menu(int(co2ppm), int(humidity), int(temperature));
            break;
        case 1:
            test();
        }
    }

}