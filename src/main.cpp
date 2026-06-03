#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include "config.h"
#include "meteo.h"
#include "web.h"
#include <uButton.h>
#include "display_api.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);
uButton button(butonPin);

int co2ppm = 0;
float temperature = 0;
float humidity = 0;
unsigned long buzzerTimer = 0;
unsigned long lastSensorRead = 0;

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
    MeteoParsing();
    Serial.println("");
}

void loop() {
    static unsigned long lastLedToggle = 0;
    if (millis() - lastLedToggle >= 500) {
        digitalWrite(led, !digitalRead(led));
        lastLedToggle = millis();

    }
    button.tick();
    if (millis() - lastSensorRead >= 1000) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        readMHZ19();
        lastSensorRead = millis();

        char line1[64];
        char line2[64];
        char line3[96];
        snprintf(line1, sizeof(line1), "Temp: %.1f C", temperature);
        snprintf(line2, sizeof(line2), "Humidity: %.1f %%", humidity);
        snprintf(line3, sizeof(line3), "CO2: %d ppm (%s)", co2ppm, co2StateName(co2State));

        elink_clear();
        elink_setCursor(0, 0);
        elink_print(String(line1));
        elink_setCursor(0, 16);
        elink_print(String(line2));
        elink_setCursor(0, 32);
        elink_print(String(line3));
        elink_update();
    }

}