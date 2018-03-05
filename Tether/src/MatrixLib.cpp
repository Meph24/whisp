#include "MatrixLib.h"

//inline
vec3 operator*(mat4 mat,vec3 b)
{
	vec3 ret;
	ret.x = mat[0] * b.x + mat[4] * b.y + mat[8] * b.z + mat[12];
	ret.y = mat[1] * b.x + mat[5] * b.y + mat[9] * b.z + mat[13];
	ret.z = mat[2] * b.x + mat[6] * b.y + mat[10] * b.z + mat[14];
	return ret;
}

//inline
vec3 operator*(mat3 mat,vec3 b)
{
	vec3 ret;
	ret.x = mat[0] * b.x + mat[3] * b.y + mat[6] * b.z;
	ret.y = mat[1] * b.x + mat[4] * b.y + mat[7] * b.z;
	ret.z = mat[2] * b.x + mat[5] * b.y + mat[8] * b.z;
	return ret;
}


//inline
vec3 operator*(vec3 b, mat4 a){ return a*b; };
//inline
vec3 operator*(vec3 b, mat3 a)
{
	return a * b;
}
;

bool operator <(vec3 a, vec3 b)
{
	return (a.x<b.x)&&(a.y<b.y)&&(a.z<b.z);
}

bool operator >(vec3 a, vec3 b)
{
	return (a.x>b.x)&&(a.y>b.y)&&(a.z>b.z);
}
