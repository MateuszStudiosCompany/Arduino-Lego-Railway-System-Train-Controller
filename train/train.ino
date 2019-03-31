uint8_t main_tick = 0;
int analog_value = 0;
int analog_threshold = 500;

#include "WebUI.h"
#include "Train.h"
#include "AppConfig.h"
#include "Networking.h"
#include "RfidReader.h"

#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
Train tc;
RfidReader rfid;
WebUI wu;

void serialInit() {
	Serial.println();
	Serial.println();
	Serial.println("------------------------------\n");
	Serial.println("     Lego Train Controller    \n");
	Serial.println("------------------------------\n");
	Serial.println();
	Serial.printf("%14s:%-3d| Boot reason: %i\n", FILE, __LINE__, ESP.getBootMode());
	Serial.printf("%14s:%-3d| Free heap: %i\n", FILE, __LINE__, ESP.getFreeHeap());
}

void networkingInit() {
	Network.init();

	httpUpdater.setup(&httpServer, UPDATE_PATH, UPDATE_USER, UPDATE_PASS);
	Serial.printf("%14s:%-3d| Started HTTP Updater\n", FILE, __LINE__);

	httpServer.begin();
	Serial.printf("%14s:%-3d| Started HTTP Server\n", FILE, __LINE__);
}

void setup() {
	tc.dbgLED(1, 1);
	Serial.begin(SERIAL_BAUD);
	serialInit();

	delay(1000);
	networkingInit();
	tc.dbgLED(2, 1);

	delay(500);
	wu.init(httpServer, tc, rfid);

	tc.dbgLED(3, 1);
	//TODO: Selftest here
}

void loop() {
	
	if (main_tick++ >= 255) main_tick = 0;
	delayMicroseconds(MAIN_TICK_DELAY);

	if (!Network.connected()) {
		Serial.printf("%14s:%-3d| Lost WiFi connection...\n", FILE, __LINE__);
		Network.wifiClientInit();
		return;
	}
	
	httpServer.handleClient();

	//TODO: send this value of A0
	analog_value = analogRead(A0);

	if (analog_value < analog_threshold){
		tc.dbgLED(3, 0);
		tc.motor_emergency_stop = true;
		tc.updateMotorState();
	}else{
		tc.motor_emergency_stop = false;
		tc.dbgLED(3, 2);
	}

	if (rfid.isCardPresent()) {
		tc.sendRFID(rfid.data);
	}

	if (main_tick % 16 == 0) {
		tc.updateMotorState();
	}

	
	if (main_tick % 32 == 0) {
		//tc.tickx();
		tc.renderx();

		if (main_tick % 64 == 0) {
			tc.getLeds();
		}
	}
	return;
}
