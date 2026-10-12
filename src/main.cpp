#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <DHT.h>
#include <GyverNTP.h>
#include <MHZ19.h>
#include <uButton.h>

#include "config.h"
#include "display_api.h"
#include "wifi.h"
#include "menu.h"
#include "led.h"
#include "settings.h"
#include "web_portal.h"

DHT dht(DHTPIN, DHTTYPE);
HardwareSerial mhzSerial(1);
uButton button(buttonPin);
MHZ19 CO2;

double lat = 55.7558;
double lon = 37.6173;
int TIMEZONE = 5;

static bool portal_mode = false;

static int month_index(const char* m) {
    static const char* names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; i++) {
        if (strncmp(m, names[i], 3) == 0) return i;
    }
    return -1;
}

static uint32_t parse_http_date(const String& s) {
    int comma = s.indexOf(',');
    String rest = (comma >= 0) ? s.substring(comma + 1) : s;
    rest.trim();
    int day = 0, year = 0, hh = 0, mm = 0, ss = 0;
    char mon[4] = {0};
    if (sscanf(rest.c_str(), "%d %3s %d %d:%d:%d", &day, mon, &year, &hh, &mm, &ss) < 6) return 0;
    int mo = month_index(mon);
    if (mo < 0) return 0;
    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = mo;
    t.tm_mday = day;
    t.tm_hour = hh;
    t.tm_min = mm;
    t.tm_sec = ss;
    return (uint32_t)timegm(&t);
}

// Резервная синхронизация времени через заголовок Date обычного HTTP
// (на некоторых сетях исходящий UDP/123 закрыт, и NTP не отвечает).
static bool sync_time_http() {
    static const char* keys[] = {"Date"};
    HTTPClient http;
    http.collectHeaders(keys, 1);
    http.begin("http://api.open-meteo.com/v1/forecast?latitude=0&longitude=0&current=temperature_2m");
    int code = http.GET();
    bool ok = false;
    if (code == 200) {
        uint32_t unix = parse_http_date(http.header("Date"));
        if (unix > 1600000000UL) {
            NTP.sync(unix);
            ok = true;
#if defined(DEVBUILD)
            Serial.print("HTTP time: ");
            Serial.println(unix);
#endif
        }
    }
    http.end();
    return ok;
}

static void show(const String& l1, const String& l2 = "") {
    elink_clear();
    elink_setCursor(0, 0);
    elink_print(l1);
    if (l2.length()) {
        elink_setCursor(0, 18);
        elink_print(l2);
    }
    elink_update();
}

static void start_config_portal() {
    portal_mode = true;
    web_portal_start(true);
    String hn = wifi_hostname();
    elink_clear();
    elink_setCursor(0, 0);
    elink_print("AirCore v" FW_VERSION);
    elink_setCursor(0, 18);
    elink_print("Режим настройки");
    elink_setCursor(0, 40);
    elink_print("Wi-Fi: " + hn);
    elink_setCursor(0, 58);
    elink_print("http://192.168.4.1");
    elink_update();
}

void setup() {
    Serial.begin(115200);
    mhzSerial.begin(9600, SERIAL_8N1, MHZ_RX, MHZ_TX);

    dht.begin();
    CO2.begin(mhzSerial);
    setup_led();

    elink_init();
    show("AirCore v" FW_VERSION, "загрузка...");

    settings_load();

    // Долгое удержание кнопки при старте — сброс настроек и запуск портала
    pinMode(buttonPin, INPUT_PULLUP);
    if (digitalRead(buttonPin) == LOW) {
        uint32_t t0 = millis();
        while (digitalRead(buttonPin) == LOW && (millis() - t0) < 3000) delay(10);
        if ((millis() - t0) >= 2000) {
            settings_reset();
            show("Сброс настроек", "запуск настройки...");
            delay(800);
        }
    }

    bool need_portal = (!cfg.valid || strlen(cfg.ssid) == 0);
    if (!need_portal) {
        show("Wi-Fi: " + String(cfg.ssid), "подключение...");
        need_portal = !wifi_connect(cfg.ssid, cfg.pass, 15000);
    }

    if (need_portal) {
        start_config_portal();
        return;
    }

    if (cfg.loc_mode == LOC_AUTO) {
        show("Определение локации", "по IP...");
        int tz = getTimezone();
        if (getLocation()) {
            cfg.lat = lat;
            cfg.lon = lon;
            cfg.tz_min = tz;
            settings_save();
        }
    } else {
        lat = cfg.lat;
        lon = cfg.lon;
    }
    TIMEZONE = tzToGmt(cfg.tz_min);

    if (DEV_MODE) {
        Serial.println("DEV MODE ENABLED");
        Serial.print("Lat: ");
        Serial.println(lat);
        Serial.print("Lon: ");
        Serial.println(lon);
        Serial.print("Tz(min): ");
        Serial.println(cfg.tz_min);
    }

    NTP.onError([]() {
        Serial.println(NTP.readError());
        Serial.print("online: ");
        Serial.println(NTP.online());
    });
    NTP.begin(TIMEZONE);
    sync_time_http();

    web_portal_start(false);

    show("AirCore v" FW_VERSION, WiFi.localIP().toString());
    delay(1500);
    get_meteo();
}

static uint32_t last_draw = (uint32_t)-1000;

void loop() {
    if (portal_mode) {
        web_portal_handle();
        delay(2);
        return;
    }

    led_tick();
    web_portal_handle();
    NTP.tick();
    if (millis() - last_draw >= 1000) {
        last_draw = millis();
        menu_tick();
    }
}
