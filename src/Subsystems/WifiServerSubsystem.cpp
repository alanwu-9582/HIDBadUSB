#include <WiFi.h>
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "Subsystems/WifiServerSubsystem.h"

WifiServerSubsystem::WifiServerSubsystem(const char* ssid, const char* password) 
    : ssid(ssid), password(password), server(80) {};

String WifiServerSubsystem::initWifi() {
    WiFi.softAP(ssid, password);
    return WiFi.softAPIP().toString();
}

void WifiServerSubsystem::begin() {
    server.begin();
}

void WifiServerSubsystem::registeRouteProcessor(const char* route, std::function<void(AsyncWebServerRequest*)> handler) {
    server.on(route, HTTP_GET, handler);
}