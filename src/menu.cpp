#include "menu.h"

extern DHT dht;
extern uButton button;
extern MHZ19 CO2;
int old_elink_clear_timer=-1;
#define elink_clear_timer 60000

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

void print_main_menu(){
    elink_fillRect(0,0,296,128,WHITE);
    draw_watch(0, 0, u8g2_font_logisoso50_tf);

    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    elink_setFont(u8g2_font_unifont_tf);
    elink_setCursor(150, 0);
    elink_print("Temp: " + String(isnan(temp) ? 0 : int(temp)) + "°C");
    elink_setCursor(150, 18);
    elink_print("Hum:  " + String(isnan(hum) ? 0 : int(hum)) + "%");
    elink_setCursor(150, 36);
    elink_print("CO2:  " + String(int(CO2.getCO2())) + "ppm");

    elink_drawLine(0, 54, 296, 54, BLACK);
    elink_drawLine(98, 54, 98, 127, BLACK);
    elink_drawLine(197, 54, 197, 127, BLACK);

    elink_update();
}

void menu_tick(){
    print_main_menu();
}   