#include "menu.h"

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

  elink_update(); 
}
