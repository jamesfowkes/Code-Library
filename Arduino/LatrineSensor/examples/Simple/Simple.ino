#include <LatrineSensor.h>

LatrineSensor sensor = LatrineSensor(onFlushStart, onFlushEnd);

void setup(void)
{
  sensor.Setup(); // MUST be called to start counting!
  Serial.begin(115200);
}

void loop(void)
{
  delay(1000);
  sensor.Update(); // Call once every second
}

/*
 * Two callback functions for the sensor
 * These are passed to the sensor class when instantiating.
 * onFlushStart is called once when flush start is detected.
 * onFlushEnd is called once when flush end is detected. Flush duration is pass as parameter
 */
void onFlushStart(void)
{
  Serial.println("Flush started...");
}

void onFlushEnd(uint16_t durationinSeconds)
{
  Serial.print("Flush ended (");
  Serial.print(durationinSeconds);
  Serial.println(" seconds)");
}














