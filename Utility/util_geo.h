#ifndef _UTIL_GEO_H_
#define _UTIL_GEO_H_

#ifndef PI
#define PI 3.14159265359f
#endif

struct coord_2d
{
	float x;
	float y;
};
typedef struct coord_2d COORD_2D;
typedef struct coord_2d VECTOR_2D;

struct coord_3d
{
	float x;
	float y;
	float z;
};
typedef struct coord_3d COORD_3D;
typedef struct coord_3d VECTOR_3D;

struct line_2d
{
	COORD_2D start;
	COORD_2D end;
};
typedef struct line_2d LINE_2D;

struct line_3d
{
	COORD_3D start;
	COORD_3D end;
};
typedef struct line_3d LINE_3D;

float Length_3DVector(VECTOR_3D * v);

float AngleBetween_3DVectors(VECTOR_3D * v1, VECTOR_3D * v2);
void Difference_3DVectors(VECTOR_3D * v1, VECTOR_3D * v2, VECTOR_3D * result);

float Azimuth_3DVector(VECTOR_3D * v1);
float Elevation_3DVector(VECTOR_3D * v1);

float radians(float deg);
float degrees(float rad);

#endif
