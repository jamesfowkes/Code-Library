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

static float length2DVector(VECTOR_2D * vector);

static float dotProduct3DVectors(VECTOR_3D * v1, VECTOR_3D * v2);
static float length3DVector(VECTOR_3D * v1);

/*
 * Public Functions
 */

float Length_2DVector(VECTOR_2D * v)
{
	return length2DVector(v);
}

float Length_3DVector(VECTOR_3D * v)
{
	return length3DVector(v);
}

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
	if (v1->x == 0.0f) { return (v1->y > 0) ? PI/2.0f : 3.0f*PI/2.0f; }
	
	return atan2(v1->y, v1->x);

}

float Elevation_3DVector(VECTOR_3D * v1)
{
	// Elevation is degrees between the vector and the projection onto the XY plane.
	VECTOR_2D xyProjection = {v1->x, v1->y};
	
	if (v1->x || v1->y)
	{
		float xy = Length_2DVector(&xyProjection);
		return atan(v1->z/Length_2DVector(&xyProjection));
	}
	else
	{
		return (v1->z > 0) ? PI/2.0f : -PI/2.0f;
	}
}

float radians(float deg) { return deg * 2 * PI / 360.0f; }
float degrees(float rad) { return rad * 360.0f / 2 * PI; }

/*
 * Private Functions
 */

static float length2DVector(VECTOR_2D * vector)
{
	float length = pow(vector->x, 2);
	length += pow(vector->y, 2);
	
	return sqrtf(length);
}

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