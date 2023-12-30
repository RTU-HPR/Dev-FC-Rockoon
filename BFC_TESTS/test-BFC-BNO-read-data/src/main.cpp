#include <Arduino.h>
#include "BNO.h"
#include <SPI.h>

//Depending on the computer these lines in platformio.ini (even if commented out) can cause 
//the serial not to work
// but there are cases when they help it to work... idk witch craft
//;monitor_rts = 0
//;monitor_dtr = 0

BNO bno;
uint32_t previousMillis = 0;

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        // Wait for Serial to be ready
    }
    
    bno.scanI2CAddresses();
    Serial.println("Orientation Sensor Test");
    Serial.println("");

    // Set BNO parameters
    bno.setWirePins(17, 18);
    bno.setWireClock(50000);
    bno.setBNOParameters(0x28, 55, &Wire);
    bno.setupBNO();
}

void loop()
{
    uint32_t currentMillis = millis();

    if (currentMillis - previousMillis >= bno.getBNO055SamplerateDelay())
    {
        bno.printSensorData();
        bno.printCalibrationData();

        previousMillis = currentMillis;
        Serial.println(previousMillis);
    }
}
