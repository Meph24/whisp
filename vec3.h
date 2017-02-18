#pragma once
#include "MathStuff.h"

class vec3
{
public:
	flt x, y, z;

	vec3();
	vec3(flt X,flt Y,flt Z);
	~vec3();

	inline vec3 operator+(vec3 other);
	inline void operator+=(vec3 other);
	inline vec3 operator-(vec3 other);
	inline void operator-=(vec3 other);
};
inline flt dotProduct(vec3 a, vec3 b);
inline vec3 crossProduct(vec3 a, vec3 b);

//TODO