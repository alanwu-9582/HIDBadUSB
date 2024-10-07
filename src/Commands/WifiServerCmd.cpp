#include <string>
#include <vector>

#include "utils/StringUtils.h"
#include "Subsystems/USBHIDController.h"
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
    wifiServer.registeRouteProcessor("/", [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", webpage);
    });

    wifiServer.registeRouteProcessor("/command", [this](AsyncWebServerRequest* request) {
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

    wifiServer.begin();
}

void WifiServerCmd::begin() {
    wifiServer.initWifi();
    setupServer();
    startController();
}