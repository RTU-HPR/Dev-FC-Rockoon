#include <Arduino.h>
#include "gps_wrapper.h"
#include <Wire.h>

void setup(){
  Serial.begin(115200);
  	for (int i=1; i<5; i++){	
	Serial.println("SETUP");
	delay(1000);}
	gps::setup();
}

void loop()
{	
    gps::readGps();
    gps::printData();
    delay(1000);

}