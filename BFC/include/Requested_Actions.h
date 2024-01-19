#include <Actions.h>

void Actions::runRequestedActions(Sensors &sensors, Navigation &navigation, Communication &communication, Logging &logging, Config &config)
{
  // Check if the communication cycle is within the response send time
  if (!(millis() - lastCommunicationCycle >= config.COMMUNICATION_RESPONSE_SEND_TIME && millis() - lastCommunicationCycle <= config.COMMUNICATION_ESSENTIAL_DATA_SEND_TIME))
  {
    return;
  }

  if (infoErrorRequestActionEnabled)
  {
    runInfoErrorSendAction(communication, logging, navigation, config);
  }
  if (completeDataRequestActionEnabled)
  {
    runCompleteDataRequestAction(sensors, navigation, communication, config);
  }
  if (formatStorageActionEnabled)
  {
    runFormatStorageAction(communication, logging, navigation, config);
  }
}

// Timed and Requested actions
void Actions::runInfoErrorSendAction(Communication &communication, Logging &logging, Navigation &navigation, Config &config)
{
  String msg_str = logging.readFromInfoErrorQueue();
  if (msg_str == "")
  {
    infoErrorRequestActionEnabled = false;
    Serial.println("No info errors to send");
    return;
  }

  uint16_t ccsds_packet_length;
  byte *ccsds_packet = create_ccsds_telemetry_packet(config.BFC_INFO_ERROR_RESPONSE, infoErrorResponseId, navigation.navigation_data.gps.epoch_time, 0, msg_str, ccsds_packet_length);

  // Send packet
  if (!communication.sendRadio(ccsds_packet, ccsds_packet_length))
  {
    // Do nothing for now
  }
  infoErrorResponseId++;
  infoErrorRequestActionEnabled = false;
  // Free memory
  delete[] ccsds_packet; // VERY IMPORTANT, otherwise a significant memory leak will occur
}

void Actions::runCompleteDataRequestAction(Sensors &sensors, Navigation &navigation, Communication &communication, Config &config)
{
  String msg_str = createCompleteDataPacket(sensors, navigation, config);

  uint16_t ccsds_packet_length;
  byte *ccsds_packet = create_ccsds_telemetry_packet(config.BFC_COMPLETE_DATA_RESPONSE, completeDataResponseId, navigation.navigation_data.gps.epoch_time, 0, msg_str, ccsds_packet_length);

  // Send packet
  if (!communication.sendRadio(ccsds_packet, ccsds_packet_length))
  {
    // Free memory after the packet has been sent
    delete[] ccsds_packet; // VERY IMPORTANT, otherwise a significant memory leak will occur
    return;
  }
  completeDataResponseId++;
  completeDataRequestActionEnabled = false;
  // Free memory
  delete[] ccsds_packet; // VERY IMPORTANT, otherwise a significant memory leak will occur
}

String Actions::createCompleteDataPacket(Sensors &sensors, Navigation &navigation, Config &config)
{
  String packet = "";
  packet += String(sensors.data.onBoardBaro.temperature, 1);
  packet += ",";
  packet += String(sensors.data.onBoardBaro.pressure);

  return packet;
}

void Actions::runFormatStorageAction(Communication &communication, Logging &logging, Navigation &navigation, Config &config)
{
  // Format the SD card
  bool success;
  if (logging.formatSdCard(config))
  {
    success = true;
  }
  else
  {
    success = false;
  }

  // Send the response
  String msg_str = String(success);

  uint16_t ccsds_packet_length;
  byte *ccsds_packet = create_ccsds_telemetry_packet(config.BFC_COMPLETE_DATA_RESPONSE, formatResponseId, navigation.navigation_data.gps.epoch_time, 0, msg_str, ccsds_packet_length);

  // Send packet
  if (!communication.sendRadio(ccsds_packet, ccsds_packet_length))
  {
    // Free memory after the packet has been sent
    delete[] ccsds_packet; // VERY IMPORTANT, otherwise a significant memory leak will occur
    return;
  }
  formatResponseId++;
  formatStorageActionEnabled = false;
  // Free memory
  delete[] ccsds_packet; // VERY IMPORTANT, otherwise a significant memory leak will occur
}