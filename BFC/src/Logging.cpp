#include <Logging.h>

bool Logging::begin(Config &config)
{
  // Initialize the info/error queue
  infoErrorQueue = new cppQueue(sizeof(Info_Error_Message), INFO_ERROR_QUEUE_SIZE, QUEUE_IMPLEMENTATION, true);

  // // Initialize the SD card
  // if (!sd_card_wrapper.init(config.sd_card_config))
  // {
  //   return false;
  // }

  return true;
}

bool Logging::formatSdCard(Config &config)
{
  // if (!sd_card_wrapper.get_initialized())
  // {
  //   return false;
  // }

  // if (!sd_card_wrapper.clean_storage(config.sd_card_config))
  // {
  //   Serial.println("SD card format failed!");
  //   return false;
  // }
  return true;
}

bool Logging::addToInfoErrorQueue(String &message)
{
  // Check if the size of message is within the limit and truncate if necessary
  if (message.length() > INFO_ERROR_MAX_LENGTH)
  {
    message = message.substring(0, INFO_ERROR_MAX_LENGTH);
  }
  // Convert the message from string to char array
  char message_char[INFO_ERROR_MAX_LENGTH];
  message.toCharArray(message_char, INFO_ERROR_MAX_LENGTH);

  // Put the char array into the struct
  Info_Error_Message infoError;
  strcpy(infoError.message, message_char);
  
  // Push the struct to the queue
  if (!infoErrorQueue->push(&infoError))
  {
    return false;
  }
  return true;
}

String Logging::readFromInfoErrorQueue()
{
  // Create a record and pop it
  Info_Error_Message infoError;
  if (!infoErrorQueue->pop(&infoError))
  {
    return "";
  };
  return String(infoError.message);
}

bool Logging::recordInfo(String &info)
{
  // Add the info to the info/error queue
  addToInfoErrorQueue(info);

  // Write the string to the info file
  // if (!writeInfo(info))
  // {
  //   return false;
  // }

  return true;
}

bool Logging::recordError(String &error)
{
  // Add the error to the info/error queue
  addToInfoErrorQueue("!" + error);

  // // Write the string to the error file
  // if (!writeError(error))
  // {
  //   return false;
  // }

  return true;
}

bool Logging::infoErrorQueueEmpty()
{
  return infoErrorQueue->isEmpty();
}

// Function to parse a string containing comma-separated values as strings
void Logging::parseString(String &input, String *values, size_t maxSize)
{
  int startIndex = 0;
  int endIndex = input.indexOf(',');
  size_t index = 0;

  while (endIndex != -1 && index < maxSize)
  {
    // Extract each substring
    values[index] = input.substring(startIndex, endIndex);

    // Move to the next substring
    startIndex = endIndex + 1;
    endIndex = input.indexOf(',', startIndex);
    index++;
  }

  // Process the last substring
  if (index < maxSize)
  {
    values[index] = input.substring(startIndex);
  }
}

bool Logging::writeTelemetry(String &data)
{
  // if (!sd_card_wrapper.get_initialized())
  // {
  //   return false;
  // }

  // if (!sd_card_wrapper.write_data(data))
  // {
  //   Serial.println("Telemetry write failed!");
  //   return false;
  // }
  return true;
}

bool Logging::writeInfo(String &data)
{
  // if (!sd_card_wrapper.get_initialized())
  // {
  //   return false;
  // }

  // data = String(millis()) + "," + data.substring(1);

  // if (!sd_card_wrapper.write_info(data))
  // {
  //   Serial.println("Info write failed!");
  //   return false;
  // }
  return true;
}

bool Logging::writeError(String &data)
{
  // if (!sd_card_wrapper.get_initialized())
  // {
  //   return false;
  // }

  // data = String(millis()) + "," + data.substring(1);

  // if (!sd_card_wrapper.write_error(data))
  // {
  //   Serial.println("Error write failed!");
  //   return false;
  // }
  return true;
}