#include <string>
#include <vector>

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"

#include "index.h"
#include "constants.h"
#include "KeysDictionary.h"
#include "MouseDictionary.h"

using std::string;
using std::vector;

const char* COMMAND_INPUT_PARAM = "commandInput";

const string commandEnd = ";;";
const string actionEnd = "--";
const string multiKeyPrefix = "$";
const string mousePrefix = "@";

AsyncWebServer server(80);
USBHIDKeyboard Keyboard;
USBHIDMouse Mouse;

void activateCommand(char* command);
vector<string> splitString(string str, string delimiter);

void setup() {
	Serial.begin(115200);

	WiFi.softAP(SSID, PASSWORD);

	Serial.println("WiFi AP Started");
	Serial.print("IP Address: ");
	Serial.println(WiFi.softAPIP());

	server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
		Serial.println("ESP32 Web Server: New request received:");  // for debugging
		Serial.println("GET /");        // for debugging
		request->send(200, "text/html", webpage);
	});

  	server.on("/command", HTTP_GET, [](AsyncWebServerRequest* request) {
		Serial.println("ESP32 Web Server: New request received:");  // for debugging
		Serial.println("GET /command");        // for debugging
		String inputMessage;
		String inputParam;

		if (request->hasParam(COMMAND_INPUT_PARAM)) {
			inputMessage = request->getParam(COMMAND_INPUT_PARAM)->value();
			inputParam = COMMAND_INPUT_PARAM;

			Serial.println(inputMessage);
			activateCommand((char*)inputMessage.c_str());
			request->send(200, "text/plain", "Sended! "+inputMessage);

		} else {
			request->send(200, "text/plain", "[ERROR] No message sent");
		}
	});

	server.begin();
	Keyboard.begin();
	Mouse.begin();
	USB.begin();
}

void loop() {
}

vector<string> splitString(string str, string delimiter)  {
	vector<string> result;
	int startIndex = 0;
	int endIndex = str.find(delimiter);
	while (endIndex != -1) {
		result.push_back(str.substr(startIndex, endIndex - startIndex));
		startIndex = endIndex + delimiter.size();
		endIndex = str.find(delimiter, startIndex);
	}
	result.push_back(str.substr(startIndex, endIndex - startIndex));
	return result;
}

void activateCommand(char* command) {
	string commandStr = string(command);
	vector<string> commandParts = splitString(commandStr, commandEnd);

	for (string action : commandParts) {
		if (action.rfind(multiKeyPrefix, 0) == 0) {
			action.erase(0, 1);
			vector<string> keys = splitString(action, actionEnd);

			for (string key : keys) {
				auto keyFound = KeysDict.find(key);
				Keyboard.press(keyFound != KeysDict.end() ? keyFound->second : key[0]);
			}

			Keyboard.releaseAll();

		} else if (action.rfind(mousePrefix, 0) == 0) {
			action.erase(0, 1);
			vector<string> mouseActions = splitString(action, actionEnd);

			if (action.rfind("MOVE", 0) == 0) {
				action.erase(0, 4);
				vector<string> moveParts = splitString(action, ",");
				Mouse.move(
					atoi(moveParts[0].c_str()),
					atoi(moveParts[1].c_str()),
					atoi(moveParts[2].c_str()),
					atoi(moveParts[3].c_str())
				);

				continue;
			}

			auto key = MouseDict.find(action);
			if (key != MouseDict.end()) Mouse.press(key->second);
			Mouse.release(MOUSE_ALL);

		} else {
			Keyboard.print(action.c_str());
		}
		
	}
}