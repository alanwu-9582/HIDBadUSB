#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

struct UploadedData {
    String filename;
    size_t index;
    uint8_t* data;
    size_t len;
    bool final;
};

class WifiServerSubsystem {
private:
    AsyncWebServer server;
    const char* ssid;
    const char* password;

    UploadedData uploadedData;

public:
    WifiServerSubsystem(const char* ssid, const char* password);
    String initWifi();
    void begin();
    void setGetRouteHandler(const char* route, std::function<void(AsyncWebServerRequest*)> handler);
    void setPostRouteHandler(
        const char* route, 
        std::function<void(AsyncWebServerRequest*)> handler, 
        std::function<void(AsyncWebServerRequest*, String, size_t, uint8_t*, size_t, bool)> fileHandler
    );
};