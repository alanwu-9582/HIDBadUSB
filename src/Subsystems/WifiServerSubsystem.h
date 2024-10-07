#pragma once

#include <WiFi.h>
#include <ESPAsyncWebServer.h>

class WifiServerSubsystem {
private:
    AsyncWebServer server;
    const char* ssid;
    const char* password;

public:
    WifiServerSubsystem(const char* ssid, const char* password);
    String initWifi();
    void begin();
    void registeRouteProcessor(const char* route, std::function<void(AsyncWebServerRequest*)> handler);
};