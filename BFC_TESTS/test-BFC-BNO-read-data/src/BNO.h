#ifndef BNO_H
#define BNO_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class BNO
{
public:
    BNO();
    void setupBNO();
    void scanI2CAddresses();
    void printSensorData();
    void printCalibrationData();
    uint16_t getBNO055SamplerateDelay() const;
    uint16_t getAddressScanInterval() const;
    void setWirePins(uint8_t sdaPin, uint8_t sclPin);
    void setWireClock(uint32_t wireClock);
    void setBNOParameters(uint8_t address, uint8_t id, TwoWire *wire);

private:
    uint16_t BNO055_SAMPLERATE_DELAY_MS;
    uint16_t ADDRESS_SCAN_INTERVAL_MS;
    Adafruit_BNO055 bno;
    uint8_t bnoAddress;
    uint8_t bnoId;
    TwoWire *bnoWire;

    void printEvent(sensors_event_t *event);
};

#endif
