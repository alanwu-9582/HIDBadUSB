#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <Update.h>

#include "FirmwareUpdater.h"

FirmwareUpdater::FirmwareUpdater() {
    shouldReboot = false;
}

void FirmwareUpdater::updateHandler(AsyncWebServerRequest* request) {
    shouldReboot = !Update.hasError(); 
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", shouldReboot ? "SUCCESS" : "FAIL");
    response->addHeader("Connection", "close");
    request->send(response);
}

void FirmwareUpdater::updateFirmware(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if(!index){
        Serial.printf("Update Start: %s\n", filename.c_str());
        if(!Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000)){
            Update.printError(Serial);
        }
    }
    
    if(!Update.hasError()){
        if(Update.write(data, len) != len){
            Update.printError(Serial);
        }
    }

    if(final){
        if(Update.end(true)){ Serial.printf("Update Success: %uB\n", index+len); } 
        else { Update.printError(Serial); }
    }
}

void FirmwareUpdater::execute() {
    if(shouldReboot){
        Serial.println("Rebooting...");
        delay(100);
        ESP.restart();
    }
}