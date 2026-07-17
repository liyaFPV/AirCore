#include "menu.h"
#include "meteo.h"

void draw_watch(int x,int y,const uint8_t *font){
  String time;
  if(NTP.minute()<10 and NTP.hour()<10){
    time=String("0")+String(NTP.hour())+":"+String("0")+String(NTP.minute());
  }else if(NTP.minute()<10 and NTP.hour()>=10){
    time=String(NTP.hour())+":"+String("0")+String(NTP.minute());
  }else if(NTP.minute()>=10 and NTP.hour()<10){
    time=String("0")+String(NTP.hour())+":"+String(NTP.minute());
  }else{
    time=String(NTP.hour())+":"+String(NTP.minute());
  }
  elink_setFont(font); 
  elink_setCursor(x, y);
  elink_print(time);
}

void draw_co2(int x,int y, int PPM,const uint8_t *font){
  String TPPM;
  if(PPM<800){
    TPPM="CO2 "+String(PPM)+"PPM :)";
  }else if(PPM>800 and PPM<2000){
    TPPM="CO2 "+String(PPM)+"PPM :/";
  }else if(PPM>2000){
    TPPM="CO2 "+String(PPM)+"PPM :(";
  }
  elink_setFont(font);
  elink_setCursor(x, y);
  elink_print(TPPM);
}

void draw_HUM(int x,int y, int HUM,const uint8_t *font){
  String THUM;
  if(HUM<40 or HUM>70){
    THUM="HUM "+String(HUM)+"% :(";
  }else if(HUM>40 and HUM<60){
    THUM="HUM "+String(HUM)+"% :)";
  }else if(HUM>60 and HUM<70){
    THUM="HUM "+String(HUM)+"% :/";
  }
  elink_setFont(font);
  elink_setCursor(x, y);
  elink_print(THUM);
}

void draw_TEM(int x,int y, int TEM,const uint8_t *font){
  String TTEM;
  TTEM="TEM "+String(TEM)+"°C";
  elink_setFont(font);
  elink_setCursor(x, y);
  elink_print(TTEM);
}

// ==== Pixel art иконки погоды (32x32) ====

void draw_sun(int x, int y) {
  elink_fillRect(x+10, y+8, 12, 12, BLACK);
  elink_drawLine(x+1, y+14, x+8, y+14, BLACK);
  elink_drawLine(x+23, y+14, x+30, y+14, BLACK);
  elink_drawLine(x+16, y+1, x+16, y+6, BLACK);
  elink_drawLine(x+16, y+25, x+16, y+30, BLACK);
  elink_drawLine(x+4, y+4, x+9, y+9, BLACK);
  elink_drawLine(x+22, y+9, x+27, y+4, BLACK);
  elink_drawLine(x+9, y+22, x+4, y+27, BLACK);
  elink_drawLine(x+22, y+22, x+27, y+27, BLACK);
  elink_fillRect(x+4, y+5, 2, 2, BLACK);
  elink_fillRect(x+16, y+2, 2, 2, BLACK);
  elink_fillRect(x+7, y+15, 2, 2, BLACK);
  elink_fillRect(x+9, y+17, 2, 2, BLACK);
  elink_fillRect(x+13, y+11, 2, 2, BLACK);
  elink_fillRect(x+17, y+21, 2, 2, BLACK);
  elink_fillRect(x+23, y+14, 2, 2, BLACK);
}

void draw_cloud(int x, int y) {
  elink_fillRect(x+5, y+2, 22, 6, BLACK);
  elink_fillRect(x+12, y-2, 8, 4, BLACK);
  elink_fillRect(x+2, y+8, 28, 8, BLACK);
  elink_fillRect(x+4, y+16, 24, 6, BLACK);
  elink_fillRect(x+8, y+22, 16, 4, BLACK);
}

void draw_rain(int x, int y, int count) {
  draw_cloud(x, y+2);
  int drops = count;
  if(drops >= 1) elink_drawLine(x+7, y+28, x+7, y+31, BLACK);
  if(drops >= 2) elink_drawLine(x+14, y+27, x+14, y+30, BLACK);
  if(drops >= 3) elink_drawLine(x+21, y+28, x+21, y+31, BLACK);
  if(drops >= 4) elink_drawLine(x+10, y+30, x+10, y+33, BLACK);
  if(drops >= 5) elink_drawLine(x+18, y+30, x+18, y+33, BLACK);
  if(drops >= 6) elink_drawLine(x+24, y+28, x+24, y+31, BLACK);
}

