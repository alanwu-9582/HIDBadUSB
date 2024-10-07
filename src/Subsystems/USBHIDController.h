#pragma once

#include <string>
#include <vector>

#include "USB.h"
#include "USBHIDKeyboard.h"
#include "USBHIDMouse.h"
#include "USBHIDConsumerControl.h"
#include "USBHIDSystemControl.h"

using std::string;
using std::vector;

class USBHIDController {
private:
    USBHIDKeyboard Keyboard;
    USBHIDMouse Mouse;
    USBHIDConsumerControl ConsumerControl;
    USBHIDSystemControl SystemControl;

public:
    USBHIDController();
    void begin(void);
    void executeMultiKeySequence(const vector<string>&);
    void processMouseActions(const vector<string>&);
    void processMediaActions(const vector<string>&);
    void processSystemActions(const vector<string>&);
    void sendKeyboardText(const string&);
};