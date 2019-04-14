#ifndef LIB_RFID
#define LIB_RFID

#include "AppConfig.h"

#include <SPI.h>
#include <MFRC522.h>

MFRC522 rc522(IO_MFRC522_SS_PIN, -1);

class RfidReader {
	private:
		
		String prepareRfidID(byte *buffer, byte bufferSize) {
			String a = "";
			for (byte i = 0; i < bufferSize; i++) {
				if (buffer[i] < 0x10){
					a += "0";
				}
				a += String(buffer[i],HEX);
			}
			return a;
		}

	public:
		String data;

		RfidReader() { 
			SPI.begin();
			rc522.PCD_Init();
		};

		bool isCardPresent() {
			if (rc522.PICC_IsNewCardPresent() && rc522.PICC_ReadCardSerial()){
				data = prepareRfidID(rc522.uid.uidByte, rc522.uid.size);
				rc522.PCD_StopCrypto1();
				Serial.printf("%14s:%-3d| Read new RFID card with UID: %s\n", FILE, __LINE__, data.c_str());
				return true;
			}
			return false;
		};
};
#endif