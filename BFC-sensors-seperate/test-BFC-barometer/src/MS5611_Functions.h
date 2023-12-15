#ifndef MS5611_FUNCTIONS_H
#define MS5611_FUNCTIONS_H

#include <MS5611.h>

class MS5611Functions {
public:
  MS5611Functions(uint8_t address);

  void initialize();
  void performTest();
  void setOversampling(osr_t samplingRate);
  
  // New functions for altitude and pressure
  float getTemperature();
  double getPressure();
  int getAltitude();
  
protected:
  uint8_t  _samplingRate;
private:
  MS5611 ms5611;
  uint32_t start, stop;
  unsigned long previousMillis;
  const unsigned long interval;
};

#endif
