#include "unity.h"

#include "latrinesensor.h"

#define TIMES_TO_RUN_UPDATE_PER_TEST 10

uint16_t s_lastDuration = 0;
uint16_t s_flushStartedCount = 0;
uint16_t s_flushEndedCount = 0;

/*
 * Two callback functions for the sensor
 * These are passed to the sensor class when instantiating.
 * onFlushStart is called once when flush start is detected.
 * onFlushEnd is called once when flush end is detected. Flush duration is passed as parameter
 */
static void onFlushStart(void)
{
	s_flushStartedCount++;
}

static void onFlushEnd(uint16_t durationinSeconds)
{
	s_flushEndedCount++;
	s_lastDuration = durationinSeconds;
}

LatrineSensor sensor = LatrineSensor(onFlushStart, onFlushEnd, true);

void reset(void)
{
	s_lastDuration = 0;
	s_flushStartedCount = 0;
	s_flushEndedCount = 0;
}

void setUp(void)
{
	printf("\n");
}

void tearDown(void)
{

}

void test_FiveUpdatesCalibrateUnit(void)
{
	uint16_t i = 0;
	TEST_ASSERT_TRUE(sensor.IsCalibrating());
	
	// Should be able to do four updates without leaving calibration
	for (i = 0; i < 4; ++i)
	{
		sensor.Update(20000);
		TEST_ASSERT_TRUE(sensor.IsCalibrating());
	}
	
	// Then should go into idle (not calibrating, not flushing)
	sensor.Update(20000);
	TEST_ASSERT_FALSE(sensor.IsCalibrating());
	TEST_ASSERT_FALSE(sensor.IsFlushing());
	printf("Threshold: %d", sensor.GetFlushStartThreshold());
}

void test_ConstantInputProducesNoFlush(void)
{
	uint16_t i = 0;
	for (i = 0; i < TIMES_TO_RUN_UPDATE_PER_TEST; ++i)
	{
		sensor.Update(20000);
		TEST_ASSERT_FALSE(sensor.IsFlushing());
	}
	printf("Threshold: %d", sensor.GetFlushStartThreshold());
}

void test_ReducingInputProducesFlush(void)
{
	uint16_t i = 0;
	for (i = 0; i < TIMES_TO_RUN_UPDATE_PER_TEST; ++i)
	{
		sensor.Update(12000);
	}
	TEST_ASSERT_EQUAL(s_flushStartedCount, 1);
	TEST_ASSERT_EQUAL(s_flushEndedCount, 0);
	TEST_ASSERT_TRUE(sensor.IsFlushing());
}

void test_IncreasingInputProducesFlushEnd(void)
{
    /* An increase to LESS than the original value, but greater than end threshold
    should still produce a flush end with no return to flush start */
	uint16_t i = 0;
	for (i = 0; i < TIMES_TO_RUN_UPDATE_PER_TEST; ++i)
	{
		sensor.Update(15000);
	}
    
	TEST_ASSERT_EQUAL(1, s_flushStartedCount);
	TEST_ASSERT_EQUAL(1, s_flushEndedCount);
	TEST_ASSERT_FALSE(sensor.IsFlushing());
	printf("Threshold: %d", sensor.GetFlushStartThreshold());
}

