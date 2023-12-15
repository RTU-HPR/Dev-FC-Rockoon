#include "radio.h"

void Radio::begin(Config &config)
{
    Serial.println("Initializing Radio");
    _radio = new RadioLib_Wrapper<radio_module>(nullptr, 5, "SX1268");
    if (!_radio->begin(config.radio_config))
    {
        Serial.println("Configuring LoRa failed");
        while (true)
        {
            Serial.println("Configuring LoRa failed");
            delay(1000);
        }
        return;
    }
}

// https://habitat.readthedocs.io/en/latest/ukhas_parser.html
// https://ukhas.org.uk/doku.php?id=communication:protocol
// DOES NOT FOLLOW THE PROTOCOL AS
// CURRENTLY THE CHECKSUM DOES NOT IGNORE THE $$ PREFIX
void Radio::msg_to_ukhas(String &msg, Config &config)
{
    String ukhas_msg = "$$" + config.BFC_CALLSIGN + ",";
    ukhas_msg += msg;

    // this will add both endline and dashstar
    _radio->add_checksum(ukhas_msg);

    msg = ukhas_msg;
}

// Sends the provided message using LoRa
bool Radio::send_radio(String msg, bool calculate_checksum)
{
    if (calculate_checksum)
    {
        _radio->add_checksum(msg);
    }
    bool status = _radio->transmit(msg);
    return status;
}

// Sends the provided message using LoRa
bool Radio::send_radio(String msg)
{
    return send_radio(msg, true);
}

// Check for received commands from Radio or PC
bool Radio::receive_command(RECEIVED_MESSAGE_STRUCTURE &received)
{
    // Check for any messages from Radio
    if (_radio->receive(received.msg, received.rssi, received.snr, received.frequency))
    {
        // Check if checksum matches
        if (_radio->check_checksum(received.msg))
        {
            received.checksum_good = true;
        }
        else
        {
            received.checksum_good = false;
        }
        received.processed = false;
        received.radio_message = true;

        return true;
    }

    // Check for any messages from PC
    if (Serial.available() > 0)
    {
        received.msg = Serial.readString();
        // Remove any line ending symbols
        received.msg.trim();

        received.checksum_good = true;
        received.processed = false;
        received.radio_message = false;

        return true;
    }

    return false;
}