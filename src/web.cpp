#include "web.h"

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
#define WIFI_CONNECT_TIMEOUT 50
#define WIFI_CONNECT_RETRIES 3
#define WIFI_RETRY_DELAY_MS  2000

bool connectWiFi() {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    for (int attempt = 0; attempt < WIFI_CONNECT_RETRIES; attempt++) {
        WiFi.begin(ssid.c_str(), pass.c_str());

        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < WIFI_CONNECT_TIMEOUT) {
            delay(300);
            attempts++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }

        WiFi.disconnect(true);
        if (attempt < WIFI_CONNECT_RETRIES - 1) {
            delay(WIFI_RETRY_DELAY_MS);
        }
    }
    return false;
}

<<<<<<< HEAD

// ===== Режим настройки (с таймаутом) =====
#define PORTAL_TIMEOUT_MS 120000

=======
>>>>>>> 98e5e70 (fix багов)
void startConfigPortal() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(deviceName.c_str(), WIFIAP_PASSWORD);

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", wifiPage);
    });

    server.on("/save", HTTP_POST, []() {
        String newSSID = server.arg("ssid");
        String newPASS = server.arg("pass");
        if (newSSID.length() == 0) {
            server.send(400, "text/html", "SSID is empty");
            return;
        }
        saveConfig(newSSID, newPASS);
        server.send(200, "text/html", "OK");
        delay(1000);
        ESP.restart();
    });

    server.begin();
    elink_clear();
    elink_setCursor(0, 0);
    elink_print("Создана точка доступа для настройки");

    elink_setCursor(0, 15);
    String text="Имя:"+String(deviceName.c_str())+" Пароль:"+WIFIAP_PASSWORD;
    elink_print(text);

    elink_setCursor(0, 30);
    elink_print("IP для настройки:192.168.4.1");

    elink_update();

    unsigned long portalStart = millis();
    while (millis() - portalStart < PORTAL_TIMEOUT_MS) {
        server.handleClient();
        delay(5);
    }

    if (ssid.length() > 0 && connectWiFi()) {
        startMDNS();
        return;
    }

    ESP.restart();
}


// ===== Инициализация =====
void initWiFi() {
    generateDeviceName();
    loadConfig();
    if(connectWiFi()){
        return;
    }
    startConfigPortal();
}

// ===== Переподключение в loop() =====
#define RECONNECT_CHECK_MS  30000
#define RECONNECT_TIMEOUT_MS 30000

void checkWiFiReconnect() {
    static unsigned long lastCheck = 0;
    static bool wasConnected = false;

    if (millis() - lastCheck < RECONNECT_CHECK_MS) return;
    lastCheck = millis();

    if (WiFi.status() == WL_CONNECTED) {
        wasConnected = true;
        return;
    }

    if (!wasConnected) return;

    wasConnected = false;
    Serial.println("WiFi lost, reconnecting...");

    WiFi.disconnect(true);
    WiFi.begin(ssid.c_str(), pass.c_str());

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < RECONNECT_TIMEOUT_MS) {
        delay(300);
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi reconnected");
        startMDNS();
    } else {
        Serial.println("WiFi reconnect failed, rebooting...");
        ESP.restart();
    }
}