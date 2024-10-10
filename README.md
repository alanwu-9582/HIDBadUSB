HID-BadUSB
======

![license MIT](https://img.shields.io/badge/license-MIT-blue)

> 2024 &copy; alanwu-9852
> 
> Language: English / [繁體中文](./README.zh-TW.md)

This project provides a web-based interface for generating BadUSB payloads. The payloads are generated in the form of Arduino code, which can be compiled and uploaded to ESP32-S3 DevKitC-1. The payloads are designed to be executed by the board when it is connected to a computer, and can be used to perform various tasks such as keystroke injection, mouse emulation, and more.

Features
---
* **Asynchronous Web Server**: Uses ESPAsyncWebServer for efficient and non-blocking web interactions.
* **HID keyboard and mouse emulation**: Uses the ESP32-S3's built-in USB HID capabilities to emulate a keyboard and mouse.
* **OTA updates**: Allows for easy updating of the board's firmware over the air.

Prerequisites
---
**ESP32-S3-DevKitC-1**: An ESP32-S3 development board with 16MB flash and 8MB PSRAM.

Getting Started
---
### 1. Clone the Repository
```bash
git clone https://github.com/alanwu-9582/HIDBadUSB.git
cd HIDBadUSB
```

### 2. Project Configuration
In platformio.ini, we define the board, platform, and required libraries. Below is the key part of the configuration:
```ini
[env:esp32-s3-devkitc-1]
platform = espressif32
board = esp32-s3-devkitc-1
framework = arduino
monitor_speed = 115200
board_build.flash_size = 16MB
board_build.psram_size = 8MB

lib_deps = 
    ESPAsyncWebServer
    AsyncTCP
```

### 3. Upload the Code
You need to upload the code to the ESP32-S3-DevKitC-1 board with cable connected for the first time. After that, you can use OTA updates to update the firmware.

Web Interface
---
Once the firmware is uploaded, connect to the ESP32-S3 board through its IP address. (You can find the IP address in the serial monitor.) The web interface provides the following features:

### 1. Command Input
You can input commands to be executed by the board. The commands can be in the form of keystrokes, mouse movements, or other HID actions. Commands need to follow the syntax defined in the [Command Syntax](#command-syntax) section.

**AddSymbol** button is a handy tool for adding special characters to the command input.

### 2. KeyTable
The KeyTable provides a list of all the keys that can be used in commands. When the **InstantRun** switch is turned off, the KeyTable will be used to add keys to the command input when a cell is clicked. Otherwise, the keys will be sent as the command to the board immediately.

### 3. Wait Command Editor
The Wait Command Editor allows you to add wait commands to the command input. Wait commands can be used to pause the execution of the command sequence for a specified amount of time.

### 4. Script Runner
The Script Runner can run complex assembled commands. Select the script and click the **RunScript** button to execute the script.

You can upload a costume script file `.usbhidscript` by clicking the **Upload Script** button. The script file should be in the following format:
```JSON
{
    "type": "USBHID-Script",
    "scripts" : [
        {
            "name": "<script name>",
            "command": "<command>"
        },
    ]
}
```