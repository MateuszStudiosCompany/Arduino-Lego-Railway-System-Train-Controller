# Overview
## New version soon!
**Arduino Lego Railway System** is a first version of an idea to create a fully automatic lego train system
(currently being developed as [Train32 Project](https://gitlab.com/FlyingArmageddon/train32)). __This repository is only for archive purpouses. Visit [Train32 Project Repo](https://gitlab.com/FlyingArmageddon/train32) to get newest version (when it becomes available).__

## About Train Controller
**Train Controller** is designed for Lego Trains (and ESP8266) and provides a few extra features for them:
- RGB lighting (WS2812B),
- Horn and front-lights,
- Auto start-stop,
- Communication with server to receive commands,
- Train position detection (RFID-based),
- Builtin web configuration.

## Project-related repos:
- [Server](https://github.com/MateuszStudiosCompany/Arduino-Lego-Railway-System-Server) - trains traffic control.
- [Web Train Controller](https://github.com/MateuszStudiosCompany/Arduino-Lego-Railway-System-Web-Train-Controller) - website for changing train lighting, horn or blink lights.
- [Train Controller](https://github.com/MateuszStudiosCompany/Arduino-Lego-Railway-System-Train-Controller) - all you need for trains **(you are here)**.
- [Environment Controller](https://github.com/MateuszStudiosCompany/Arduino-Lego-Railway-System-Environment-Controller) - railway crossings, semaphores, etc.

*Note: this old version is not fully automatic for more than one train ([Train32](https://gitlab.com/FlyingArmageddon/train32) will be)!*

# Setup
1. Install required libraries:

    | Library Name | Download link | Usage |
    | -------------|---------------|------- |
    | ArduinoJson  | [GitHub](https://github.com/bblanchon/ArduinoJson/releases) | JSON Decoding |
    | Adafruit NeoPixel | [GitHub](https://github.com/adafruit/Adafruit_NeoPixel/releases) | WS2812B LEDs |
    | MFRC522      | [GitHub](https://github.com/miguelbalboa/rfid/releases) | RFID Tag reader |
    | PowerFunctions   | [GitHub](https://github.com/jurriaan/Arduino-PowerFunctions) | Control of train movement |

1. Add your wireless network credentials to train/Secrets.h
2. Configure app in train/AppConfig.h
3. Build and upload train.ino using Arduino addon for VS Code (recommended) or Arduino IDE

# Editing config webpage
1. Do changes to files in `web` directory
2. Run `python main.py` in `web` directory
3. Build and upload train.ino using Arduino addon for VS Code (recommended) or Arduino IDE

# How to update software over WiFi
1. Build train.ino to bin file
2. Use one of those methods:
   - Open `TrainHostnameOrIP/update` in your browser and upload bin file
   - Run `curl -F "image=@path_to_compiled.bin" --user username:password TrainHostnameOrIP/update`