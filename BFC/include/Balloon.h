#pragma once
#include <Config.h>
#include <Sensors.h>
#include <Navigation.h>
#include <Actions.h>
#include <Communication.h>
#include <Logging.h>

class Balloon
{
private:
  /**
   * @brief Initialise the HardwareSerial, SPI, I2C communication busses
   */
  bool initCommunicationBusses();

public:
  Config config;
  Sensors sensors;
  Navigation navigation;
  Actions actions;
  Communication communication;
  Logging logging;

  /**
   * @brief Initialise the PFC
   */
  void begin();
};