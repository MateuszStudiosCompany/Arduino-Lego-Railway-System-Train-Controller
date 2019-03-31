# Lego Train Controller (ESP8266)

## Setup
1. Install required libraries:
   Library Name | Download link | Usage
   -------------|---------------|-------
   ArduinoJson  | [GitHub](https://github.com/bblanchon/ArduinoJson/releases) | JSON Decoding
   Adafruit NeoPixel | [GitHub](https://github.com/adafruit/Adafruit_NeoPixel/releases) | WS2812B LEDs
   MFRC522      | [GitHub](https://github.com/miguelbalboa/rfid/releases) | RFID Tag reader
   PowerFunctions   | [GitHub](https://github.com/jurriaan/Arduino-PowerFunctions) | Control of train movement
2. Add your wireless network credentials to train/Secrets.h
3. Configure app in train/AppConfig.h
4. Build and upload train.ino using Arduino addon for VS Code (recommended) or Arduino IDE

## Editing config webpage
1. Do changes to files in `web` directory
2. Run `python main.py` in `web` directory
3. Build and upload train.ino using Arduino addon for VS Code (recommended) or Arduino IDE

## How to update software over WiFi
1. Build train.ino to bin file
2. Use one of those methods:
   - Open `TrainHostnameOrIP/update` in your browser and upload bin file
   - Run `curl -F "image=@path_to_compiled.bin" --user username:password TrainHostnameOrIP/update`