#pragma once

#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

const unordered_map<string, char> KeysDict = {
    {"LEFT_CTRL", 0x80},
    {"LEFT_SHIFT", 0x81},
    {"LEFT_CTRL", 0x80},
    {"LEFT_SHIFT", 0x81},
    {"LEFT_ALT", 0x82},
    {"LEFT_GUI", 0x83},
    {"RIGHT_CTRL", 0x84},
    {"RIGHT_SHIFT", 0x85},
    {"RIGHT_ALT", 0x86},
    {"RIGHT_GUI", 0x87},
    {"UP_ARROW", 0xDA},
    {"DOWN_ARROW", 0xD9},
    {"LEFT_ARROW", 0xD8},
    {"RIGHT_ARROW", 0xD7},
    {"MENU", 0xFE},
    {"SPACE", 0x20},
    {"BACKSPACE", 0xB2},
    {"TAB", 0xB3},
    {"RETURN", 0xB0},
    {"ESC", 0xB1},
    {"INSERT", 0xD1},
    {"DELETE", 0xD4},
    {"PAGE_UP", 0xD3},
    {"PAGE_DOWN", 0xD6},
    {"HOME", 0xD2},
    {"END", 0xD5},
    {"NUM_LOCK", 0xDB},
    {"CAPS_LOCK", 0xC1},
    {"F1", 0xC2},
    {"F2", 0xC3},
    {"F3", 0xC4},
    {"F4", 0xC5},
    {"F5", 0xC6},
    {"F6", 0xC7},
    {"F7", 0xC8},
    {"F8", 0xC9},
    {"F9", 0xCA},
    {"F10", 0xCB},
    {"F11", 0xCC},
    {"F12", 0xCD},
    {"F13", 0xF0},
    {"F14", 0xF1},
    {"F15", 0xF2},
    {"F16", 0xF3},
    {"F17", 0xF4},
    {"F18", 0xF5},
    {"F19", 0xF6},
    {"F20", 0xF7},
    {"F21", 0xF8},
    {"F22", 0xF9},
    {"F23", 0xFA},
    {"F24", 0xFB},
    {"PRINT_SCREEN", 0xCE},
    {"SCROLL_LOCK", 0xCF},
    {"PAUSE", 0xD0}
};

const unordered_map<string, char> MouseDict = {
    {"LEFT", 0x01},
    {"RIGHT", 0x02},
    {"MIDDLE", 0x04},
    {"BACKWARD ", 0x08},
    {"FORWARD", 0x10},
    {"ALL", 0x1F}
};

const unordered_map<string, char> ConsumerControlDict = {
    {"VOLUME_UP", 0x00E9},
    {"VOLUME_DOWN", 0x00EA},
    {"MUTE", 0x00E2},
    {"PLAY_PAUSE", 0x00CD},
    {"STOP", 0x00B7},
    {"BRIGHTNESS_UP", 0x006F},
    {"BRIGHTNESS_DOWN", 0x0070},
};

const unordered_map<string, char> SystemControlDict = {
    {"POWER_OFF", 1},
    {"STANDBY", 2},
    {"WAKE_HOST", 3}
};

