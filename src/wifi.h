#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

void wifi_init();
String get_wifi_ip();
bool getLocation();
int getTimezone();
bool get_meteo();