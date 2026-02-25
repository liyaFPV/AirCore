#include "web.h"
#include "led.h"

Preferences prefs;
WebServer server(80);

String deviceName;

String ssid;
String pass;

const char* wifiPage = R"rawliteral(<html><head><meta charset=UTF-8><title>WiFi</title><style>body{font-family:Arial}</style></head><body><h2>WiFi Setup</h2><form action=/save method=POST>SSID:<br><input name=ssid><br><br>Pass:<br><input type=password name=pass><br><br><input type=submit value=Save></form></body></html>)rawliteral";

void generateDeviceName() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char suffix[16];
    sprintf(suffix, "AirCore-%02X%02X", mac[4], mac[5]);
    deviceName = suffix;
}

void startMDNS() {
    MDNS.begin(deviceName.c_str());
}

// ===== Загрузка =====
void loadConfig() {
    prefs.begin("wifi", true);
    ssid = prefs.getString("ssid", "");
    pass = prefs.getString("pass", "");
    prefs.end();
}


// ===== Сохранение =====
void saveConfig(String newSSID, String newPASS) {
    prefs.begin("wifi", false);
    prefs.putString("ssid", newSSID);
    prefs.putString("pass", newPASS);
    prefs.end();
}


// ===== Подключение =====
bool connectWiFi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), pass.c_str());

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(300);
        attempts++;
    }
    return (WiFi.status() == WL_CONNECTED);
}


// ===== Режим настройки (БЛОКИРУЮЩИЙ) =====
void startConfigPortal() {
    led_set(0, 0, 255);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(deviceName.c_str(), WIFIAP_PASSWORD);

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", wifiPage);
    });

    server.on("/save", HTTP_POST, []() {
        saveConfig(server.arg("ssid"), server.arg("pass"));
        server.send(200, "text/html", "OK");
        delay(1000);
        ESP.restart();
    });

    server.begin();
    while (true) {
        server.handleClient();
        delay(5);
    }
}


// ===== Инициализация =====
void initWiFi() {
    generateDeviceName();
    loadConfig();
    if(button.press()){
        startConfigPortal();
        return;
    }

    if (ssid.length() > 0 && connectWiFi()) {
        startMDNS();
        led_set(0, 255, 0);
        return;
    }
    startConfigPortal();
}