#pragma once

#include <string>
#include <vector>

#include "Subsystems/WifiServerSubsystem.h"
#include "Subsystems/USBHIDController.h"

using std::string;
using std::vector;

class WifiServerCmd {
private:
    WifiServerSubsystem& wifiServer;
    USBHIDController& controller;

    void activateCommand(char* command);
    void startController();
    void setupServer();

public:
    WifiServerCmd(WifiServerSubsystem& wifiServer, USBHIDController& controller);
    void begin();
};