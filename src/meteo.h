#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <GyverNTP.h>

struct WeatherRecord {
    String time_str[24];
    int temp[24];
    int humidity[24];
    int wind_speed[24];
    int wind_deg[24];
    int w_code[24];
    String w_code_str[24];
};

void MeteoInit();
String getMeteo();
void MeteoParsing();
void getWeatherForecast(int count);