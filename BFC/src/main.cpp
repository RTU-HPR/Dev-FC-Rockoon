#include <Balloon.h>

Balloon balloon;

// Performance monitoring
unsigned long lastLoopMillis = 0;
extern int loopTime; // Set as global variable
int loopTime = 0;

void setup()
{
  balloon.begin();
  Serial.println("Balloon setup complete");
  Serial.println();
}

void loop()
{
  lastLoopMillis = millis();
  balloon.actions.runAllActions(balloon.sensors, balloon.navigation, balloon.communication, balloon.logging, balloon.config);
  loopTime = millis() - lastLoopMillis;
}
