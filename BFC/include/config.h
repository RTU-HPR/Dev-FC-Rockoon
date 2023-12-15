#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RadioLib_wrapper.h>
#include <Gps_wrapper.h>

class Config
{
public:
    // PC
    const int PC_BAUDRATE = 115200;
    const bool WAIT_PC = false;

    // MISCELLANEOUS
    const int BUZZER_PIN = 42;
    const int ATKABE_PIN = 4;

    // GPS Serial
    const int GPS_RX = 9;
    const int GPS_TX = 8;
    const int BAUD_RATE_GPS = 9600; // this needs extra setup when changing
    // Gps config
    Gps_Wrapper::Gps_Config_UART gps_config{
        .config = {
            .timeout = 5000,                       // Time it takes for anything to timeout
            .measurement_rate = 500,               // how often measurement will be taken in ms
            .navigation_frequency = 2,             // how often tu updated navigation in s
            .dynamic_model = DYN_MODEL_AIRBORNE2g, // DYN_MODEL_AIRBORNE2g
            .com_settings = COM_TYPE_UBX,          // COM_TYPE_UBX
            .auto_pvt = true                       // for neo6m dont use this
        },
        .serial = &Serial1};

    // I2C
    const int WIRE_SCL = 18;
    const int WIRE_SDA = 17;

    // Wire usage
    TwoWire &MS5611_WIRE = Wire;
    TwoWire &BNO055_WIRE = Wire;

    // I2C addresses
    const int MS5611_ADDRESS_I2C = 0x77;
    const int BNO055_ADDRESS_I2C = 0x28;

    // MS5611 sensor ID
    const int MS5611_SENSOR_ID = 0; // 0 or 1

    // Sensor ID
    const int BNO055_SENSOR_ID = 55;

    //  SPI
    const int SPI_RX = 13; // MISO
    const int SPI_TX = 11; // MOSI
    const int SPI_SCK = 12;

// LoRa object
#define radio_module SX1268
#define radio_module_family Sx126x
#define radio_module_rf_switching Gpio

    // Radio module config
    RadioLib_Wrapper<radio_module>::Radio_Config radio_config{
        .frequency = 434, // Frequency
        .cs = 10,         // Chip select
        .dio0 = 48,       // Busy
        .dio1 = 2,        // Interrupt action
        .family = RadioLib_Wrapper<radio_module>::Radio_Config::Chip_Family::radio_module_family,
        .rf_switching = RadioLib_Wrapper<radio_module>::Radio_Config::Rf_Switching::radio_module_rf_switching,
        // If using GPIO pins for RX/TX switching, define pins used for RX and TX control
        .rx_enable = 36,
        .tx_enable = 35,
        .reset = 47,
        .sync_word = 0xF4,
        .tx_power = 14,
        .spreading = 10,
        .coding_rate = 7,
        .signal_bw = 62.5,
        .frequency_correction = false,
        .spi_bus = &SPI // SPI bus used by radio
    };

    // Misc messages
    const String BFC_CALLSIGN = "rtu_vip_bfc";
    const String BFC_ACK_MSG = "BFC_ACK:";
    const String BFC_ERR_MSG = "BFC_ERR:";

    // Receivable messages
    const String PING_MSG = "BFC_PING";
    const String ATKABE_MSG = "BFC_ATKABE";
    const String BUZZER_ON_MSG = "BFC_BUZZER_ON";
    const String BUZZER_OFF_MSG = "BFC_BUZZER_OFF";
    const String DATA_REQUEST_MSG = "BFC_DATA_REQUEST";
    const String DATA_SEND_ON_MSG = "BFC_DATA_SEND_ON";
    const String DATA_SEND_OFF_MSG = "BFC_DATA_SEND_OFF";

    // Sendable messages
    const String PONG_MSG = "BFC_PONG";
};
