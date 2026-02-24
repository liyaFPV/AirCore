#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// ==== ПИНЫ ====
#define SDA_PIN 8
#define SCL_PIN 9
#define DHTPIN 4
#define DHTTYPE DHT11   // если DHT11 → поменяй
#define MHZ_RX 5
#define MHZ_TX 6
#define buzzerPin 7
#define butonPint 0
#define ledRedPin 10
#define ledGreenPin 20
#define ledBluePin 21

// ==== OLED ====
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);

int co2ppm = 0;
float temperature = 0;
float humidity = 0;

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

    Wire.begin(SDA_PIN, SCL_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("OLED не найден");
        while (true);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    dht.begin();

    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);

    display.setCursor(0,0);
    display.println("Meteo Station");
    display.display();
    delay(2000);
}

void loop() {
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    readMHZ19();

    display.clearDisplay();
    display.setCursor(0,0);

    display.println("=== Meteo ===");

    display.print("Temp: ");
    display.print(temperature);
    display.println(" C");

    display.print("Hum:  ");
    display.print(humidity);
    display.println(" %");

    display.print("CO2:  ");
    display.print(co2ppm);
    display.println(" ppm");

    display.display();

    delay(3000);
}