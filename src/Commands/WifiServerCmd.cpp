#include <string>
#include <vector>

#include <Update.h>

#include "utils/StringUtils.h"
#include "Subsystems/USBHIDController.h"
#include "Subsystems/ICStateMonitor.h"
#include "Subsystems/FirmwareUpdater.h"
#include "WifiServerCmd.h"
#include "Webpages/index.h"

using std::string;
using std::vector;

const char* COMMAND_INPUT_PARAM = "commandInput";
const char* MOUSE_CONTROL_PARAM = "mouseControl";

const string commandEnd = ";;";
const string actionEnd = "--";
const string multiKeyPrefix = "$";
const string mousePrefix = "@";
const string mediaPrefix = "MDI/";
const string systemPrefix = "SYS/";

ICStateMonitor icStateMonitor;
FirmwareUpdater firmwareUpdater;

WifiServerCmd::WifiServerCmd(WifiServerSubsystem& wifiServer, USBHIDController& controller)
: wifiServer(wifiServer), controller(controller) {}

void WifiServerCmd::startController() {
    controller.begin();
}

void WifiServerCmd::activateCommand(char* command) {
    string commandStr = string(command);
    vector<string> commandParts = splitString(commandStr, commandEnd);

    for (string action : commandParts) {
		if (action.rfind(multiKeyPrefix, 0) == 0) {
            action.erase(0, 1);
			vector<string> keys = splitString(action, actionEnd);
            controller.executeMultiKeySequence(keys);

        } else if (action.rfind(mousePrefix, 0) == 0) {
            action.erase(0, 1);
            vector<string> mouseActions = splitString(action, actionEnd);
            controller.processMouseActions(mouseActions);

        } else if (action.rfind(mediaPrefix, 0) == 0) {
            action.erase(0, 4);
            vector<string> mediaActions = splitString(action, actionEnd);
            controller.processMediaActions(mediaActions);

        } else if (action.rfind(systemPrefix, 0) == 0) {
            action.erase(0, 4);
            vector<string> systemActions = splitString(action, actionEnd);
            controller.processSystemActions(systemActions);

        } else {
            controller.sendKeyboardText(action);
        }
    }
}

void WifiServerCmd::setupServer() {
    wifiServer.setGetRouteHandler("/", [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", webpage);
    });

    wifiServer.setGetRouteHandler("/command", [this](AsyncWebServerRequest* request) {
        String inputMessage;
        String inputParam;

        if (request->hasParam(COMMAND_INPUT_PARAM)) {
            inputMessage = request->getParam(COMMAND_INPUT_PARAM)->value();
            inputParam = COMMAND_INPUT_PARAM;

            activateCommand((char*)inputMessage.c_str());
            request->send(200, "text/plain", "Sended! "+inputMessage);

        } else {
            request->send(200, "text/plain", "[ERROR] No message sent");
        }
    });

    wifiServer.setGetRouteHandler("/icstate", [this](AsyncWebServerRequest* request) {
        icStateMonitor.updateState();

        String response = String(icStateMonitor.getTemperature()) + "," +
                          String(icStateMonitor.getFreeHeap()) + "," +
                          String(icStateMonitor.getUptime());

        request->send(200, "text/plain", response);
    });

    wifiServer.setPostRouteHandler("/updateFirmware", 
        [this](AsyncWebServerRequest* request) {
            firmwareUpdater.updateHandler(request);
        }, 
        [this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
            firmwareUpdater.updateFirmware(request, filename, index, data, len, final);
        }
    );


    wifiServer.begin();
}

void WifiServerCmd::begin() {
    wifiServer.initWifi();
    setupServer();
    startController();
}

void WifiServerCmd::execute() {
    firmwareUpdater.execute();
}