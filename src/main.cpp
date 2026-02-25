#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include "config.h"
#include "web.h"
#include "oled.h"
#include "led.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);

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
    oled_init();
    dht.begin();
    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);
    pinMode(buzzerPin, OUTPUT);
    led_init();
    playStartupMelody();
    initWiFi();
}

void loop() {
    button.tick();
    
    // Ограничиваем частоту чтения датчиков до 2 сек
    if (millis() - lastSensorRead >= 2000) {
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        readMHZ19();
        lastSensorRead = millis();
        
        // Управление индикаторами и зуммером
        if(co2ppm < 800) {
            led_set(0, 255, 0);
            noTone(buzzerPin);
        } else if(co2ppm < 1200) {
            led_set(255, 255, 0);
            noTone(buzzerPin);
        } else {
            led_set(255, 0, 0);
            
            if (!co2Bad) {
                badCo2Start = millis();
                co2Bad = true;
                buzzerTimer = 0;
            }
            if (millis() - badCo2Start >= 300000 && millis() - buzzerTimer >= 60000) {
                tone(buzzerPin, 1000, 500);
                buzzerTimer = millis();
            }
        }
        if (co2ppm < 1200) co2Bad = false;
    }
    
    oled_tikc();
}