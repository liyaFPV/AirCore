#include "graf.h"

extern GxEPD2_BW<GxEPD2_290_T94_V2, GxEPD2_290_T94_V2::HEIGHT> display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit;

SensorData sensor_data_hour;
SensorData sensor_data_day;

int timer_hour = -1;
int timer_day = -1;

#define max_CO2 5000
#define max_HUM 80
#define max_TEM 50

#define min_CO2 400
#define min_HUM 20
#define min_TEM 0

void write_data(int CO2, int HUM, int TEM){
    if (millis() - timer_hour >= graf_hour_update_time) {
        sensor_data_hour.CO2[sensor_data_hour.index] = CO2;
        sensor_data_hour.HUM[sensor_data_hour.index] = HUM;
        sensor_data_hour.TEM[sensor_data_hour.index] = TEM;
    }
    if (millis() - timer_day >= graf_day_update_time) {
        sensor_data_day.CO2[sensor_data_day.index] = CO2;
        sensor_data_day.HUM[sensor_data_day.index] = HUM;
        sensor_data_day.TEM[sensor_data_day.index] = TEM;
    }
}

void fillRandomData(SensorData& data){
    for (int i = 0; i < 200; i++) {
        data.CO2[i] = random(400, 2001);
        data.HUM[i] = random(20, 81);
        data.TEM[i] = random(0, 51);
    }

    data.index = 0;
}

void draw_graf(int indata[200], int min, int max){
    for(int i; i<=200; i++){
        int data=map(indata[i],min,max,0,128);
        elink_drawLine(i,0,i,data);
    }
}

void test(){
    fillRandomData(sensor_data_hour);
    draw_graf(sensor_data_hour.CO2,min_CO2,max_CO2);
}