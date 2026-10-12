#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "config.h"

// Уникальное имя "AirCore-XXXX" на основе MAC
String wifi_hostname();

// Подключение к сети с таймаутом. true при успехе.
bool wifi_connect(const char* ssid, const char* pass, uint32_t timeout_ms);

// Открытая точка доступа с именем wifi_hostname()
void wifi_start_ap();

// Список сетей в виде JSON-массива [{ssid,rssi,secure}]
String wifi_scan_json();

// Автоопределение по IP (ipwho.is). getTimezone() возвращает смещение в минутах.
bool getLocation();
int getTimezone();

bool get_meteo();
