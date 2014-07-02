/*
 * Standard library includes
 */

#include <stdint.h>
#include <math.h>

/*
 * Utility Library Includes
 */

#include "util_geo.h"

/*
 * Private Function Prototypes
 */
 
static float dotProduct3DVectors(VECTOR_3D * v1, VECTOR_3D * v2);
static float length3DVector(VECTOR_3D * v1);

/*
 * Public Functions
 */

float AngleBetween_3DVectors(VECTOR_3D * v1, VECTOR_3D * v2)
{
	float length1 = length3DVector(v1);
	float length2 = length3DVector(v2);
	float dp = dotProduct3DVectors(v1, v2);
	
	return acosf(dp/(length1*length2));
}

void Difference_3DVectors(VECTOR_3D * v1, VECTOR_3D * v2, VECTOR_3D * result)
{
	result->x = v1->x - v2->x;
	result->y = v1->y - v2->y;
	result->z = v1->z - v2->z;
}

float Azimuth_3DVector(VECTOR_3D * v1)
{
	// Azimuth is degrees between x and y components
	return atan(v1->x/v1->y);
}

float Elevation_3DVector(VECTOR_3D * v1)
{
	// Elevation is degrees between the vector and the projection onto the XY plane.
	// Create a projection vector and use that
	VECTOR_3D xy = { v1->x, v1->y, 0 };
	return AngleBetween_3DVectors(v1, &xy);
}

/*
 * Private Functions
 */

static float length3DVector(VECTOR_3D * vector)
{
	float length = pow(vector->x, 2);
	length += pow(vector->y, 2);
	length += pow(vector->z, 2);
	
	return sqrtf(length);
}

static float dotProduct3DVectors(VECTOR_3D * v1, VECTOR_3D * v2)
{
	return (v1->x * v2->x) + (v1->y * v2->y) + (v1->z * v2->z);
}