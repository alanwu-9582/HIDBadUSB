#include <WiFi.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "Subsystems/WifiServerSubsystem.h"

WifiServerSubsystem::WifiServerSubsystem(const char* ssid, const char* password) 
    : ssid(ssid), password(password), server(80) {
        uploadedData = UploadedData();

};

String WifiServerSubsystem::initWifi() {
    WiFi.softAP(ssid, password);
    return WiFi.softAPIP().toString();
}

void WifiServerSubsystem::begin() {
    server.begin();
}

void WifiServerSubsystem::setGetRouteHandler(const char* route, std::function<void(AsyncWebServerRequest*)> handler) {
    server.on(route, HTTP_GET, handler);
}

void WifiServerSubsystem::setPostRouteHandler(
    const char* route, 
    std::function<void(AsyncWebServerRequest*)> handler, 
    std::function<void(AsyncWebServerRequest*, String, size_t, uint8_t*, size_t, bool)> onUpload = nullptr) 
{
    server.on(route, HTTP_POST, handler, onUpload);
}