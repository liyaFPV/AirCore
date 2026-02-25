#include <Arduino.h>
#include <Wire.h>
#include <DHT.h>
#include "config.h"
#include "web.h"
#include "oled.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);

int co2ppm = 0;
float temperature = 0;
float humidity = 0;
unsigned long buzzerTimer = 0;
unsigned long badCo2Start = 0;
bool co2Bad = false;

// ==== Чтение MH-Z19C ====
void readMHZ19() {
    byte cmd[9] = {0xFF,0x01,0x86,0,0,0,0,0,0x79};
    byte response[9];

    mhzSerial.write(cmd, 9);
    delay(10);

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
    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);
    analogWrite(ledRedPin, 255);
    analogWrite(ledGreenPin, 255);    
    analogWrite(ledBluePin, 255);
    tone(buzzerPin, 125, 500);
    delay(2000);
    Serial.println("START");
    initWiFi();
}

void loop() {
    button.tick();
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    readMHZ19();

    if(co2ppm < 800){
        analogWrite(ledRedPin, 0);
        analogWrite(ledGreenPin, 255);    
        analogWrite(ledBluePin, 0);
        noTone(buzzerPin);
    }else if(co2ppm < 1200 and co2ppm >= 800){
        analogWrite(ledRedPin, 255);
        analogWrite(ledGreenPin, 255);    
        analogWrite(ledBluePin, 0);
        noTone(buzzerPin);
    }else if (co2ppm >= 1200) {

    if (!co2Bad) {
        badCo2Start = millis();
        co2Bad = true;
    }

    if (millis() - badCo2Start >= 300000) { // 5 минут
        if (millis() - buzzerTimer >= 60000) {
            tone(buzzerPin, 1000, 1000);
            buzzerTimer = millis();
        }
    }
    } else {
        co2Bad = false;
    }
    

    Serial.println();
    Serial.println("=== Meteo ===");
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.println(" C");
    Serial.print("Hum:  ");
    Serial.print(humidity);
    Serial.println(" %");
    Serial.print("CO2:  ");
    Serial.print(co2ppm);
    Serial.println(" ppm");
    Serial.println();
    Serial.println("================");
    delay(1000);
    oled_tikc();
}