#include "web_portal.h"
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>

#include "settings.h"
#include "wifi.h"
#include "config.h"
#include "web_page.h"

static WebServer server(80);
static DNSServer dns;
static bool ap_active = false;
static bool started = false;

bool web_portal_ap_active() {
    return ap_active;
}

static void handle_root() {
    String page = FPSTR(PAGE_INDEX);
    page.replace("__FWVER__", FW_VERSION);
    server.send(200, "text/html", page);
}

static void handle_scan() {
    server.send(200, "application/json", wifi_scan_json());
}

static void handle_location() {
    JsonDocument doc;
    if (cfg.valid) {
        doc["valid"] = true;
        doc["ssid"] = cfg.ssid;
        doc["lat"] = cfg.lat;
        doc["lon"] = cfg.lon;
        doc["tz_min"] = cfg.tz_min;
        doc["loc_mode"] = (cfg.loc_mode == LOC_AUTO) ? "auto" : "city";
        doc["city"] = cfg.city;
    } else {
        doc["valid"] = false;
    }
    String out;
    serializeJson(doc, out);
    server.send(200, "application/json", out);
}

static void handle_save() {
    String ssid = server.arg("ssid");
    String pass = server.arg("pass");
    String mode = server.arg("loc_mode");
    String lat = server.arg("lat");
    String lon = server.arg("lon");
    String tz_min = server.arg("tz_min");
    String city = server.arg("city");

    if (ssid.length() == 0) {
        server.send(400, "application/json", "{\"error\":\"ssid\"}");
        return;
    }

    strncpy(cfg.ssid, ssid.c_str(), sizeof(cfg.ssid) - 1);
    cfg.ssid[sizeof(cfg.ssid) - 1] = 0;
    strncpy(cfg.pass, pass.c_str(), sizeof(cfg.pass) - 1);
    cfg.pass[sizeof(cfg.pass) - 1] = 0;
    strncpy(cfg.city, city.c_str(), sizeof(cfg.city) - 1);
    cfg.city[sizeof(cfg.city) - 1] = 0;
    cfg.loc_mode = (mode == "city") ? LOC_MANUAL : LOC_AUTO;
    if (lat.length()) cfg.lat = lat.toDouble();
    if (lon.length()) cfg.lon = lon.toDouble();
    if (tz_min.length()) cfg.tz_min = (int16_t)tz_min.toInt();
    cfg.valid = true;
    settings_save();

    server.send(200, "application/json", "{\"ok\":true}");
    delay(500);
    ESP.restart();
}

static void handle_reset() {
    settings_reset();
    server.send(200, "application/json", "{\"ok\":true}");
    delay(500);
    ESP.restart();
}

static void handle_notfound() {
    server.sendHeader("Location", "/", true);
    server.send(302, "text/plain", "");
}

void web_portal_start(bool ap_mode) {
    if (started) return;
    started = true;

    if (ap_mode) {
        wifi_start_ap();
        dns.start(53, "*", WiFi.softAPIP());
        ap_active = true;
    }

    server.on("/", HTTP_GET, handle_root);
    server.on("/scan", HTTP_GET, handle_scan);
    server.on("/location", HTTP_GET, handle_location);
    server.on("/save", HTTP_POST, handle_save);
    server.on("/reset", HTTP_POST, handle_reset);
    server.onNotFound(handle_notfound);
    server.begin();

    Serial.println("Web portal started");
}

void web_portal_handle() {
    if (ap_active) dns.processNextRequest();
    server.handleClient();
}
