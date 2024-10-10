#include <esp_sleep.h>

#include "SleepManager.h"
#include "ICStateMonitor.h"

const int SLEEP_TIME = 10;
const int HIGH_TEMP_THRESHOLD = 60;

SleepManager::SleepManager() {
    icStateMonitor = ICStateMonitor();
}

void SleepManager::begin() {
    xTaskCreate(
        temperatureMonitorTask,
        "Temperature Monitor Task",
        2048,
        this,
        1,
        NULL
    );
}

void SleepManager::enterDeepSleep() {
    Serial.println("Entering deep sleep due to high temperature...");
    esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000); // 10 seconds
    esp_deep_sleep_start();
}

void SleepManager::temperatureMonitorTask(void* pvParameters) {
    SleepManager* sleepManager = (SleepManager*) pvParameters;
    while (true) {
        sleepManager->icStateMonitor.updateState();
        float temp = sleepManager->icStateMonitor.getTemperature();
        if (temp > HIGH_TEMP_THRESHOLD) {
            sleepManager->enterDeepSleep();
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS); // 10 seconds
    }
}