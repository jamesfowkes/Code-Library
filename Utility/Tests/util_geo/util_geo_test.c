#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "unity.h"

#include "util_angle.h"
#include "util_geo.h"

VECTOR_3D vectorX = {1, 0, 0};
VECTOR_3D vectorY = {0, 1, 0};
VECTOR_3D vectorZ = {0, 0, 1};

VECTOR_3D vectorinXYplane = {1, 1, 0};
VECTOR_3D vectorinXZplane = {1, 0, 1};
VECTOR_3D vectorinYZplane = {0, 1, 1};

VECTOR_3D opposingX = {-1, 0, 0};
VECTOR_3D opposingY = {0, -1, 0};
VECTOR_3D opposingZ = {0, 0, -1};
	
void setUp(void)
{
}

void tearDown(void)
{

}

void testAngleBetweenSameVectorsIsZero(void)
{
	TEST_ASSERT_EQUAL_FLOAT(0.0f, AngleBetween_3DVectors(&vectorX, &vectorX));
	TEST_ASSERT_EQUAL_FLOAT(0.0f, AngleBetween_3DVectors(&vectorY, &vectorY));
	TEST_ASSERT_EQUAL_FLOAT(0.0f, AngleBetween_3DVectors(&vectorZ, &vectorZ));
}

void testAngleBetweenOrthogonalVectorsIsHalfPi(void)
{
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorX, &vectorY));
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorX, &vectorZ));
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorY, &vectorX));
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorY, &vectorZ));
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorZ, &vectorX));
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, AngleBetween_3DVectors(&vectorZ, &vectorY));
}

void testAngleBetweenOpposingVectorsIsPi(void)
{
	TEST_ASSERT_EQUAL_FLOAT(PI, AngleBetween_3DVectors(&vectorX, &opposingX));
	TEST_ASSERT_EQUAL_FLOAT(PI, AngleBetween_3DVectors(&vectorY, &opposingY));
	TEST_ASSERT_EQUAL_FLOAT(PI, AngleBetween_3DVectors(&vectorZ, &opposingZ));
}

void testAngleBetween45degreeVectorsIsQuarterPi(void)
{
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorX, &vectorinXYplane));
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorY, &vectorinXYplane));
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorX, &vectorinXZplane));
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorZ, &vectorinXZplane));
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorY, &vectorinYZplane));
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, AngleBetween_3DVectors(&vectorZ, &vectorinYZplane));
}

void testLengthOfVector(void)
{
	TEST_ASSERT_EQUAL_FLOAT(1.0f, Length_3DVector(&vectorX));
	TEST_ASSERT_EQUAL_FLOAT(1.0f, Length_3DVector(&vectorY));
	TEST_ASSERT_EQUAL_FLOAT(1.0f, Length_3DVector(&vectorZ));
	
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2), Length_3DVector(&vectorinXYplane));
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2), Length_3DVector(&vectorinXZplane));
	TEST_ASSERT_EQUAL_FLOAT(sqrt(2), Length_3DVector(&vectorinYZplane));
}

void testSubtractVectors(void)
{
	VECTOR_3D result;
	
	Difference_3DVectors(&vectorX, &vectorY, &result);
	TEST_ASSERT_EQUAL_FLOAT(1.0f, result.x);
	TEST_ASSERT_EQUAL_FLOAT(-1.0f, result.y);
	TEST_ASSERT_EQUAL_FLOAT(0.0f, result.z);
	
	Difference_3DVectors(&vectorX, &vectorZ, &result);
	TEST_ASSERT_EQUAL_FLOAT(1.0f, result.x);
	TEST_ASSERT_EQUAL_FLOAT(0.0f, result.y);
	TEST_ASSERT_EQUAL_FLOAT(-1.0f, result.z);
	
	Difference_3DVectors(&vectorY, &vectorZ, &result);
	TEST_ASSERT_EQUAL_FLOAT(0.0f, result.x);
	TEST_ASSERT_EQUAL_FLOAT(1.0f, result.y);
	TEST_ASSERT_EQUAL_FLOAT(-1.0f, result.z);
}

void test_VectorAzimuth(void)
{
	TEST_ASSERT_EQUAL_FLOAT(0.0f, Azimuth_3DVector(&vectorX));				// 0 degrees
	TEST_ASSERT_EQUAL_FLOAT(PI/4.0f, Azimuth_3DVector(&vectorinXYplane));	// 45 degrees
	TEST_ASSERT_EQUAL_FLOAT(PI/2.0f, Azimuth_3DVector(&vectorY));			// 90 degrees
	// 135 degrees
	TEST_ASSERT_EQUAL_FLOAT(PI, Azimuth_3DVector(&opposingX));				// 180 degrees
	// 225 degrees
	TEST_ASSERT_EQUAL_FLOAT(3*PI/2.0f, Azimuth_3DVector(&opposingY));			// 270 degrees
}

void test_VectorElevationFirstQuadrant(void)
{
	VECTOR_3D zplanevector = {0,0,0};
	char message[100];
	
	for (float deg = 0.0f; deg < 90.0f; deg++)
	{
		zplanevector.z = sin(radians(deg));
		zplanevector.x = cos(radians(deg));
		float result = Elevation_3DVector(&zplanevector);
				
		sprintf(message, "(For %.1f degrees, expected %f, got %f)", deg, radians(deg), result);	
		TEST_ASSERT_EQUAL_FLOAT_MESSAGE(radians(deg), result, message);
	}
}

void test_VectorElevationSecondQuadrant(void)
{
	VECTOR_3D zplanevector = {0,0,0};
	char message[100];
	
	for (float deg = 90.0f; deg < 180.0f; deg++)
	{
		zplanevector.z = sin(radians(deg));
		zplanevector.x = cos(radians(deg));
		float result = Elevation_3DVector(&zplanevector);
				
		sprintf(message, "(For %.1f degrees, expected %f, got %f)", deg, radians(180.0f-deg), result);	
		TEST_ASSERT_EQUAL_FLOAT_MESSAGE(radians(180.0f-deg), result, message);
	}
}

void test_VectorElevationThirdQuadrant(void)
{
	VECTOR_3D zplanevector = {0,0,0};
	char message[100];
	
	for (float deg = 181.0f; deg < 270.0f; deg++)
	{
		zplanevector.z = sin(radians(deg));
		zplanevector.x = cos(radians(deg));
		float result = Elevation_3DVector(&zplanevector);
				
		sprintf(message, "(For %.1f degrees, expected %f, got %f)", deg, radians(180.0f-deg), result);	
		TEST_ASSERT_EQUAL_FLOAT_MESSAGE(radians(180.0f-deg), result, message);
	}
}

void test_VectorElevationFourthQuadrant(void)
{
	VECTOR_3D zplanevector = {0,0,0};
	char message[100];
	
	for (float deg = 270.0f; deg < 359.0f; deg++)
	{
		zplanevector.z = sin(radians(deg));
		zplanevector.x = cos(radians(deg));
		float result = Elevation_3DVector(&zplanevector);
				
		sprintf(message, "(For %.1f degrees, expected %f, got %f)", deg, radians(deg-360.0f), result);	
		TEST_ASSERT_EQUAL_FLOAT_MESSAGE(radians(deg-360.0f), result, message);
	}
}
