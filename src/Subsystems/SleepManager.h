#pragma once

#include <Arduino.h>
#include "ICStateMonitor.h"

class SleepManager {
private:
    ICStateMonitor icStateMonitor;
    void enterDeepSleep();
    static void temperatureMonitorTask(void*);

public:
    SleepManager();
    void begin();
};