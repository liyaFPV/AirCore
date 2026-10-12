#pragma once
#include <Arduino.h>

enum LocMode : uint8_t {
    LOC_AUTO = 0,
    LOC_MANUAL = 1,
};

struct Settings {
    char ssid[33];
    char pass[65];
    uint8_t loc_mode;
    double lat;
    double lon;
    int16_t tz_min;  // смещение UTC в минутах
    char city[40];
    bool valid;
};

extern Settings cfg;

void settings_load();
void settings_save();
void settings_reset();

// GyverNTP/Stamp принимает часы (-12..14) либо минуты. Вернём минуты, если
// смещение не кратно часу, иначе часы.
int tzToGmt(int16_t tz_min);
