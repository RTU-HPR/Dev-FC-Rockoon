// WORKS
#include <Arduino.h>

#include <config.h>
#include <radio.h>
#include <sensors.h>
#include <actions.h>

// Objects
Config config;
Radio radio;
Sensors sensors;
Actions actions;

void setup()
{

    // Initialize PC serial
    Serial.begin(config.PC_BAUDRATE);
    if (config.WAIT_PC)
    {
        while (!Serial)
        {
            delay(500);
        }
    }

    // delay(3000);
    //  Set pins to correct mode and state
    pinMode(config.BUZZER_PIN, OUTPUT); // Buzzer
    digitalWrite(config.BUZZER_PIN, HIGH);
    pinMode(config.ATKABE_PIN, OUTPUT); // LED and Atkabe
    digitalWrite(config.ATKABE_PIN, LOW);

    // Initialize SPI
    SPI.begin(config.SPI_SCK, config.SPI_RX, config.SPI_TX);

    // Initialize I2C
    Wire.begin(config.WIRE_SDA, config.WIRE_SCL);

    // Initialize Radio
    radio.begin(config);

    // Initialize GPS
    config.gps_config.serial->begin(config.BAUD_RATE_GPS, SERIAL_8N1, config.GPS_RX, config.GPS_TX);
    if (!*(config.gps_config.serial))
    {
        Serial.println("GPS Serial not available");
        radio.send_radio("GPS Serial not available");
    }

    // Initialize sensors
    if (!sensors.init_sensors(config))
    {
        Serial.println("Sensor initialization failed");
        radio.send_radio("Sensor initialization failed");
    }
    else
    {
        Serial.println("Sensor initialization success");
    }
}

void loop()
{

    // Check for any received messages from Radio or PC
    if (radio.receive_command(radio.received))
    {
        // Print the received message to PC
        if (radio.received.radio_message)
        {
            Serial.print("RADIO COMMAND | RSSI: " + String(radio.received.rssi) + " | SNR: " + String(radio.received.snr) + " FREQUENCY: " + String(radio.received.frequency, 8) + " | MSG: ");
        }
        else
        {
            Serial.print("PC COMMAND | MSG: ");
        }
        Serial.println(radio.received.msg);

        // Execute the received command
        if (radio.received.msg != "" && radio.received.checksum_good && !radio.received.processed)
        {
            // Ping Pong command
            if (radio.received.msg == config.PING_MSG)
            {
                radio.send_radio(config.PONG_MSG);
            }
            // Single telemetry packet request command
            else if (radio.received.msg == config.DATA_REQUEST_MSG)
            {
                actions.data_request_toggled = true;
            }
            // Turn on telemetry packet sending command
            else if (radio.received.msg == config.DATA_SEND_ON_MSG)
            {
                // Send an acknowledgement message
                radio.send_radio(config.BFC_ACK_MSG + radio.received.msg);
                actions.data_send_toggled = true;
            }
            // Turn off telemetry packet sending command
            else if (radio.received.msg == config.DATA_SEND_OFF_MSG)
            {
                // Send an acknowledgement message
                radio.send_radio(config.BFC_ACK_MSG + radio.received.msg);
                actions.data_send_toggled = false;
            }
            // Atkabe command
            else if (radio.received.msg == config.ATKABE_MSG)
            {
                // Send an acknowledgement message
                radio.send_radio(config.BFC_ACK_MSG + radio.received.msg);
                actions.atkabe_toggled = true;
            }
            // Buzzer turn on command
            else if (radio.received.msg == config.BUZZER_ON_MSG)
            {
                actions.buzzer_toggled = true;
            }
            // Buzzer turn off command
            else if (radio.received.msg == config.BUZZER_OFF_MSG)
            {
                actions.buzzer_toggled = false;
            }
            else
            {
                String error_msg = "Invalid command";
                Serial.println(error_msg);
                radio.send_radio(config.BFC_ERR_MSG + error_msg);
            }
        }
    }

    // Run any actions
    actions.run_actions(config, radio, sensors);
    // Read all sensors
    sensors.read_sensors();
}
