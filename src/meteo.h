#include <Arduino.h>
#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "ArduinoJson.h"
#include <GyverNTP.h>

struct WeatherRecord {
    String time_str[23];
    int temp[23];
    int humidity[23];
    int wind_speed[23];
    int wind_deg[23];
    int w_code[23];
    String w_code_str[23];
};

void MeteoInit();
String getMeteo();
void MeteoParsing();