#pragma once
#include <Config.h>
#include <Logging.h>

class Sensors
{
private:
  /**
   * @brief Object representing the MS56XX sensor.
   *
   * This class provides functionality to interface with the MS56XX sensor.
   */
  MS56XX _onBoardBaro;

  /**
   * @brief Object representing the IMU sensor.
   *
   * This class provides functionality to interface with the IMU sensor.
   */
  Adafruit_BNO055 *imu;

  /**
   * @brief Structure to hold IMU sensor data.
   */
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

public:
  String sensorErrorString = "";

  /**
   * @brief Structure to store all sensor data
   */
  struct SENSOR_DATA
  {
    MS56XX::MS56XX_Data onBoardBaro;
    IMU_Data imu;
  };

  SENSOR_DATA data;

  /**
   * @brief Initializes all sensor objects and gets them ready to be used.
   *
   * @param config The configuration object containing the sensor settings.
   * @return True if the initialization is successful, false otherwise.
   */
  bool begin(Logging &logging, Config &config);

  /**
   * @brief Reads data from all sensors.
   *
   */
  void readSensors();

  /**
   * Initializes the on-board barometer sensor.
   *
   * @param config The configuration object.
   * @return True if the sensor initialization is successful, false otherwise.
   */
  bool beginOnBoardBaro(Config &config);

  /**
   * Initializes the IMU sensor.
   *
   * @param config The configuration object.
   * @return True if the initialization is successful, false otherwise.
   */
  bool beginImu(Config &config);

  /**
   * @brief Reads data from the onboard barometer.
   *
   * @return true if the data was successfully read, false otherwise.
   */
  bool readOnBoardBaro();

  /**
   * @brief Reads data from the IMU.
   *
   * @return true if the data was successfully read, false otherwise.
   */
  bool readImu();
};