#include "display_api.h"
#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "config.h"
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <GyverNTP.h>

struct SensorData {
    int CO2[200];
    int HUM[200];
    int TEM[200];
    int index=0;
};

void write_data(int CO2, int HUM, int TEM);
void test();