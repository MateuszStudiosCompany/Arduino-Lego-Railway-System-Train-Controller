#ifndef LIB_TRAIN
#define LIB_TRAIN

#include "AppConfig.h"
#include "Networking.h"

#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <PowerFunctions.h>

PowerFunctions pf(IO_IR_TRANSMIT_PIN, 0);

class Train {
	private:
		Adafruit_NeoPixel strip;
	
	public:
		// Lights:
		int state_rgb_leds[3][3];
		bool state_led_front;
		bool state_led_back;
		int led_control_mode;
		int disco_speed = 10;
		int rainbow_speed = 200;
		int current_rainbow = 0;
		uint8_t motor_speed = 0; // 0-5 (or 6?). 0 is STOP
		bool motor_direction = true; //false -> backwards | true -> forwards
		bool motor_change = false;

		// Internal state:
		int tick_id = 0;

	public:
		Train() {
			pinMode(IO_LED_WS2812B_PIN, OUTPUT);
			pinMode(IO_LED_FRONT_PIN, OUTPUT);
			pinMode(IO_BEEP_PIN, OUTPUT);
			
			strip = Adafruit_NeoPixel(IO_LED_WS2812B_COUNT, IO_LED_WS2812B_PIN,
										NEO_GRB + NEO_KHZ800);

			
			strip.begin();
			strip.setBrightness(255);

			state_led_front = false;
			state_led_back = false;
			led_control_mode = 0;
		};

		void dbgLED(int howMany, int color){
			for (int i = 0; i <= howMany; i++){
				state_rgb_leds[i][0] = 0;
				state_rgb_leds[i][1] = 0;
				state_rgb_leds[i][2] = 0;
				state_rgb_leds[i][color] = 255;
			}
			renderx();
		}

		void tickx() {
			if (tick_id++ >= 65535) tick_id = 0;
			// TODO: send state via IR.

			if (led_control_mode == 0) {
				// Manual control
				renderx();
				return;

			} else if (led_control_mode == 1) {
				if (tick_id % rainbow_speed == 0) {
					for (int i = 0; i < 3; i++) {
						strip.setPixelColor(i, ledWheel(current_rainbow & 255));
					}
					if (current_rainbow++ >= 255) current_rainbow = 0;
					strip.show();
				}
				digitalWrite(IO_LED_FRONT_PIN, state_led_front);
				return;

			} else if (led_control_mode == 2) {
				if (tick_id % disco_speed == 0) {
					for (int i = 0; i < 3; i++) {
						strip.setPixelColor(i, ledWheel(random(10, 180) & 255));
					}
					strip.show();
				}
				digitalWrite(IO_LED_FRONT_PIN, state_led_front);
				return;
			}

		};

		void renderx() {
			digitalWrite(IO_LED_FRONT_PIN, state_led_front);
			for (uint8_t i = 0; i < 3; i++) {
				strip.setPixelColor(i, 
					state_rgb_leds[i][0], 
					state_rgb_leds[i][1],
					state_rgb_leds[i][2]
				);
			}
			strip.show();
			return;
		};

		int32_t ledWheel(byte WheelPos) {
			WheelPos = 255 - WheelPos;
			if (WheelPos < 85) {
				return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
			}

			if (WheelPos < 170) {
				WheelPos -= 85;
				return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
			}

			WheelPos -= 170;
			return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
		}

		void getLeds() {
			String payload = doRequest("GET_LEDS", "");
			
			if (payload != "") {
				StaticJsonDocument<JSON_BUFFER> doc;
				DeserializationError error = deserializeJson(doc, payload);

				if (error) {
					Serial.printf("%14s:%-3d| JSON decode error\n", FILE, __LINE__);
					return;
				}

				if ((motor_direction != doc["motor_dir"]) || (motor_speed != doc["motor_speed"])){
					motor_change = true;
				}

				motor_direction = doc["motor_dir"];
				motor_speed = doc["motor_speed"];
				state_led_front = doc["leds"]["front"];
				state_led_back = doc["leds"]["back"];
				led_control_mode = doc["leds"]["led_control_mode"];
				disco_speed = doc["leds"]["disco_speed"];
				rainbow_speed = doc["leds"]["rainbow_speed"];

				for (uint8_t i = 0; i < 3; i++) {
					for (uint8_t j = 0; j < 3; j++) {
						state_rgb_leds[i][j] = doc["leds"]["rgb"][i][j];
					}
				}

			}
			return;
		}

		void updateMotorState(){
			if (motor_speed == 0){ // Stop
				pf.combo_pwm(PWM_BRK,PWM_BRK);
				pf.combo_pwm(PWM_FLT,PWM_FLT); //WTF?
				return;
			}

			uint8_t base_command = 0x0;
			
			if (motor_speed > 0 && motor_speed <= 5){
				if (!motor_direction) {
					base_command = 0x9;
					base_command += (6 - motor_speed);
				}else{
					base_command += motor_speed;
				}
				pf.combo_pwm(base_command, base_command);
			}
		}

		void sendRFID(String &rfid_tag) {
			// TODO: Parse response
			String payload = doRequest("READ_TAG", rfid_tag);
			return;
		}
};
#endif