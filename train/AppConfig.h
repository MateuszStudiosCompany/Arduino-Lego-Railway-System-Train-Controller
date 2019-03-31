#include "Secrets.h" // For storing WiFi SSIDs and passwords

#define SERIAL_BAUD 250000

// Network config
#define HOSTNAME "webtrain1"
#define DEFAULT_SERVER_IP "10.0.0.201"
#define DEFAULT_SERVER_API_PATH "train_api"
#define APMODE

#define HTTP_TIMEOUT 300
#define JSON_BUFFER  430

// Update config
#define UPDATE_PATH 			"/update"
#define UPDATE_USER 			"admin"
#define UPDATE_PASS 			"admin"

// Things used for interfacing with humans
#define IO_IR_TRANSMIT_PIN   	15
#define IO_LED_WS2812B_PIN   	5
#define IO_LED_FRONT_PIN 		16
#define IO_BEEP_PIN	  			4
#define IO_MFRC522_SS_PIN		2

#define IO_LED_WS2812B_COUNT 	3


// Debug
#define DEBUG_HTTP
#define MAIN_TICK_DELAY 5000 // [ns]


/* DO NOT CHANGE THIS */
#ifndef CONFIG_LOADED
#define CONFIG_LOADED
String SERVER_IP = DEFAULT_SERVER_IP;
String SERVER_API_PATH = DEFAULT_SERVER_API_PATH;
String CLIENT_SSID = DEFAULT_CLIENT_SSID;
String CLIENT_PASS = DEFAULT_CLIENT_PASS;
#endif

#ifndef FILE
#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif