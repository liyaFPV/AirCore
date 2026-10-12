#include "wifi.h"
#include <esp_mac.h>

String wifi_hostname() {
    uint8_t mac[6] = {0};
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    char hostname[16];
    snprintf(hostname, sizeof(hostname), "AirCore-%02X%02X", mac[4], mac[5]);
    return String(hostname);
}

bool wifi_connect(const char* ssid, const char* pass, uint32_t timeout_ms) {
    String hostname = wifi_hostname();
    WiFi.setHostname(hostname.c_str());
#if defined(DEVBUILD)
    Serial.print("Hostname: ");
    Serial.println(hostname);
#endif

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    Serial.print("Connecting to WiFi");
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && (millis() - start) < timeout_ms) {
        delay(250);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());
        return true;
    }
    WiFi.disconnect(true);
    return false;
}

void wifi_start_ap() {
    String hostname = wifi_hostname();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(hostname.c_str());
    Serial.print("AP: ");
    Serial.print(hostname);
    Serial.print("  http://");
    Serial.println(WiFi.softAPIP());
}

String wifi_scan_json() {
    int n = WiFi.scanNetworks();
    JsonDocument doc;
    JsonArray arr = doc.to<JsonArray>();
    for (int i = 0; i < n; i++) {
        JsonObject o = arr.add<JsonObject>();
        o["ssid"] = WiFi.SSID(i);
        o["rssi"] = WiFi.RSSI(i);
        o["secure"] = WiFi.encryptionType(i) != WIFI_AUTH_OPEN;
    }
    WiFi.scanDelete();
    String out;
    serializeJson(doc, out);
    return out;
}

bool getLocation() {
    HTTPClient http;
    http.begin("http://ipwho.is/");
    int code = http.GET();
    if (code == 200) {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, http.getString());
        if (!err && doc["success"] == true) {
            lat = doc["latitude"];
            lon = doc["longitude"];
            http.end();
            return true;
        }
    }
    http.end();
    return false;
}

int getTimezone() {
    HTTPClient http;
    http.begin("http://ipwho.is/");
    int code = http.GET();
    if (code == 200) {
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, http.getString());
        if (!err && doc["success"] == true) {
            int offset = doc["timezone"]["offset"];  // секунды
            http.end();
            return offset / 60;  // минуты
        }
    }
    http.end();
    return 180;
}

bool get_meteo() {
    String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(lat) + "&longitude=" + String(lon) + "&current=temperature_2m,wind_speed_10m&hourly=temperature_2m,relative_humidity_2m,wind_speed_10m";
    HTTPClient http;
    http.begin(url);
    int code = http.GET();
#if defined(DEVBUILD)
    Serial.print("Meteo HTTP code: ");
    Serial.println(code);
#endif
    if (code == 200) {
        String payload = http.getString();
        JsonDocument doc;
        DeserializationError err = deserializeJson(doc, payload);
        if (!err) {
            if (DEV_MODE) {
                Serial.println("Meteo Data:");
                serializeJson(doc, Serial);
                Serial.println();
            }
            http.end();
            return true;
        }
#if defined(DEVBUILD)
        Serial.print("Meteo JSON error: ");
        Serial.println(err.c_str());
        Serial.print("Payload length: ");
        Serial.println(payload.length());
#endif
    }
    http.end();
    return false;
}
