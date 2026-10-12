#include "wifi.h"

void wifi_init() {
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char hostname[16];
    snprintf(hostname, sizeof(hostname), "AirCore-%02X%02X", mac[4], mac[5]);
    WiFi.setHostname(hostname);
    #if defined(DEVBUILD)
        Serial.print("Hostname: ");
        Serial.println(hostname);
    #endif

    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    return;
}

String get_wifi_ip() {
    return WiFi.localIP().toString();
}

bool getLocation() {
    HTTPClient http;
    http.begin("http://ipwho.is/");
    int code = http.GET();
    if (code == 200) {
        JsonDocument doc;
        DeserializationError err =
            deserializeJson(doc, http.getString());
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
            int offset = doc["timezone"]["offset"];
            http.end();
            return offset / 3600;
        }
    }
    http.end();
    return 3;
}

bool get_meteo(){
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
        if(!err){
            if(DEV_MODE){
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