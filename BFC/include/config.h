#pragma once
// Main libraries
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
// #include <LittleFS.h>
// #include <SDFS.h>
#include <cppQueue.h>

// Public sensor libraries
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Our wrappers
#include <RadioLib_wrapper.h>
#include <ranging_wrapper.h>
#include <Gps_Wrapper.h>
// #include <Sd_card_wrapper.h>

// Our functions
#include <Ccsds_packets.h>

// Our sensor libaries
#include <MS56XX.h>
#include <Adc_Voltage.h>

// Used radio module
#define radio_module SX1268
#define radio_module_family Sx126x
#define radio_module_rf_switching Gpio

class Config
{
public:
  bool WAIT_PC = false;
  const bool LOG_TO_STORAGE = true;

  // 433 MHz LoRa
  RadioLib_Wrapper<radio_module>::Radio_Config radio_config{
      .frequency = 434.5,
      .cs = 2,
      .dio0 = 3,
      .dio1 = 5,
      .family = RadioLib_Wrapper<radio_module>::Radio_Config::Chip_Family::radio_module_family,
      .rf_switching = RadioLib_Wrapper<radio_module>::Radio_Config::Rf_Switching::radio_module_rf_switching,
      .rx_enable = 36,
      .tx_enable = 35,
      .reset = 47,
      .sync_word = 0xF4,
      .tx_power = 10,
      .spreading = 11,
      .coding_rate = 8,
      .signal_bw = 62.5,
      .frequency_correction = false,
      .spi_bus = &SPI,
  };

  // GPS
  Gps_Wrapper::Gps_Config_UART gps_config{
      .config = {
          .timeout = 5000,                       // Time it takes for anything to timeout
          .measurement_rate = 500,               // how often measurement will be taken in ms
          .navigation_frequency = 2,             // how often tu updated navigation in s
          .dynamic_model = DYN_MODEL_AIRBORNE2g, // DYN_MODEL_AIRBORNE2g
          .com_settings = COM_TYPE_UBX,          // COM_TYPE_UBX
          .auto_pvt = true
      },
      .serial = &Serial1};

  // MS56XX
  MS56XX::MS56XX_Config ms56xx_config = {
      .wire = &Wire1,
      .i2c_address = MS56XX::MS56XX_I2C_ADDRESS::I2C_0x77, // or 0x76
      .ms56xx_type = MS56XX::MS56XX_TYPE::MS5607,          // or MS5611
      .oversampling = MS56XX::MS56XX_OVERSAMPLING::OSR_STANDARD,
  };

  // IMU
  struct IMU_Config
  {
    TwoWire *wire;
    int i2c_address;
    int sensor_id;
  };
  IMU_Config imu_config = {
      .wire = &Wire,
      .i2c_address = 0x28,
      .sensor_id = 55,
  };

// Info/error queue
#define QUEUE_IMPLEMENTATION FIFO
#define INFO_ERROR_QUEUE_SIZE 20
#define INFO_ERROR_MAX_LENGTH 100

  // Watchdog
  const int WATCHDOG_TIMER = 8000; // Max is 8400

  // GPS Serial
  const int GPS_RX = 9;
  const int GPS_TX = 8;
  const int BAUD_RATE_GPS = 9600;

  // Wire0
  const int WIRE0_SCL = 18;
  const int WIRE0_SDA = 17;

  // SPI0
  const int SPI0_RX = 13;
  const int SPI0_TX = 11;
  const int SPI0_SCK = 12;

  // logging
  const int PC_BAUDRATE = 115200;

  // Buzzer
  const int BUZZER_PIN = 42;

  // Atkabe
  const int ATKABE_PIN = 19;
  const int ATKABE_ON_TIME = 5000;

  // Actions
  const int TIMED_ACTION_INITIAL_DELAY = 10000;

  // Data send action interval
  const int COMMUNICATION_CYCLE_INTERVAL = 12000;
  const int COMMUNICATION_RESPONSE_SEND_TIME = 5000;
  const int COMMUNICATION_ESSENTIAL_DATA_SEND_TIME = 9000;

  // Sendable commands
  const int BFC_ESSENTIAL_DATA_RESPONSE = 200;
  const int BFC_COMPLETE_DATA_RESPONSE = 201;
  const int BFC_INFO_ERROR_RESPONSE = 202;
  const int BFC_FORMAT_RESPONSE = 203;
  const int BFC_ATKABe_RESPONSE = 204;

  // Receiveable commands
  const int BFC_COMPLETE_DATA_REQUEST = 2000;
  const int BFC_INFO_ERROR_REQUEST = 2001;
  const int BFC_FORMAT_REQUEST = 2002;
  const int BFC_ATKABE_REQUEST = 2003;
};