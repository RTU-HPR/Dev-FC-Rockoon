#include <Actions.h>

void Actions::runContinousActions(Sensors &sensors, Navigation &navigation, Communication &communication, Logging &logging, Config &config)
{
  // Receive any commands
  if (commandReceiveActionEnabled)
  {
    runCommandReceiveAction(communication, logging, config);
  }

  // Check if the communication cycle should be started
  if (getCommunicationCycleStartActionEnabled)
  {
    runGetCommunicationCycleStartAction(navigation, config);
  }

  // Run the sensor action
  if (sensorActionEnabled)
  {
    runSensorAction(sensors);
  }

  // Run the GPS action
  if (gpsActionEnabled)
  {
    runGpsAction(navigation);
  }

  // Run the logging action
  if (loggingActionEnabled)
  {
    runLoggingAction(logging, navigation, sensors);
  }
}

void Actions::runCommandReceiveAction(Communication &communication, Logging &logging, Config &config)
{
  byte *msg = new byte[256];
  uint16_t msg_length = 0;
  float rssi = 1;
  float snr = 0;
  double frequency = 0;
  bool checksum_good = false;

  // Check for any messages from Radio
  if (communication._radio->receive_bytes(msg, msg_length, rssi, snr, frequency))
  {
    // Check if checksum matches
    if (check_crc_16_cciit_of_ccsds_packet(msg, msg_length))
    {
      checksum_good = true;
    }
  }
  else
  {
    // Free memory after no message has been received
    delete[] msg; // VERY IMPORTANT, otherwise a significant memory leak will occur
    return;
  }

  // Check if the checksum is good
  if (checksum_good)
  {
    // Print the received message
    Serial.print("RADIO COMMAND | RSSI: " + String(rssi) + " | SNR: " + String(snr) + " FREQUENCY: " + String(frequency, 8) + " | MSG: ");

    for (int i = 0; i < msg_length; i++)
    {
      Serial.print(msg[i], HEX);
      Serial.print(" ");
    }
    Serial.println();

    // A CCSDS Telecommand packet was received
    uint16_t apid = 0;
    uint16_t sequence_count = 0;
    byte *packet_data = new byte[msg_length];
    uint16_t packet_id = 0;
    uint16_t packet_data_length = 0;
    parse_ccsds_telecommand(msg, apid, sequence_count, packet_id, packet_data, packet_data_length);

    Serial.println("APID: " + String(apid));
    Serial.println("Sequence count: " + String(sequence_count));
    Serial.println("Packet data length: " + String(packet_data_length));
    Serial.println("Packet ID: " + String(packet_id));

    // Set the action flag according to the received command
    if (packet_id == 1000)
    {
      completeDataRequestActionEnabled = true;
    }
    else if (packet_id == 1001)
    {
      infoErrorRequestActionEnabled = true;
    }
    else if (packet_id == 1002)
    {
      formatStorageActionEnabled = true;
    }
    else
    {
      Serial.println("No mathcing command found");
    }

    // Free memory after the packet data has been parsed
    delete[] packet_data; // VERY IMPORTANT, otherwise a significant memory leak will occur
  }
  else if (!checksum_good)
  {
    Serial.println("Command with invalid checksum received: ");
    for (int i = 0; i < msg_length; i++)
    {
      Serial.print(msg[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
  }

  // Free memory after the message has been parsed
  delete[] msg; // VERY IMPORTANT, otherwise a significant memory leak will occur
}

void Actions::runSensorAction(Sensors &sensors)
{
  // Read all sensors
  sensors.readSensors();
}

void Actions::runGpsAction(Navigation &navigation)
{
  navigation.readGps(navigation.navigation_data);
}

void Actions::runLoggingAction(Logging &logging, Navigation &navigation, Sensors &sensors)
{
  // Log the data to the sd card
  String packet = createLoggablePacket(sensors, navigation);
  logging.writeTelemetry(packet);
}

void Actions::runGetCommunicationCycleStartAction(Navigation &navigation, Config &config)
{
  // Serial.println("GPS epoch time: " + String(navigation.navigation_data.gps.epoch_time));
  if (millis() - lastCommunicationCycle <= 3000)
  {
    // Serial.println("Communication cycle already started: " + String(millis() - lastCommunicationCycle));
    return;
  }
  if (navigation.navigation_data.gps.epoch_time == 0)
  {
    return;
  }

  int comm_cycle_interval_sec = config.COMMUNICATION_CYCLE_INTERVAL / 1000;
  if (navigation.navigation_data.gps.second % comm_cycle_interval_sec == 0)
  {
    lastCommunicationCycle = millis();
    dataEssentialSendActionEnabled = true;
    Serial.println("New communication cycle started: " + String(lastCommunicationCycle) + " " + String(navigation.navigation_data.gps.second));
  }
}

String Actions::createLoggablePacket(Sensors &sensors, Navigation &navigation)
{
  String packet = "";
  // UKHAS
  packet += String(loggable_packed_id);
  packet += ",";
  packet += String(navigation.navigation_data.gps.hour);
  packet += ":";
  packet += String(navigation.navigation_data.gps.minute);
  packet += ":";
  packet += String(navigation.navigation_data.gps.second);
  packet += ",";
  packet += String(navigation.navigation_data.gps.lat, 7);
  packet += ",";
  packet += String(navigation.navigation_data.gps.lng, 7);
  packet += ",";
  packet += String(navigation.navigation_data.gps.altitude, 2);
  packet += ",";
  packet += String(navigation.navigation_data.gps.satellites);
  packet += ",";
  packet += String(sensors.data.onBoardBaro.pressure);
  packet += ",";
  packet += String(navigation.navigation_data.gps.speed, 2);
  packet += ",";
  packet += String(sensors.data.onBoardBaro.altitude, 2);
  // CUSTOM
  // IMU
  packet += ",";
  packet += String(sensors.data.imu.acc_x, 4);
  packet += ",";
  packet += String(sensors.data.imu.acc_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.acc_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.g_acc_x, 4);
  packet += ",";
  packet += String(sensors.data.imu.g_acc_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.g_acc_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.gyro_x, 4);
  packet += ",";
  packet += String(sensors.data.imu.gyro_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.gyro_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.lin_acc_x, 4);
  packet += ",";
  packet += String(sensors.data.imu.lin_acc_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.lin_acc_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.magnetic_x, 4); 
  packet += ",";
  packet += String(sensors.data.imu.magnetic_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.magnetic_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.orientation_x, 4);
  packet += ",";
  packet += String(sensors.data.imu.orientation_y, 4);
  packet += ",";
  packet += String(sensors.data.imu.orientation_z, 4);
  packet += ",";
  packet += String(sensors.data.imu.imu_temperature, 2);
  // MS56XX
  packet += ",";
  packet += String(sensors.data.onBoardBaro.temperature, 2);
  // GPS
  packet += String(navigation.navigation_data.gps.epoch_time);
  packet += ",";
  packet += String(navigation.navigation_data.gps.heading);
  packet += ",";
  packet += String(navigation.navigation_data.gps.pdop);
  packet += ",";
  // MISC
  packet += String(millis());
  packet += ",";
  packet += String(loopTime);

  loggable_packed_id++;

  return packet;
}