#include "Balloon.h"

bool Balloon::initCommunicationBusses()
{
  bool success = true;

  // GPS Serial
  config.gps_config.serial->begin(config.BAUD_RATE_GPS, SERIAL_8N1, config.GPS_RX, config.GPS_TX);

  // Wire0
  if (Wire.begin(config.WIRE0_SDA, config.WIRE0_SCL))
  {
    Serial.println("Wire0 communication bus initialized");
  }
  else
  {
    String errorString = "Wire0 begin fail";
    logging.recordError(errorString);
    success = false;
  }

  // SPI
  SPI.begin(config.SPI0_SCK, config.SPI0_RX, config.SPI0_TX);
  Serial.println("SPI0 communication bus initialized");

  return success;
}

void Balloon::begin()
{
  // Initialize PC serial
  Serial.begin(config.PC_BAUDRATE);
  if (config.WAIT_PC)
  {
    while (!Serial)
    {
      delay(100);
    }
  }

  Serial.println("PC Serial initialized");

  // Initialize the communication busses
  if (initCommunicationBusses())
  {
    Serial.println("All communication busses initialized successfully");
  }
  else
  {
    Serial.println("Error in initializing communication busses");
  }

  // Set the pyro channels to output and pull them low
  pinMode(config.ATKABE_PIN, OUTPUT);
  digitalWrite(config.ATKABE_PIN, LOW);

  Serial.println("Atkabe pin set to output and pulled low");

  // // Initialize the SD card
  // if (!logging.begin(config))
  // {
  //   String errorString = "SD begin fail";
  //   logging.recordError(errorString);
  // }
  // else
  // {
  //   Serial.println("SD card initialized successfully");

  //   // Read config file
  //   if (!logging.readConfig(config))
  //   {
  //     String infoString = "New config file created";
  //     logging.recordInfo(infoString);
  //   }
  //   else
  //   {
  //     Serial.println("Config file read successfully");
  //   }
  // }

  // Initialise the radio
  if (!communication.beginRadio(config))
  {
    String errorString = "Radio begin fail";
    logging.recordError(errorString);
  }
  else
  {
    Serial.println("Radio initialized successfully");
  }

  Serial.println();

  // Send inital error string
  if (!logging.infoErrorQueueEmpty())
  {
    String infoError = "";
    while (!logging.infoErrorQueueEmpty())
    {
      infoError += logging.readFromInfoErrorQueue();
      if (!logging.infoErrorQueueEmpty())
      {
        infoError += ",";
      }
    }
    Serial.println("INITAL INFO/ERROR: " + infoError);
    Serial.println();
    logging.writeError(infoError);
    communication.sendError(infoError);
  }

  // Initialise all sensors
  if (!sensors.begin(logging, config))
  {
    Serial.println("Error initializing sensors");
  }
  else
  {
    Serial.println("Sensors initialized successfully");
  }

  // Send inital error string
  if (!logging.infoErrorQueueEmpty())
  {
    String infoError = "";
    while (!logging.infoErrorQueueEmpty())
    {
      infoError += logging.readFromInfoErrorQueue();
      if (!logging.infoErrorQueueEmpty())
      {
        infoError += ",";
      }
    }
    Serial.println("SENSOR ERROR: " + infoError);
    Serial.println();
    logging.writeError(infoError);
    communication.sendError(infoError);
  }

  // Initialise GPS
  if (!navigation.beginGps(config.gps_config))
  {
    String errorString = "GPS begin fail";
    Serial.println(errorString);
    logging.recordError(errorString);
    communication.sendError(errorString);
  }
  else
  {
    Serial.println("GPS initialized successfully");
  }

  Serial.println();
}