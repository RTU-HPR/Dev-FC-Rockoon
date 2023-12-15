#pragma once
#include <Arduino.h>

#include <config.h>
#include <radio.h>
#include <sensors.h>
#include <Gps_wrapper.h>

class Actions
{
private:
    // Action duration
    const int ATKABE_TOGGLE_DURATION = 3000;
    const int BUZZER_TOGGLE_DURATION = 250;

    // Action timing
    unsigned long atkabe_start_millis = 0;
    unsigned long buzzer_last_change_millis = 0;
    unsigned long data_send_start_millis = 0;

    // Function declarations
    void toggle_atkabe(Config &config);
    void toggle_buzzer(Config &config);

    void update_telemetry_packet(const Sensors::SENSOR_DATA &data);
    void send_telemetry(Config &config, Radio &radio, Sensors &sensors);
    void data_request(Config &config, Radio &radio, Sensors &sensors);
    void data_send(Config &config, Radio &radio, Sensors &sensors);

    // Telemetry
    int telemetry_packet_id = 1;
    String telemetry_packet = "";

public:
    // Action flags
    bool atkabe_toggled = false;
    bool buzzer_toggled = false;
    bool data_request_toggled = false;
    bool data_send_toggled = false;

    // Function declarations
    void run_actions(Config &config, Radio &radio, Sensors &sensors);
};