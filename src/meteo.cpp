#include "meteo.h"

String getMeteo(){
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(MeteoURL);
        int httpResponseCode = http.GET();
        if (httpResponseCode > 0) {
            String payload = http.getString();
            http.end();
            return payload.c_str();
        } else {
            http.end();
            return "Error on HTTP request";
        }
    }else {
        return "WiFi not connected";
    }
}