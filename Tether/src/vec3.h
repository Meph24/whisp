
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

	vec3 operator+(vec3 other);
	vec3 operator-(vec3 other);
	void operator+=(vec3 other);
	void operator-=(vec3 other);
	vec3 operator*(flt scalar);
	vec3 operator*(vec3 other);

	//returns bit vector of comparison lsb=x
	unsigned int operator<(vec3 other);
	unsigned int operator>(vec3 other);//TODO must be const?

	void normalize();
	flt normGetLen();//normalize this vec3 and return length of the unnormalized version
	vec3 normalizedCopy();//returns a normalized copy of itself

	flt length();
	flt lengthSq();//return squared length

};
vec3 operator*(flt scalar,vec3 v);
flt dotProduct(vec3 a, vec3 b);
vec3 crossProduct(vec3 a, vec3 b);
std::ostream& operator<<(std::ostream &out, const vec3 v);

//TODO


#endif/*SRC_VEC3_H_*/
