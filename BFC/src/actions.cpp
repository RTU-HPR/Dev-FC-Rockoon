#include <actions.h>

void Actions::run_actions(Config &config, Radio &radio, Sensors &sensors)
{
    // Atkabe
    if (atkabe_toggled)
    {
        toggle_atkabe(config);
    }

    // Buzzer
    if (buzzer_toggled)
    {
        toggle_buzzer(config);
    }
    else
    {
        // If buzzer is not toggled, make sure it is turned off
        digitalWrite(config.BUZZER_PIN, HIGH);
    }

    // Telemetry
    if (data_request_toggled)
    {
        data_request(config, radio, sensors);
    }

    if (data_send_toggled)
    {
        data_send(config, radio, sensors);
    }
}

// Follows recommended UKHAS format as closely as possible
void Actions::update_telemetry_packet(const Sensors::SENSOR_DATA &data)
{
    telemetry_packet = "";
    telemetry_packet += String(telemetry_packet_id);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.hour);
    telemetry_packet += ":";
    telemetry_packet += String(data.gps_data.minute);
    telemetry_packet += ":";
    telemetry_packet += String(data.gps_data.second);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.lat, 7);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.lng, 7);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.altitude, 2);
    telemetry_packet += ",";
    telemetry_packet += String(data.barometer_data.temperature, 2);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.satellites);
    telemetry_packet += ",";
    telemetry_packet += String(data.barometer_data.pressure);
    telemetry_packet += ",";
    telemetry_packet += String(data.gps_data.speed, 2);
    telemetry_packet += ",";
    telemetry_packet += String(data.barometer_data.altitude, 2);

    telemetry_packet_id++;
}

void Actions::send_telemetry(Config &config, Radio &radio, Sensors &sensors)
{
    update_telemetry_packet(sensors.sensor_data);

    radio.msg_to_ukhas(telemetry_packet, config);
    // Don't calculate checksum, as that is already done in msg_to_ukhas
    Serial.println(telemetry_packet);
    if (!radio.send_radio(telemetry_packet, false))
    {
        Serial.println("Failed to send telemetry packet");
    }
}

void Actions::data_request(Config &config, Radio &radio, Sensors &sensors)
{
    send_telemetry(config, radio, sensors);
    data_request_toggled = false;
}

void Actions::data_send(Config &config, Radio &radio, Sensors &sensors)
{
    // Can only send if seconds' are divisible by 10
    if (millis() % 10000 == 0)
    {
        send_telemetry(config, radio, sensors);
    }
}

void Actions::toggle_atkabe(Config &config)
{

    digitalWrite(config.ATKABE_PIN, HIGH);
    delay(1000);
    digitalWrite(config.ATKABE_PIN, HIGH);
    atkabe_toggled = false;
}

void Actions::toggle_buzzer(Config &config)
{
    // Save the toggle start millis
    if (buzzer_last_change_millis == 0)
    {
        buzzer_last_change_millis == millis();
        digitalWrite(config.BUZZER_PIN, LOW);
    }

    // Flip the state, if the duration has passed
    if (millis() - buzzer_last_change_millis >= BUZZER_TOGGLE_DURATION)
    {
        digitalWrite(config.BUZZER_PIN, !digitalRead(config.BUZZER_PIN));
        buzzer_last_change_millis = millis();
    }
}
