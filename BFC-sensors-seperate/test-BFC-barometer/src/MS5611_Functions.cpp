#include "MS5611_Functions.h"
#include <Arduino.h>

MS5611Functions::MS5611Functions(uint8_t address) : ms5611(address), previousMillis(0), interval(1000) {}

void MS5611Functions::initialize() {
  Serial.println("Initialize MS5611 Sensor");

  while (!ms5611.begin()) {
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
  }

  Serial.println("MS5611 found.");
}

void MS5611Functions::performTest() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    start = micros();
    
    int result = ms5611.read();
    stop = micros();
  
    if (result != MS5611_READ_OK) {
      Serial.print("Error in read: ");
      Serial.println(result);
    } else {
      Serial.print("T:\t");
      Serial.print(ms5611.getTemperature(), 2);
      Serial.print("\tP:\t");
      Serial.print(ms5611.getPressure(), 2);
      Serial.print("\tt:\t");
      Serial.print(stop - start);
      Serial.println();
    }
  }
}

void MS5611Functions::setOversampling(osr_t samplingRate)
{
  _samplingRate = (uint8_t) samplingRate;
}

float MS5611Functions::getTemperature() {
  // Return altitude using the getAltitude method from the MS5611 library
  return ms5611.getTemperature();
}

double MS5611Functions::getPressure() {
  // Return pressure using the getPressure method from the MS5611 library
  return ms5611.getPressure();
}

int MS5611Functions::getAltitude() {
  // Standard sea-level pressure in Pascals
  const float seaLevelPressure = 101325.0;

  // Get the current pressure from the sensor
  float currentPressure = ms5611.getPressure();

  // Calculate the altitude using the barometric formula
  float altitude = 44330.0 * (1.0 - pow(currentPressure / seaLevelPressure, 0.1903));

  return altitude;
}