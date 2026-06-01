#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include "config.h"
#include "web.h"
#include <uButton.h>
#include "display_api.h"
#include "led.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);
uButton button(butonPin);

int co2ppm = 0;
float temperature = 0;
float humidity = 0;
unsigned long buzzerTimer = 0;
unsigned long badCo2Start = 0;
unsigned long lastSensorRead = 0;
bool co2Bad = false;

// ==== Приветственная мелодия ====
void playStartupMelody() {
    const int notes[] = {392, 494, 392, 349, 392};  // Sol, Si, Sol, Fa, Sol
    const int duration = 150;
    
    for(int i = 0; i < 5; i++) {
        tone(buzzerPin, notes[i], duration);
        delay(duration + 50);
    }
    noTone(buzzerPin);
}

// ==== Чтение MH-Z19C ====
void readMHZ19() {
    static byte cmd[] = {0xFF,0x01,0x86,0,0,0,0,0,0x79};
    byte response[9];

    mhzSerial.write(cmd, 9);
    delay(1);  // упущенная задержка

    int attempts = 0;
    while (mhzSerial.available() < 9 && attempts < 20) {
        delayMicroseconds(100);
        attempts++;
    }
    
    if (mhzSerial.available() >= 9) {
        mhzSerial.readBytes(response, 9);
        if (response[0] == 0xFF && response[1] == 0x86) {
            co2ppm = (response[2] << 8) + response[3];
        }
    }
}

void setup() {
    Serial.begin(115200);
    dht.begin();
    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);
    pinMode(buzzerPin, OUTPUT);
    led_init();
    playStartupMelody();
    initWiFi();
}

void loop() {
    button.tick();
    if (millis() - lastSensorRead >= 1000) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        readMHZ19();
        lastSensorRead = millis();
        printf("Temp: %.1f C, Humidity: %.1f %%, CO2: %d ppm\n", temperature, humidity, co2ppm);
    }

}