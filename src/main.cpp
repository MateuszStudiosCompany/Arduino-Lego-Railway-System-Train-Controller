#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <secrets.h>

#define WS2812B_PIN   12
#define WS2812B_COUNT 3
#define LED_FRONT_PIN 16
#define BEEP_PIN	  5

const char* ssid = WLAN_SSID;
const char* password =  WLAN_PASSWORD;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(WS2812B_COUNT, WS2812B_PIN, NEO_GRB + NEO_KHZ800);

#include <ESP8266WiFi.h>            
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

bool front_led_on = false;
bool demo_mode = false;
bool disco_mode = false;
int disco_speed = 10;
int rgb_leds[3][3];

void handleSetCommand() {
	Serial.println("Start");
	String message = "Number of args received:";
	message += server.args();
	message += "\n";

	int r,g,b;

	for (int i = 0; i < server.args(); i++) {
		String val = server.arg(i);
		String name = server.argName(i);

		if (name == "front_led"){
			if (val == "1") {
				front_led_on = true;
			}else{
				front_led_on = false;
			}
		}else if (name == "demo_mode"){
			if (val == "1") {
				disco_mode = false;
				demo_mode = true;
			}else{
				demo_mode = false;
			}
		}else if (name == "disco_mode"){
			if (val == "1") {
				demo_mode = false;
				disco_mode = true;
			}else{
				disco_mode = false;
			}
		}else if (name == "disco_speed"){
			disco_speed = val.toInt();
		}else if (name == "led_1"){
			rgb_leds[0][0] = val.substring(0,3).toInt();
			rgb_leds[0][1] = val.substring(4,7).toInt();
			rgb_leds[0][2] = val.substring(8,11).toInt();
		}else if (name == "led_2"){
			rgb_leds[1][0] = val.substring(0,3).toInt();
			rgb_leds[1][1] = val.substring(4,7).toInt();
			rgb_leds[1][2] = val.substring(8,11).toInt();
		}else if (name == "led_3"){
			rgb_leds[2][0] = val.substring(0,3).toInt();
			rgb_leds[2][1] = val.substring(4,7).toInt();
			rgb_leds[2][2] = val.substring(8,11).toInt();
		}

		message += server.argName(i) + ": ";
		message += val + "\n"; 
	} 

	if (!demo_mode){
		for (int i = 0; i < 3; i++){
			strip.setPixelColor(i, rgb_leds[i][0], rgb_leds[i][1], rgb_leds[i][2]);
		}
		
		strip.show();
	}

	digitalWrite(LED_FRONT_PIN, front_led_on);

	server.send(200, "text/plain", message);
}

void handleBeepCommand() {
	tone(BEEP_PIN, 400, 500);
	digitalWrite(LED_FRONT_PIN, HIGH);
	delay(100);
	digitalWrite(LED_FRONT_PIN, LOW);
	delay(100);
	digitalWrite(LED_FRONT_PIN, HIGH);
	delay(100);
	digitalWrite(LED_FRONT_PIN, LOW);
	server.send(200, "text/plain", "OK");
}

void handleGetSettingsCommand() {
	String message = "{\n";

	message += "\tfront_led: ";
	message += front_led_on ;
	message += ",\n";

	message += "\tdemo_mode: ";
	message += demo_mode ;
	message += ",\n";

	message += "\tdisco_mode: ";
	message += disco_mode ;
	message += ",\n";

	message += "\tdisco_speed: ";
	message += disco_speed ;
	message += ",\n";

	message += "\trgb_leds: {\n";

	for (int lid = 0; lid < 3; lid++){
		message += "\t\t";
		message += lid;
		message += ":[";

		for (int c = 0; c < 3; c++){
			message += rgb_leds[lid][c];
			if (c != 2) {
				message += ",";
			}
		}

		if (lid == 2) {
			message += "]\n";
		}else{
			message += "],\n";
		}
	}
	
	message += "\t},\n";
	message += "\tstatus: true\n";

	message += "}";
	server.send(200, "application/json", message);
}

void setup() {
	Serial.begin(115200);

  	strip.begin();
  	strip.setBrightness(255);
  	strip.show();
  	pinMode(16, OUTPUT);
	Serial.print("Leds initialized");
	
	delay(100);
	WiFi.mode(WIFI_STA);
	delay(100);

	WiFi.begin(ssid, password);
	
	Serial.print("Waiting for connection");
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("Connected");

	delay(100);
	server.on("/set", handleSetCommand);
	server.on("/getSettings", handleGetSettingsCommand);
	server.on("/beep", handleBeepCommand);

	delay(100);
	server.begin();
	Serial.print("Server listening on ");   
	Serial.print(WiFi.localIP());
	Serial.println(":80"); 
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
	uint16_t i, j;
	for(j=0; j<256; j++) {
		delay(wait);
	}
}

void stopSound(){
}

void makeSound(){
	tone(BEEP_PIN, 400, 500);
}

void loop() {
	server.handleClient();
	if (demo_mode){
		int endMillis = millis() + (1000 * 25);
		int currentColor = 0; 

		while (endMillis > millis()){
			if (millis() % 100 == 0){
				for(int i=0; i<3; i++) {
					strip.setPixelColor(i, Wheel(currentColor & 255));
				}
				currentColor++;
				strip.show();
			}
			server.handleClient();
			if (!demo_mode) break;
		}
	}else if (disco_mode){
		int endMillis = millis() + (1000 * 1);

		while (endMillis > millis()){
			if (millis() % disco_speed == 0){
				for(int i=0; i<3; i++) {
					strip.setPixelColor(i, Wheel(random(10,180) & 255));
				}
				strip.show();
			}
			server.handleClient();
			if (!disco_mode) break;
		}
	}

}