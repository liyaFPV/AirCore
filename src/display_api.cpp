#include "display_api.h"
GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display(GxEPD2_290_T94_V2(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN));

U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit;

void elink_init() {
  // hardware SPI с кастомными пинами
  #define ENABLE_GxEPD2_GFX 0
  SPI.begin(CLK_PIN, -1, DIN_PIN, CS_PIN); // SCK, MISO(-1), MOSI, CS
  display.init(0);
  display.setRotation(1);
  display.fillScreen(GxEPD_WHITE);
  u8g2_for_adafruit.begin(display);
  u8g2_for_adafruit.setFontMode(1);
  u8g2_for_adafruit.setFont(u8g2_font_unifont_t_cyrillic);
  u8g2_for_adafruit.setForegroundColor(GxEPD_BLACK);
  u8g2_for_adafruit.setBackgroundColor(GxEPD_WHITE);
}

void elink_setCursor(int16_t x, int16_t y) {
  int16_t offset = u8g2_for_adafruit.getFontAscent();
  u8g2_for_adafruit.setCursor(x, y + offset);
}

void elink_print(const String text) {
  u8g2_for_adafruit.print(text);
}

void elink_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color=BLACK) {
  display.drawLine(x0, y0, x1, y1, color);
}

void elink_update() {
  display.display(true);
}

void elink_updateWindow(int16_t x, int16_t y, int16_t width, int16_t height) {
  display.displayWindow(x, y, width, height);
}

void elink_fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color=BLACK) {
  display.fillRect(x, y, w, h, color);
}

void elink_drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color=BLACK) {
  display.drawRect(x, y, w, h, color);
}

void elink_clear() {
  display.fillScreen(GxEPD_WHITE);
}

void elink_sleep(){
  //print bitmap
  display.hibernate();
}

void elink_wakeUp(){
  elink_init();
}