void draw_snow(int x, int y) {
  draw_cloud(x, y+2);
  elink_fillRect(x+7, y+26, 3, 3, BLACK);
  elink_fillRect(x+15, y+28, 3, 3, BLACK);
  elink_fillRect(x+20, y+25, 3, 3, BLACK);
  elink_fillRect(x+10, y+30, 3, 3, BLACK);
  elink_fillRect(x+23, y+30, 3, 3, BLACK);
}

void draw_storm(int x, int y) {
  draw_cloud(x, y+2);
  elink_drawLine(x+16, y+27, x+12, y+30, BLACK);
  elink_drawLine(x+12, y+30, x+18, y+30, BLACK);
  elink_drawLine(x+18, y+30, x+14, y+33, BLACK);
}

void draw_icon(int x, int y, String weatherStr) {
  if (weatherStr == "Ясно") {
    draw_sun(x, y);
  } else if (weatherStr == "Облачно с прояснениями") {
    draw_cloud(x, y);
  } else if (weatherStr == "Облачно") {
    draw_cloud(x, y);
  } else if (weatherStr == "Слабый дождь") {
    draw_rain(x, y, 2);
  } else if (weatherStr == "Умеренный дождь") {
    draw_rain(x, y, 4);
  } else if (weatherStr == "Сильный дождь") {
    draw_rain(x, y, 6);
  } else if (weatherStr == "Снегопад") {
    draw_snow(x, y);
  } else if (weatherStr == "Гроза") {
    draw_storm(x, y);
  } else {
    draw_cloud(x, y);
  }
}

void draw_wind_arrow(int x, int y, int deg) {
  int dx, dy, dx2, dy2;
  if (deg < 23 || deg >= 338)      { dx = 0; dy = -10; dx2 = 0; dy2 = -10; }  // N
  else if (deg < 68)   { dx = 7; dy = -7; dx2 = 7; dy2 = -7; }  // NE
  else if (deg < 113)  { dx = 10; dy = 0; dx2 = 10; dy2 = 0; }  // E
  else if (deg < 158)  { dx = 7; dy = 7; dx2 = 7; dy2 = 7; }    // SE
  else if (deg < 203)  { dx = 0; dy = 10; dx2 = 0; dy2 = 10; }  // S
  else if (deg < 248)  { dx = -7; dy = 7; dx2 = -7; dy2 = 7; }  // SW
  else if (deg < 293)  { dx = -10; dy = 0; dx2 = -10; dy2 = 0; }// W
  else                 { dx = -7; dy = -7; dx2 = -7; dy2 = -7; }// NW

  elink_drawLine(x, y, x + dx, y + dy, BLACK);
  if (dx2 == dx && dy2 == dy) {
    elink_drawLine(x + dx, y + dy, x + dx - dy/3, y + dy + dx/3, BLACK);
    elink_drawLine(x + dx, y + dy, x + dx + dy/3, y + dy - dx/3, BLACK);
  }
}

void draw_weather_widget(int x, int y, int temp, int windSpeed, int windDeg, String weatherStr, int hour) {
  draw_icon(x, y, weatherStr);
  String tempStr = String(temp) + "°C";
  elink_setFont(u8g2_font_logisoso22_tf);
  elink_setCursor(x + 36, y + 24);
  elink_print(tempStr);
  String windStr = String(windSpeed) + "m/s";
  elink_setFont(u8g2_font_unifont_tf);
  elink_setCursor(x + 150, y + 12);
  elink_print(windStr);
  draw_wind_arrow(x + 200, y + 8, windDeg);
  String timeStr;
  if (hour < 10) timeStr = "0" + String(hour) + ":00";
  else timeStr = String(hour) + ":00";
  elink_setCursor(x + 230, y + 12);
  elink_print(timeStr);
}

void draw_DATE(int x,int y, const uint8_t *font){
  elink_setFont(font);
  elink_setCursor(x, y);
  elink_print(String(NTP.dateToString()));
}

void main_menu(int PPM,int HUM,int TEM){
  NTP.tick();
  elink_clear(); 

  draw_watch(-2,0,u8g2_font_logisoso62_tn);//отрисовка цасы

  elink_setFont(u8g2_font_unifont_tf);
  const int16_t x_pos = 180;
  const int16_t y_offset=17;
  int16_t y_pos = 0;

  draw_co2(x_pos, y_pos, PPM,u8g2_font_unifont_tf);//отресовка Co2
  y_pos=y_pos+y_offset;

  draw_HUM(x_pos, y_pos, HUM, u8g2_font_unifont_tf);//отресока HUM
  y_pos=y_pos+y_offset;

  draw_TEM(x_pos, y_pos, TEM, u8g2_font_unifont_tf);//отресока TEM
  y_pos=y_pos+y_offset;

  draw_DATE(x_pos, y_pos, u8g2_font_unifont_tf);//отресока TEM

  elink_update(); 
}
