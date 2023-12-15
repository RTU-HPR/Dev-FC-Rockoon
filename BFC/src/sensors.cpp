#include "sensors.h"

bool Sensors::begin_barometer(int i2c_address = 0x77, uint ms56xx_type = 0)
{
    // 0 for board 1, 1 for board 2
    barometer = new MS56XX(i2c_address, ms56xx_type);

    if (!barometer->begin())
    {
        Serial.println("Barometer begin failed");
        return false;
    };
    barometer->configBaro(BARO_PRESS_D1_OSR_4096, BARO_TEMP_D2_OSR_4096);

    Serial.println("Barometer begin success");
    barometer_initialized = true;
    return true;
}

bool Sensors::begin_imu(TwoWire &wire, int sensor_id = 55, int i2c_address = 0x28)
{
    imu = new Adafruit_BNO055(sensor_id, i2c_address, &wire);
    // good idead to add a specified operating mode (update frquency etc.)
    if (!imu->begin())
    {
        Serial.println("IMU begin failed");
        return false;
    }
    Serial.println("IMU begin success");

    imu_initialized = true;
    return true;
}
bool Sensors::begin_gps(const Gps_Wrapper::Gps_Config_UART &gps_config)
{
    gps = new Gps_Wrapper(nullptr, "GPS");
    unsigned long start = millis();
    while (!gps->begin(gps_config))
    {
        if (start + 15000 < millis())
        {
            Serial.println("GPS begin failed COMPLETELY");
            return false;
        }
        Serial.println("GPS begin failed, retrying");
    }

    gps_initialized = true;
    return true;
}
bool Sensors::init_sensors(const Config &config)
{
    bool success = true;
    if (!begin_barometer(config.MS5611_ADDRESS_I2C, config.MS5611_SENSOR_ID))
    {
        success = false;
    }
    if (!begin_imu(config.BNO055_WIRE, config.BNO055_SENSOR_ID, config.BNO055_ADDRESS_I2C))
    {
        success = false;
    }
    if (!begin_gps(config.gps_config))
    {
        success = false;
    }
    return success;
}

void Sensors::read_sensors()
{
    Barometer_Data baro_data;
    if (read_barometer(baro_data))
    {
        sensor_data.barometer_data = baro_data;
    }
    IMU_Data imu_data;
    if (read_imu(imu_data))
    {
        sensor_data.imu_data = imu_data;
    }
    Gps_Wrapper::Gps_Data gps_data;
    if (read_gps(gps_data))
    {
        sensor_data.gps_data = gps_data;
    }
}

bool Sensors::read_barometer(Barometer_Data &data)
{
    if (!barometer_initialized)
    {
        return false;
    }
    if (barometer->doBaro(true))
    {
        data.temperature = barometer->temperature; // Returns temperature in C
        data.pressure = barometer->pressure;       // Returns pressure in Pascals
        data.altitude = barometer->altitude;       // Returns altitude in meters
        return true;
    }
    else
    {
        return false;
    }
}
bool Sensors::read_gps(Gps_Wrapper::Gps_Data &data)
{
    if (!gps_initialized)
    {
        return false;
    }
    Gps_Wrapper::Gps_Data temp_data;
    if (gps->read(temp_data))
    {
        data = temp_data;
        return true;
    }
    else
    {
        return false;
    }
}
bool Sensors::read_imu(IMU_Data &data)
{
    if (!imu_initialized)
    {
        return false;
    }
    sensors_event_t orientationData, angVelocityData, linearAccelData, magnetometerData, accelerometerData, gravityData;
    imu->getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
    imu->getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
    imu->getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    imu->getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
    imu->getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
    imu->getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

    data.acc_x = accelerometerData.acceleration.x;
    data.acc_y = accelerometerData.acceleration.y;
    data.acc_z = accelerometerData.acceleration.z;

    data.gyro_x = angVelocityData.gyro.x;
    data.gyro_y = angVelocityData.gyro.y;
    data.gyro_z = angVelocityData.gyro.z;

    data.orientation_x = orientationData.orientation.x;
    data.orientation_y = orientationData.orientation.y;
    data.orientation_z = orientationData.orientation.z;

    data.magnetic_x = magnetometerData.magnetic.x;
    data.magnetic_y = magnetometerData.magnetic.y;
    data.magnetic_z = magnetometerData.magnetic.z;

    data.lin_acc_x = linearAccelData.acceleration.x;
    data.lin_acc_y = linearAccelData.acceleration.y;
    data.lin_acc_z = linearAccelData.acceleration.z;

    data.g_acc_x = accelerometerData.acceleration.x;
    data.g_acc_y = accelerometerData.acceleration.y;
    data.g_acc_z = accelerometerData.acceleration.z;

    data.imu_temperature = imu->getTemp();
    return true;
}
