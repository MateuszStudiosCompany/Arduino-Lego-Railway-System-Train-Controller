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
		+ "&devtype=" + String(DEVICE_TYPE)
		+ "&type=" + request_type 
		+ "&value=" + value;
	
	if (!http.begin(client, url)) {
		#ifdef DEBUG_HTTP
		Serial.printf("%14s:%-3d| HTTP Request to %s failed\n", FILE, __LINE__, url.c_str());
		#endif
		return "";
	}
	
	http.setTimeout(HTTP_TIMEOUT);
	int httpCode = http.GET();
	String payload = http.getString();
	#ifdef DEBUG_HTTP
	Serial.printf("%14s:%-3d| HTTP Request to %s succeded\n", FILE, __LINE__, url.c_str());
	Serial.printf("%14s:%-3d| HTTP Response Code: %i\n", FILE, __LINE__, httpCode);
	#endif
	http.end();
	delay(1);
	client.stopAll();
	return payload;
}

class Networking{
	public:
		bool init(String hostname = "") {
			if (hostname == ""){
				hostname = HOSTNAME;
			}
			
			WiFi.hostname(hostname);
			wifiClientInit();
			wifiAPInit();
			client.setDefaultSync(true);
		}

		bool wifiAPInit(String ap_ssid = "", String ap_pass = ""){
			if (ap_ssid == "") {
				ap_ssid = AP_SSID;
			}
			if (ap_pass == "") {
				ap_pass = AP_PASS;
			}

			WiFi.mode(WIFI_AP_STA);
			WiFi.softAP(ap_ssid, ap_pass);
			Serial.printf("%14s:%-3d| Started AP with SSID: %s\n", FILE, __LINE__, AP_SSID);
			return true;
		}

		bool wifiClientInit(String client_ssid = "", String client_pass = ""){
			if (client_ssid == "") {
				client_ssid = DEFAULT_CLIENT_SSID;
			}
			if (client_pass == "") {
				client_pass = DEFAULT_CLIENT_PASS;
			}
			WiFi.begin(client_ssid, client_pass);

			Serial.printf("%14s:%-3d| Trying to connect to %s\n", FILE, __LINE__, client_ssid.c_str());

			if (WiFi.waitForConnectResult() != WL_CONNECTED) {
				Serial.printf("%14s:%-3d| Failed to connect to %s\n", FILE, __LINE__, client_ssid.c_str());
				return false;
			}

			Serial.printf("%14s:%-3d| Connected to %s\n", FILE, __LINE__, client_ssid.c_str());
			return true;
		}

		bool connected(){
			return WiFi.status() == WL_CONNECTED;
		}
};

Networking Network;
#endif