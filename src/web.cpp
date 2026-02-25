#include "web.h"

Preferences prefs;
WebServer server(80);

String ssid;
String pass;

const char* wifiPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>WiFi Setup</title>
</head>
<body>
<h2>WiFi Setup</h2>
<form action="/save" method="POST">
SSID:<br>
<input type="text" name="ssid"><br><br>
Password:<br>
<input type="password" name="pass"><br><br>
<input type="submit" value="Save">
</form>
</body>
</html>
)rawliteral";

String deviceName;

void generateDeviceName() {

    uint8_t mac[6];
    WiFi.macAddress(mac);

    char suffix[5];
    sprintf(suffix, "%02X%02X", mac[4], mac[5]);

    deviceName = "AirCore-" + String(suffix);

    Serial.println("Device name: " + deviceName);
}

void startMDNS() {

    if (MDNS.begin(deviceName.c_str())) {
        Serial.println("mDNS started");
        Serial.println(deviceName + ".local");
    }
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

    Serial.print("Connecting");

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected!");
        Serial.println(WiFi.localIP());
        return true;
    }

    Serial.println("\nFailed.");
    return false;
}


// ===== Режим настройки (БЛОКИРУЮЩИЙ) =====
void startConfigPortal() {

    analogWrite(ledRedPin, 0);
    analogWrite(ledGreenPin, 0);    
    analogWrite(ledBluePin, 255);
    WiFi.mode(WIFI_AP);
    WiFi.softAP(deviceName, WIFIAP_PASSWORD);

    Serial.println("AP started");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", wifiPage);
    });

    server.on("/save", HTTP_POST, []() {
        String newSSID = server.arg("ssid");
        String newPASS = server.arg("pass");

        saveConfig(newSSID, newPASS);

        server.send(200, "text/html", "Saved! Rebooting...");
        delay(2000);
        ESP.restart();
    });

    server.begin();

    // Блокируем выполнение здесь
    while (true) {
        server.handleClient();
        delay(10);
    }
}


// ===== Инициализация =====
void initWiFi() {
    generateDeviceName();
    loadConfig();
    if(digitalRead(butonPint) == LOW){
        Serial.println("Button pressed, starting config portal...");
        startConfigPortal();
        return;
    }

    if (ssid.length() > 0) {
        if (connectWiFi()) {
            startMDNS();
            analogWrite(ledRedPin, 0);
            analogWrite(ledGreenPin, 255);  
            analogWrite(ledBluePin, 0);
            delay(500);
            return; // всё ок
        }
    }
    startConfigPortal();
}