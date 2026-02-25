#include "oled.h"

GyverOLED<SSD1306_128x64, OLED_NO_BUFFER> oled;
uButton button(butonPin);
int screen = 0;

void oled_init() {
  oled.init(SDA_PIN, SCL_PIN);
  oled.clear();
  oled.setCursor(0, 0);
  oled.setContrast(255);
  oled.setScale(2);
  oled.print("FW: " + String(FW_VERSION));
  oled.update();
  delay(2000);
  oled.clear();
}
void oled_tikc(){
    if(button.press()){
        screen++;
        if(screen > 1) screen = 0;
        oled.clear();
    }
    switch(screen){
        case 0:
            oled.clear();
            oled.setCursor(0, 0);
            oled.setScale(2);
            oled.print("CO2: " + String(co2ppm) + " ppm");
            oled.update();
            break;
        case 1:
            oled.clear();
            oled.setCursor(0, 0);
            oled.setScale(2);
            oled.print("T: " + String(temperature) + " C");
            oled.setCursor(0, 1);
            oled.print("H: " + String(humidity) + " %");
            oled.update();
            break;
    }
}