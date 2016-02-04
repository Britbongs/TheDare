#include "Utils.h"

bool overlaps(SATProjection proj1, SATProjection proj2)
{//Checks if two values overlap for SAT collision detection
	return(proj1.max >= proj2.min && proj1.min < proj2.max);
}

float random(float a, float b)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;

}

float radians(float a)
{
	return static_cast<float> ((PI / 180.f) * a);
}

float degrees(float a)
{
	return static_cast<float> ((180.f / PI) * a);
}

float lerp(float v1, float v2, float mod)
{
	return(v1 + ((v2 - v1) * mod));
}