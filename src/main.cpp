#include <Arduino.h>
// #include <ArduinoOTA.h>

#include "password.h"
#include "Subsystems/USBHIDController.h"
#include "Subsystems/WifiServerSubsystem.h"
#include "Commands/WifiServerCmd.h"

USBHIDController Controller;
WifiServerSubsystem WifiServer(ssid, password);
WifiServerCmd ServerCmd(WifiServer, Controller);

void setup() {
	Serial.begin(115200);
	Serial.println("ESP32 Web Server: Initializing...");
	Serial.print("IP Address: ");
	Serial.println(WifiServer.initWifi());

	ServerCmd.begin();
}

void loop() {
	ServerCmd.execute();
}