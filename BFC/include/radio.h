#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <RadioLib_wrapper.h>
#include "config.h"

class Radio
{
  private:
    
  public:
    RadioLib_Wrapper<radio_module> *_radio;

    struct RECEIVED_MESSAGE_STRUCTURE
    {
        String msg;
        float rssi;
        float snr;
        double frequency;
        bool processed;
        bool checksum_good;
        bool radio_message;
    };
    RECEIVED_MESSAGE_STRUCTURE received;

    void begin(Config &config);
    void msg_to_ukhas(String &msg, Config &config);
    bool send_radio(String msg, bool calculate_checksum);
    bool send_radio(String msg);
    bool receive_command(RECEIVED_MESSAGE_STRUCTURE &received);
};
