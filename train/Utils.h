#include <Arduino.h>

#ifndef LIB_UTILS
#define LIB_UTILS

#define MEASURE_SLOTS 16

class TimeMeasure{
	public:
		float start_time[MEASURE_SLOTS];
		float end_time[MEASURE_SLOTS];

		bool start(int mid){
			if ((mid < 0) || (mid >= MEASURE_SLOTS)) return false;
			start_time[mid] = millis();
			return true;
		}

		bool stop(int mid){
			if ((mid < 0) || (mid >= MEASURE_SLOTS)) return false;
			end_time[mid] = millis();
			return true;
		}

		bool reset(int mid){
			if ((mid < 0) || (mid >= MEASURE_SLOTS)) return false;
			start_time[mid] = -1;
			end_time[mid] = -1;
			return true;
		}

		bool resetAll(){
			for (int i = 0; i < MEASURE_SLOTS; i++){
				reset(i);
			}
		}

		bool dumpAll(){
			Serial.println();
			Serial.println();
			Serial.println("---CUT HERE---");
			for (int i = 0; i < MEASURE_SLOTS; i++){
				int result = 0;

				if ((start_time[i] == -1) || (end_time[i] == -1)){
					result = -1;
				}else{
					result = end_time[i] - start_time[i];
				}

				Serial.print(result);
				Serial.print(",");
			}
			Serial.println();
			Serial.println("---CUT HERE---");
			Serial.println();
			Serial.println();
		}

		float getResult(int mid){
			if ((mid < 0) || (mid >= MEASURE_SLOTS)) return -1;
			return end_time[mid] - start_time[mid];
		}
};

#endif