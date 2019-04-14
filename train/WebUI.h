#ifndef LIB_WEBUI
#define LIB_WEBUI

#include "Train.h"
#include "RfidReader.h"
#include "AppConfig.h"
#include "Networking.h"
#include "WebUITemplates.h"

#include <ESP8266WebServer.h>

class WebUI{
	private:
		ESP8266WebServer *server = nullptr;
		Train *train = nullptr;
		RfidReader *rfid = nullptr;
	
	public:
		bool checkNullPointer(){
			return (server != nullptr);
		}

		void handle() {
			if (!checkNullPointer()) return;
		}

		void handleSave(){
			//TODO: Use SPI flash to do that
			bool reloadNetwork = true;

			for (int i = 0; i < server->args(); i++) {
				String val = server->arg(i);
				String name = server->argName(i);

				if (name == "server_ip")
					SERVER_IP = val;

				else if (name == "server_path")
					SERVER_API_PATH = val;

				else if (name == "client_ssid")
					CLIENT_SSID = val;

				else if (name == "client_pass")
					CLIENT_PASS = val;

				else if (name == "reloadNetwork")
					reloadNetwork = false;

				else if (name == "analog_threshold")
					analog_threshold = val.toInt();
			}

			if (reloadNetwork){
				Network.wifiAPInit();
			}
			server->send(200, F("text/html"), PAGE_CONTENT_SAVED);
		}

		void handleStatusJson(){
			if (!checkNullPointer()) return;

			String message = "";
			const size_t capacity = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 340;
			StaticJsonDocument<capacity> doc;

			JsonObject form = doc.createNestedObject("form");
			form["client_ssid"] = 		CLIENT_SSID;
			form["client_pass"] = 		CLIENT_PASS;
			form["ap_ssid"] = 			AP_SSID;
			form["ap_pass"] =			AP_PASS;
			form["server_ip"] = 		SERVER_IP;
			form["server_path"] = 		SERVER_API_PATH;

			JsonObject table = doc.createNestedObject("table");
			table["state_led_front"] =	train->state_led_front;
			table["state_led_back"] = 	train->state_led_back;
			table["mode"] = 			5;
			table["disco_speed"] = 		train->disco_speed;
			table["rainbow_speed"] = 	train->rainbow_speed;
			table["current_rainbow"] =	train->current_rainbow;
			table["tick_id"] = 			train->tick_id;
			table["motor_speed"] = 		train->motor_speed;
			table["motor_direction"] = 	train->motor_direction;
			table["esp_boot_mode"] = 	ESP.getBootMode();
			table["esp_free_heap"] = 	ESP.getFreeHeap();

			table["rfid_id"] = 			rfid->data;
			table["analog_value"] = 		analog_value;
			table["analog_threshold"] =	analog_threshold;

			serializeJson(doc, message);
			server->send(200, F("application/json"), message);
			
		}

		void handleIndex() {
			if (!checkNullPointer()) return;
			server->send(200, F("text/html"), PAGE_CONTENT_INDEX);
		}

		void handleNotFound() {
			if (!checkNullPointer()) return;
			server->send(404, F("text/html"), PAGE_CONTENT_404);
		}

		void init(ESP8266WebServer &server, Train &train, RfidReader &rfid) {
			this->server = &server;
			this->train = &train;
			this->rfid = &rfid;
			server.on("/", std::bind(&WebUI::handleIndex, this));
			server.on("/save", std::bind(&WebUI::handleSave, this));
			server.on("/status", std::bind(&WebUI::handleStatusJson, this));
			server.onNotFound(std::bind(&WebUI::handleNotFound, this));
			// server.on("/", handle);
		}
};

#endif