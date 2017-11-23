
#ifndef SRC_VEC3_H_
#define SRC_VEC3_H_


#include "MathStuff.h"
#include <iostream>

class vec3
{
public:
	flt x, y, z;

	vec3();
	vec3(flt X,flt Y,flt Z);
	~vec3();

	vec3 operator+(vec3 other);    //inline//inline
	vec3 operator-(vec3 other);    //inline
	void operator+=(vec3 other);
	void operator-=(vec3 other);   //inline
	vec3 operator*(flt scalar);    //inline
	vec3 operator*(vec3 other);
	void normalize();
	flt normGetLen();//normalized and returns length
	flt length();
	flt lengthSq();
};
vec3 operator*(flt scalar,vec3 v); //inline
flt dotProduct(vec3 a, vec3 b);    //inline
vec3 crossProduct(vec3 a, vec3 b); //inline
std::ostream& operator<<(std::ostream &out, const vec3 v);

//TODO


#endif/*SRC_VEC3_H_*/
