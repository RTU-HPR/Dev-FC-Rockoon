#pragma once
#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <MS56XX.h>
#include <Gps_wrapper.h>
#include <config.h>

class Sensors
{
public:
    struct Barometer_Data
    {
        // MS56XX
        float temperature;
        int pressure;
        float altitude;
    };
    struct IMU_Data
    {
        // BNO055
        float acc_x;
        float acc_y;
        float acc_z;

        float gyro_x;
        float gyro_y;
        float gyro_z;

        float orientation_x;
        float orientation_y;
        float orientation_z;

        float magnetic_x;
        float magnetic_y;
        float magnetic_z;

        float lin_acc_x;
        float lin_acc_y;
        float lin_acc_z;

        float g_acc_x;
        float g_acc_y;
        float g_acc_z;

        float imu_temperature;
    };
    struct SENSOR_DATA
    {
        Barometer_Data barometer_data;
        IMU_Data imu_data;
        Gps_Wrapper::Gps_Data gps_data;
    };

private:
    MS56XX *barometer;
    Adafruit_BNO055 *imu;
    Gps_Wrapper *gps;

    bool barometer_initialized = false;
    bool imu_initialized = false;
    bool gps_initialized = false;

    bool begin_barometer(int i2c_address, uint ms56xx_type);
    bool begin_imu(TwoWire &wire, int sensor_id, int i2c_address);
    bool begin_gps(const Gps_Wrapper::Gps_Config_UART &gps_config);

    bool read_barometer(Barometer_Data &data);
    bool read_imu(IMU_Data &data);
    bool read_gps(Gps_Wrapper::Gps_Data &data);

public:
    SENSOR_DATA sensor_data;

    // reads all data and stores in global variable
    void read_sensors();
    bool init_sensors(const Config &config);
};