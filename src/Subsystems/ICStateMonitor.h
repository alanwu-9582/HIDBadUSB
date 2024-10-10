#pragma once

#include <esp_system.h>
#include <esp_sleep.h>
#include <esp_log.h>

class ICStateMonitor {
private:
    float temperature;
    uint32_t freeHeap;
    int64_t uptime;

public:
    ICStateMonitor();

    void updateState();
    float getTemperature();
    uint32_t getFreeHeap();
    int64_t getUptime();

};