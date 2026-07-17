#include "display_api.h"
#include <GxEPD2_BW.h>
#include <Adafruit_GFX.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "config.h"
#include <Fonts/FreeSansBold24pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <GyverNTP.h>

void main_menu(int PPM,int HUM,int TEM);
void draw_weather_widget(int x, int y, int temp, int windSpeed, int windDeg, String weatherStr, int hour);