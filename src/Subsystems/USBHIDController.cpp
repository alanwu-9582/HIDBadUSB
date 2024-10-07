#include <Arduino.h>

#include "utils/StringUtils.h"
#include "USBHIDController.h"
#include "dictionaries.h"

USBHIDController::USBHIDController() {}

void USBHIDController::begin(void) {
    Keyboard.begin();
    Mouse.begin();
    ConsumerControl.begin();
    SystemControl.begin();
    USB.begin();
}

void USBHIDController::executeMultiKeySequence(const vector<string> &keys) {
    for (string key : keys) {
        auto keyFound = KeysDict.find(key);
        Keyboard.press(keyFound != KeysDict.end() ? keyFound->second : key[0]);
    }

    Keyboard.releaseAll();
}

void USBHIDController::processMouseActions(const vector<string> &mouseActions) {
    for (string action : mouseActions) {
        if (action.rfind("MOVE", 0) == 0) {
            action.erase(0, 4);
            vector<string> moveParts = splitString(action, ",");
            Mouse.move(
                atoi(moveParts[0].c_str()),
                atoi(moveParts[1].c_str()),
                atoi(moveParts[2].c_str()),
                atoi(moveParts[3].c_str())
            );

        } else {
            auto actionFound = MouseDict.find(action);
            Mouse.press(actionFound != MouseDict.end() ? actionFound->second : action[0]);
        }
    }

    Mouse.release(MOUSE_ALL);
}

void USBHIDController::processMediaActions(const vector<string> &mediaActions) {
    for (string action : mediaActions) {
        auto actionFound = ConsumerControlDict.find(action);
        ConsumerControl.press(actionFound != ConsumerControlDict.end() ? actionFound->second : action[0]);
    }

    ConsumerControl.release();
}

void USBHIDController::processSystemActions(const vector<string> &systemActions) {
    for (string action : systemActions) {
        auto actionFound = SystemControlDict.find(action);
        SystemControl.press(actionFound != SystemControlDict.end() ? actionFound->second : action[0]);
    }

    SystemControl.release();
}


void USBHIDController::sendKeyboardText(const string &text) {
    Keyboard.print(text.c_str());
}
