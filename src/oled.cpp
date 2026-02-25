#include "oled.h"

// Используем буферный режим — рисуем в памяти и обновляем экран одним вызовом.
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
uButton button(butonPin);
int screen = 0;

void oled_init() {
  oled.init(SDA_PIN, SCL_PIN);
  oled.setContrast(255);
  oled.setScale(2);
  oled.clear();
  oled.setCursor(0, 0);
  oled.print("FW: ");
  oled.print(FW_VERSION);
  oled.update();
  delay(800);
}

void oled_tikc(){
    static unsigned long lastUpdate = 0;

    if(button.press()){
        screen++;
        if(screen > 1) screen = 0;
        lastUpdate = 0;  // принудить обновление
    }

    // Обновляем экран 1 раз в 500мс
    if(millis() - lastUpdate < 500) return;
    lastUpdate = millis();

    oled.clear();
    oled.setScale(2);

    switch(screen){
        case 0:
            oled.setScale(1);
            oled.setCursor(0, 0);
            oled.print("CO2:");
            oled.setCursor(24, 0);
            oled.setScale(2);
            oled.print(co2ppm);
            oled.print(" ppm");
            oled.setCursor(0, 2);
            oled.print("T: ");
            oled.print((int)temperature);
            oled.print("C");
            oled.setCursor(0, 4);
            oled.print("H: ");
            oled.print((int)humidity);
            oled.print("%");
            break;
        case 1:
            oled.setScale(1);
            oled.setCursor(0, 0);
            oled.print("IP:"+WiFi.localIP().toString());
            oled.setCursor(0, 2);
            oled.print(deviceName);
            oled.setCursor(0, 4);
            oled.print("SSID:"+WiFi.SSID());
            oled.setCursor(0, 6);
            oled.print("RSSI: "+String(WiFi.RSSI())+" dBm");
            break;
    }
    oled.update();
}