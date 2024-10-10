#include <Arduino.h>
#include <esp_system.h>
#include <esp_sleep.h>
#include <esp_log.h>

#include "ICStateMonitor.h"

ICStateMonitor::ICStateMonitor() {
    temperature = 0;
    freeHeap = 0;
    uptime = 0;
}

void ICStateMonitor::updateState() {
    temperature = temperatureRead();
    freeHeap = esp_get_free_heap_size();
    uptime = esp_timer_get_time();
}


float ICStateMonitor::getTemperature() {
    return temperature;
}

uint32_t ICStateMonitor::getFreeHeap() {
    return freeHeap;
}

int64_t ICStateMonitor::getUptime() {
    return uptime;
}
