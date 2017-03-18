#pragma once
#include "MathStuff.h"
#include "vec3.h"

class quat
{
public:
	flt r, i, j, k;

	quat();
	quat(vec3 normVec,float angle);
	quat(flt R,flt I,flt J,flt K);
	~quat();

	quat operator*(quat other);
	quat operator+(quat other);
	quat inverse();
	void invert();
	vec3 operator*(vec3 point);
	flt abs();
	flt absSq();
};

//TODO
