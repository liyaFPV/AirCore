#include "settings.h"
#include <Preferences.h>

Settings cfg = {};

static Preferences prefs;
static const char* NS = "aircore";

void settings_load() {
    prefs.begin(NS, true);
    cfg.valid = prefs.getBool("valid", false);
    String ssid = prefs.getString("ssid", "");
    String pass = prefs.getString("pass", "");
    String city = prefs.getString("city", "");
    strncpy(cfg.ssid, ssid.c_str(), sizeof(cfg.ssid) - 1);
    cfg.ssid[sizeof(cfg.ssid) - 1] = 0;
    strncpy(cfg.pass, pass.c_str(), sizeof(cfg.pass) - 1);
    cfg.pass[sizeof(cfg.pass) - 1] = 0;
    strncpy(cfg.city, city.c_str(), sizeof(cfg.city) - 1);
    cfg.city[sizeof(cfg.city) - 1] = 0;
    cfg.loc_mode = prefs.getUChar("loc_mode", LOC_AUTO);
    cfg.lat = prefs.getDouble("lat", 55.7558);
    cfg.lon = prefs.getDouble("lon", 37.6173);
    cfg.tz_min = prefs.getShort("tz_min", 300);
    prefs.end();
}

void settings_save() {
    prefs.begin(NS, false);
    prefs.putBool("valid", cfg.valid);
    prefs.putString("ssid", cfg.ssid);
    prefs.putString("pass", cfg.pass);
    prefs.putUChar("loc_mode", cfg.loc_mode);
    prefs.putDouble("lat", cfg.lat);
    prefs.putDouble("lon", cfg.lon);
    prefs.putShort("tz_min", cfg.tz_min);
    prefs.putString("city", cfg.city);
    prefs.end();
}

void settings_reset() {
    prefs.begin(NS, false);
    prefs.clear();
    prefs.end();
    memset(&cfg, 0, sizeof(cfg));
    cfg.loc_mode = LOC_AUTO;
    cfg.lat = 55.7558;
    cfg.lon = 37.6173;
    cfg.tz_min = 300;
    strncpy(cfg.city, "Москва", sizeof(cfg.city) - 1);
    cfg.valid = false;
}

int tzToGmt(int16_t tz_min) {
    if (tz_min % 60 == 0) return tz_min / 60;
    return tz_min;
}
