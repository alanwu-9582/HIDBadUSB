#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class FirmwareUpdater {
private:
    bool shouldReboot;

public:
    FirmwareUpdater();
    void updateHandler(AsyncWebServerRequest*);
    void updateFirmware(AsyncWebServerRequest*, String, size_t, uint8_t*, size_t, bool);
    void execute();
};