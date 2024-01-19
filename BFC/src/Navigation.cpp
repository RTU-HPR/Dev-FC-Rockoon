#include <Navigation.h>

bool Navigation::beginGps(const Gps_Wrapper::Gps_Config_UART &gps_config)
{
  _gps = Gps_Wrapper(nullptr, "GPS");
  unsigned long start = millis();
  while (!_gps.begin(gps_config))
  {
    if (millis() - start > 10000)
    {
      return false;
    }
  }

  return true;
}

bool Navigation::readGps(NAVIGATION_DATA &navigation_data)
{
  bool position_valid = false;
  bool time_valid = false;
  if (!_gps.read(navigation_data.gps, position_valid, time_valid))
  {
    return false;
  }
  return true;
}