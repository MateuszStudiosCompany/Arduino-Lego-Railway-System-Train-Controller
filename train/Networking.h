#ifndef LIB_NET
#define LIB_NET

#include "AppConfig.h"

#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient client;
HTTPClient http;
String wlan_mac = String(WiFi.macAddress());
String doRequest(String request_type, String value) {
	String url = "http://" 
		+ String(SERVER_IP) + "/" 
		+ String(SERVER_API_PATH)  + "/"
		+ "?device=" + wlan_mac
		+ "&type=" + request_type 
		+ "&value=" + value;
	
	if (!http.begin(client, url)){
		Serial.printf("%14s:%-3d| HTTP Request to %s failed\n", FILE, __LINE__, url.c_str());
		return "";
	}
	
	http.setTimeout(HTTP_TIMEOUT);
	int httpCode = http.GET();
	String payload = http.getString();
	
	Serial.printf("%14s:%-3d| HTTP Request to %s succeded\n", FILE, __LINE__, url.c_str());
	Serial.printf("%14s:%-3d| HTTP Response Code: %i\n", FILE, __LINE__, httpCode);
	http.end();
	return payload;
}

class Networking{
	public:
		bool init(){
			wifiAPInit();
			wifiClientInit();
			WiFi.hostname(HOSTNAME);
		}

		bool wifiAPInit(){
			WiFi.mode(WIFI_AP_STA);
			WiFi.softAP(AP_SSID, AP_PASS);
			Serial.printf("%14s:%-3d| Started AP with SSID: %s\n", FILE, __LINE__, AP_SSID);
			return true;
		}

		bool wifiClientInit(){
			WiFi.begin(DEFAULT_CLIENT_SSID, DEFAULT_CLIENT_PASS);
			
			Serial.printf("%14s:%-3d| Trying to connect to %s\n", FILE, __LINE__, CLIENT_SSID.c_str());

			if (WiFi.waitForConnectResult() != WL_CONNECTED) {
				Serial.printf("%14s:%-3d| Failed to connect to %s\n", FILE, __LINE__, CLIENT_SSID.c_str());
				return false;
			}

			Serial.printf("%14s:%-3d| Connected to %s\n", FILE, __LINE__, CLIENT_SSID.c_str());
			return true;
		}

		bool connected(){
			return WiFi.status() == WL_CONNECTED;
		}
};

extern Networking Network;

#endif