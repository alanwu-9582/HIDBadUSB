#include <Arduino.h>

#include "password.h"
#include "Subsystems/USBHIDController.h"
#include "Subsystems/WifiServerSubsystem.h"
#include "Commands/WifiServerCmd.h"
#include "Subsystems/SleepManager.h"

USBHIDController Controller;
WifiServerSubsystem WifiServer(ssid, password);
WifiServerCmd ServerCmd(WifiServer, Controller);
SleepManager sleepManager;

void setup() {
	Serial.begin(115200);
	Serial.println("ESP32 Web Server: Initializing...");
	Serial.print("IP Address: ");
	Serial.println(WifiServer.initWifi());

	ServerCmd.begin();
	sleepManager.begin();
}

void loop() {
	ServerCmd.execute();
}