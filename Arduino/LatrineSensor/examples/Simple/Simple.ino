#include <LatrineSensor.h>

LatrineSensor sensor = LatrineSensor(0, onFlushStart, onFlushEnd);

void setup(void)
{
  Serial.begin(115200);
}

void loop(void)
{
	sensor.Update(); // Call at least every second
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












