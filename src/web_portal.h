#pragma once
#include <Arduino.h>

// Запуск веб-интерфейса. ap_mode=true — поднять точку доступа с captive portal.
void web_portal_start(bool ap_mode);

// Вызывать в loop()
void web_portal_handle();

bool web_portal_ap_active();
