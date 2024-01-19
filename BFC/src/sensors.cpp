#include "Sensors.h"

bool Sensors::begin(Logging &logging, Config &config)
{
  bool success = true;
  // Change analogRead resolution
  // This is needed for higher accuracy battery voltage and thermistor readings
  analogReadResolution(12);

  // MAIN BOARD
  // Initialize MS56XX
  if (!beginOnBoardBaro(config))
  {
    String errorString = "Onboard barometer begin fail";
    logging.recordError(errorString);
    success = false;
  }
  else
  {
    Serial.println("Onboard barometer initialization complete");
  }

  // Initialize IMU
  if (!beginImu(config))
  {
    String errorString = "IMU begin fail";
    logging.recordError(errorString);
    success = false;
  }
  else
  {
    Serial.println("IMU initialization complete");
  }

  Serial.println();

  return success;
}

void Sensors::readSensors()
{
  // Read MS56XX
  readOnBoardBaro();

  // Read IMU
  readImu();
}


bool Sensors::beginOnBoardBaro(Config &config)
{
  if (!_onBoardBaro.begin(config.ms56xx_config))
  {
    return false;
  }
  return true;
}

bool Sensors::beginImu(Config &config)
{
  imu = new Adafruit_BNO055(config.imu_config.sensor_id, config.imu_config.i2c_address, config.imu_config.wire);
  // good idead to add a specified operating mode (update frquency etc.)
  if (!imu->begin())
  {
    return false;
  }
  return true;
}

bool Sensors::readOnBoardBaro()
{
  if (_onBoardBaro.read(data.onBoardBaro))
  {
    return true;
  }
  Serial.println("Onboard barometer reading failed!");
  return false;
}

bool Sensors::readImu()
{
  sensors_event_t orientationData, angVelocityData, linearAccelData, magnetometerData, accelerometerData, gravityData;
  imu->getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER);
  imu->getEvent(&angVelocityData, Adafruit_BNO055::VECTOR_GYROSCOPE);
  imu->getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
  imu->getEvent(&magnetometerData, Adafruit_BNO055::VECTOR_MAGNETOMETER);
  imu->getEvent(&linearAccelData, Adafruit_BNO055::VECTOR_LINEARACCEL);
  imu->getEvent(&gravityData, Adafruit_BNO055::VECTOR_GRAVITY);

  data.imu.acc_x = accelerometerData.acceleration.x;
  data.imu.acc_y = accelerometerData.acceleration.y;
  data.imu.acc_z = accelerometerData.acceleration.z;

  data.imu.gyro_x = angVelocityData.gyro.x;
  data.imu.gyro_y = angVelocityData.gyro.y;
  data.imu.gyro_z = angVelocityData.gyro.z;

  data.imu.orientation_x = orientationData.orientation.x;
  data.imu.orientation_y = orientationData.orientation.y;
  data.imu.orientation_z = orientationData.orientation.z;

  data.imu.magnetic_x = magnetometerData.magnetic.x;
  data.imu.magnetic_y = magnetometerData.magnetic.y;
  data.imu.magnetic_z = magnetometerData.magnetic.z;

  data.imu.lin_acc_x = linearAccelData.acceleration.x;
  data.imu.lin_acc_y = linearAccelData.acceleration.y;
  data.imu.lin_acc_z = linearAccelData.acceleration.z;

  data.imu.g_acc_x = accelerometerData.acceleration.x;
  data.imu.g_acc_y = accelerometerData.acceleration.y;
  data.imu.g_acc_z = accelerometerData.acceleration.z;

  data.imu.imu_temperature = imu->getTemp();
  return true;
}