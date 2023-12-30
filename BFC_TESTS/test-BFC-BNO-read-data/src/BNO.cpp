#include "BNO.h"

BNO::BNO()
    : BNO055_SAMPLERATE_DELAY_MS(1000), ADDRESS_SCAN_INTERVAL_MS(5000), bno(55, 0x28, &Wire), bnoAddress(0x28), bnoId(55), bnoWire(&Wire)
{
}

void BNO::setupBNO()
{
    bnoWire->begin();
    bnoWire->setClock(50000); // default is 100kH max is 400kH

    Serial.println("trying BNO555!");

    if (!bno.begin())
    {
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1)
            ;
    }

    Serial.println("BNO055 detected!");
}

void BNO::scanI2CAddresses()
{
    Serial.println("Scanning...");
    bnoWire->begin();
    Serial.println("Wire begin");

    byte error, address;
    int nDevices = 0;

    uint32_t startMillis = millis(); // Record the start time for address scanning

    while (millis() - startMillis <= ADDRESS_SCAN_INTERVAL_MS)
    {
        for (address = 1; address < 127; address++)
        {
            bnoWire->beginTransmission(address);
            error = bnoWire->endTransmission();

            if (error == 0)
            {
                Serial.print("I2C device found at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.print(address, HEX);
                Serial.println("  !");
                nDevices++;
            }
            else if (error == 4)
            {
                Serial.print("Unknown error at address 0x");
                if (address < 16)
                    Serial.print("0");
                Serial.println(address, HEX);
            }
        }
    }

    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
}

void BNO::printSensorData()
{
    sensors_event_t orientationData, angVelocityData, linearAccelData, magnetometerData, accelerometerData, gravityData;
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    bno.getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    bno.getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    bno.getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    bno.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    bno.getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

    // printEvent(&orientationData);
    printEvent(&angVelocityData);
    printEvent(&linearAccelData);
    printEvent(&magnetometerData);
    printEvent(&accelerometerData);
    printEvent(&gravityData);

    int8_t boardTemp = bno.getTemp();
    Serial.println();
    Serial.print(F("temperature: "));
    Serial.println(boardTemp);
}

void BNO::printCalibrationData()
{
    uint8_t system, gyro, accel, mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
    Serial.println();
    Serial.print("Calibration: Sys=");
    Serial.print(system);
    Serial.print(" Gyro=");
    Serial.print(gyro);
    Serial.print(" Accel=");
    Serial.print(accel);
    Serial.print(" Mag=");
    Serial.println(mag);

    Serial.println("--");
}

void BNO::printEvent(sensors_event_t *event)
{
    double x = -1000000, y = -1000000, z = -1000000; // dumb values, easy to spot a problem

    if (event->type == SENSOR_TYPE_ACCELEROMETER)
    {
        Serial.print("Accl:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else if (event->type == SENSOR_TYPE_ORIENTATION)
    {
        Serial.print("Orient:");
        x = event->orientation.x;
        y = event->orientation.y;
        z = event->orientation.z;
    }
    else if (event->type == SENSOR_TYPE_MAGNETIC_FIELD)
    {
        Serial.print("Mag:");
        x = event->magnetic.x;
        y = event->magnetic.y;
        z = event->magnetic.z;
    }
    else if (event->type == SENSOR_TYPE_GYROSCOPE)
    {
        Serial.print("Gyro:");
        x = event->gyro.x;
        y = event->gyro.y;
        z = event->gyro.z;
    }
    else if (event->type == SENSOR_TYPE_ROTATION_VECTOR)
    {
        Serial.print("Rot:");
        x = event->gyro.x;
        y = event->gyro.y;
        z = event->gyro.z;
    }
    else if (event->type == SENSOR_TYPE_LINEAR_ACCELERATION)
    {
        Serial.print("Linear:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else if (event->type == SENSOR_TYPE_GRAVITY)
    {
        Serial.print("Gravity:");
        x = event->acceleration.x;
        y = event->acceleration.y;
        z = event->acceleration.z;
    }
    else
    {
        Serial.print("Unk:");
    }

    Serial.print("\tx= ");
    Serial.print(x);
    Serial.print(" |\ty= ");
    Serial.print(y);
    Serial.print(" |\tz= ");
    Serial.println(z);
}

uint16_t BNO::getBNO055SamplerateDelay() const
{
    return BNO055_SAMPLERATE_DELAY_MS;
}

uint16_t BNO::getAddressScanInterval() const
{
    return ADDRESS_SCAN_INTERVAL_MS;
}

void BNO::setWirePins(uint8_t sdaPin, uint8_t sclPin)
{
    bnoWire->setPins(sdaPin, sclPin);
}

void BNO::setWireClock(uint32_t wireClock)
{
    bnoWire->setClock(wireClock);
}

void BNO::setBNOParameters(uint8_t address, uint8_t id, TwoWire *wire)
{
    bnoAddress = address;
    bnoId = id;
    bnoWire = wire;
}
