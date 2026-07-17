#include "meteo.h"

WiFiClient client; 
HTTPClient http;
JsonDocument doc;
WeatherRecord Weather[3];

void MeteoInit(){
    NTP.begin(5);
    NTP.updateNow();
}

String getMeteo(){
    if (WiFi.status() == WL_CONNECTED) {
        String url = "http://api.open-meteo.com/v1/forecast?latitude=" + String(LATITUDE) +
                     "&longitude=" + String(LONGITUDE) +
                     "&hourly=temperature_2m,relative_humidity_2m,weather_code,wind_speed_10m,wind_direction_10m" +
                     "&wind_speed_unit=ms&timezone=auto&forecast_days=2";
        http.begin(client, url); 
        
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            String payload = http.getString();
            http.end();
            return payload;
        } else {
            Serial.print("HTTP Error code: ");
            Serial.println(httpResponseCode);
            
            http.end();
            return "Error on HTTP request: " + String(httpResponseCode);
        }
    } else {
        return "WiFi not connected";
    }
} 

String weather_codeParsing(int code){
switch (code) {
        case 0: 
            return "Ясно";
        case 1: case 2: 
            return "Облачно с прояснениями";
        case 3: case 45: case 48: 
            return "Облачно";
        case 51: case 61: case 80: 
            return "Слабый дождь";
        case 53: case 63: case 81: 
            return "Умеренный дождь"; 
        case 55: case 65: case 82: 
            return "Сильный дождь"; 
        case 71: case 73: case 75: 
            return "Снегопад";
        case 95: case 96: case 99:
            return "Гроза";
        default: 
            return "Неизвестно";
    }
}

void MeteoParsing(){
    NTP.tick();
    String meteoData = getMeteo();
    DeserializationError error = deserializeJson(doc, meteoData);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
    Serial.println(NTP.dateToString());
    JsonObject hourly = doc["hourly"];
    size_t data_count = hourly["time"].size();
    for (size_t i = 0; i < data_count; i++) {
        String time_str = hourly["time"][i].as<String>();
        int temp = hourly["temperature_2m"][i].as<int>();
        int humidity = hourly["relative_humidity_2m"][i].as<int>();
        int wind_speed = hourly["wind_speed_10m"][i].as<int>();
        int wind_deg = hourly["wind_direction_10m"][i].as<int>();
        int w_code = hourly["weather_code"][i].as<int>();
        Weather[i/24].time_str[i%24]=time_str;
        Weather[i/24].temp[i%24]=temp;
        Weather[i/24].humidity[i%24]=humidity;
        Weather[i/24].wind_speed[i%24]=wind_speed;
        Weather[i/24].wind_deg[i%24]=wind_deg;
        Weather[i/24].w_code[i%24]=w_code;
        Weather[i/24].w_code_str[i%24]=weather_codeParsing(w_code);

        /*
            Serial.print("====");
            Serial.print(time_str);
            Serial.print("====");
            Serial.print(i/24);
            Serial.print("    ");
            Serial.println(i/7);

            Serial.print("Temp: ");
            Serial.print(temp);
            Serial.print("°C, Humidity: ");
            Serial.print(humidity);
            Serial.print("%, Wind Speed: ");
            Serial.print(wind_speed);
            Serial.print(" m/s, Wind Direction: ");
            Serial.print(wind_deg);
            Serial.print("°, Weather Code: ");
            Serial.print(w_code);
            Serial.print(", Weather: ");
            Serial.println(weather_codeParsing(w_code));
            */
    }
}

void getWeatherForecast(int count){
    MeteoParsing();
    int startHour = NTP.hour();
    for(int i = startHour; i < startHour + count; i++){
        int day = i / 24;
        int hour = i % 24;
        Serial.print(Weather[day].time_str[hour]);
        Serial.print(" | Temp: ");
        Serial.print(Weather[day].temp[hour]);
        Serial.print("C, Humidity: ");
        Serial.print(Weather[day].humidity[hour]);
        Serial.print("%, Wind: ");
        Serial.print(Weather[day].wind_speed[hour]);
        Serial.print(" m/s, Dir: ");
        Serial.print(Weather[day].wind_deg[hour]);
        Serial.print("deg, ");
        Serial.println(Weather[day].w_code_str[hour]);
    }
}