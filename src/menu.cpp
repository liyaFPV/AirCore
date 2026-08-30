#include "menu.h"
#include "meteo.h"
#include "weather_icons.h"

extern GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit;
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

  // ==== Виджет погоды (вертикальный блок) ====
  int wHour = NTP.hour();
  int wDay = wHour / 24;
  int wH = wHour % 24;
  draw_weather_widget(2, 65, Weather[wDay].temp, Weather[wDay].wind_speed, Weather[wDay].wind_deg, Weather[wDay].w_code_str, wH);

  elink_update(); 
}

// ==== Виджет погоды (bitmap иконки, компактно) ====

static const unsigned char* get_weather_bits(String weatherStr) {
  if (weatherStr == "Ясно")                   return sun_bits;
  if (weatherStr == "Облачно с прояснениями") return cloud_sun_bits;
  if (weatherStr == "Облачно")                return cloud_bits;
  if (weatherStr == "Слабый дождь")           return rain0_bits;
  if (weatherStr == "Умеренный дождь")        return rain1_bits;
  if (weatherStr == "Сильный дождь")          return rain2_bits;
  if (weatherStr == "Снегопад")               return snow_bits;
  if (weatherStr == "Гроза")                  return rain_lightning_bits;
  return cloud_bits;
}

void draw_weather_widget(int x, int y, int temp[], int windSpeed[], int windDeg[], String weatherStr[], int hour) {
  elink_drawRect(x,y,x+96,63,BLACK);
  elink_drawRect(x,y,x+96*2,63,BLACK);
  elink_drawRect(x,y,x+96*3,63,BLACK);

  elink_drawBitmap(x+5,y+5,get_weather_bits(weatherStr[hour]),32,32);

  elink_setCursor(x+40, y+20);
  elink_setFont(u8g2_font_logisoso20_tn);
  elink_print("-99");
  elink_setCursor(x+80, y-7);
  elink_setFont(u8g2_font_unifont_tf);
  elink_print("°C");
}