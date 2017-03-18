#include "vec3.h"


vec3::vec3()
{

}


vec3::vec3(flt X, flt Y, flt Z):
x(X), y(Y), z(Z)
{

}


vec3::~vec3()
{

}


//inline
vec3 vec3::operator+(vec3 other)
{
	return { x + other.x, y + other.y, z + other.z };
}

//inline
void vec3::operator+=(vec3 other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}
//inline
vec3 vec3::operator-(vec3 other)
{
	return { x - other.x, y - other.y, z - other.z };
}

//inline
void vec3::operator-=(vec3 other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

//inline
vec3 vec3::operator*(flt scalar)
{
	return {x*scalar,y*scalar,z*scalar};
}

void vec3::normalize()
{
	flt lenInv=1/length();
	x*=lenInv;
	y*=lenInv;
	z*=lenInv;
}

flt vec3::normGetLen()
{
	flt len=length();
	flt lenInv=1/len;
	x*=lenInv;
	y*=lenInv;
	z*=lenInv;
	return len;
}



flt vec3::length()
{
	return sqrt(lengthSq());
}

flt vec3::lengthSq()
{
	return x*x+y*y+z*z;
}

//inline
vec3 operator*(flt scalar,vec3 v)
{
	return {v.x*scalar,v.y*scalar,v.z*scalar};
}



//get "similarity" between two unit vectors (0°=1 90°=0 180°=-1)
//inline
flt dotProduct(vec3 a, vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

//get a new vector in right angle to both input vectors (right-handed system)
//inline
vec3 crossProduct(vec3 a, vec3 b)
{
	return{ a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x };
}
