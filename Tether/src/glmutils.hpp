#ifndef GLMUTILS_HPP
#     define GLMUTILS_HPP


#define GLM_META_PROG_HELPERS
//Set flag to enable static length

#include <glm/glm.hpp>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#include <glm/ext.hpp>
#pragma GCC diagnostic pop
//TODO find out why it is throwing lots of warnings with -O3 (instead of just ignoring them)
//This only happens with one specific setup (windows, eclipse, mingw)

#include "fltcmp.hpp"

#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::inverse;

namespace glm
{

template<typename vec_type>
inline float sqlen(const vec_type& vec)
{
	float sum = 0.0;
	auto vptr = glm::value_ptr(vec);
	for ( int i = 0; i < vec_type::components; ++i )
	{
		sum += *(vptr+i) * *(vptr+i);
	}
	return sum;
}

inline glm::vec3 operator*(const mat4& m, const vec3& b)
{
	vec4 temp = m*vec4(b, 1.0f);
	return vec3(temp.x, temp.y, temp.z);
}


inline glm::vec3 operator*(const glm::vec3& b, const glm::mat4& a)
{ return a*b; }

/* alternative
 * since glm define v*m differently from m*v, since vectors have no
 * differenciation between column and row vectors v is interpreted differently
 * at both multiplication operations
 * we previously defined this here as v*m = m*v, which is wrong
 * and togeter with the librarys implementation inconsistent.
 *
 * PROPOSAL:
 * inline glm::vec4 operator*(const glm::vec3& b, const glm::mat4& a) {return vec4(b, 1.0f) * a;}
*/

inline glm::vec4 operator+(const glm::vec4& v4, const glm::vec3& v3)
{	return vec4(vec3(v4) + v3, v4.w);	}

inline glm::vec4 operator+(const glm::vec3& v3, const glm::vec4& v4)
{	return vec4(v3 + vec3(v4), v4.w);	}

template<template<typename, precision> class matType, typename T, precision P>
inline std::ostream& operator<<(std::ostream& os, matType<T,P> const & m)
{
	os << glm::to_string(m);
	return os;
}

inline mat4 	rotateDeg (float angle, vec3 const &v)
{
	return glm::rotate(glm::radians(angle), v);
}

inline mat4 	rotateDeg (mat4 const &m, float angle, vec3 const& v)
{
	return glm::rotate(m, glm::radians(angle), v);
}

#define GLMUTILS_EQUALS_DEFAULT_EPSIOLON 0.00001

template<typename mat_type, typename epsilon_prec>
inline bool matEqualsE(mat_type a, mat_type b, epsilon_prec epsilon)
{
	auto ap	= glm::value_ptr(a);
    auto bp	= glm::value_ptr(b);

	for ( int i = 0; i < (mat_type::cols * mat_type::rows); ++i )
	{
		if(!flteqE(ap[i], bp[i], epsilon))
		return false;
	}
	return true;
}

template<typename mat_type>
inline bool matEquals(mat_type a, mat_type b)
{
	return matEqualsE(a, b, GLMUTILS_EQUALS_DEFAULT_EPSIOLON);
}

template<typename vec_type, typename epsilon_prec>
inline bool vecEqualsE(vec_type a, vec_type b, epsilon_prec epsilon)
{
	auto ap = glm::value_ptr(a);
	auto bp = glm::value_ptr(b);

	for ( int i = 0; i < a.length(); ++i )
	{
		if(!flteqE(ap[i], bp[i], epsilon))
		return false;
	}
	return true;
}

template<typename vec_type>
inline bool vecEquals(const vec_type& a, const vec_type& b)
{
	return vecEqualsE(a, b, GLMUTILS_EQUALS_DEFAULT_EPSIOLON);
}

#include <iostream>
using std::cout;

#include "MathStuff.h"

inline vec3 directionToAngleRotation(const vec3& direction)
{
	if(direction == vec3(0.0f)) return vec3(0.0f);
	vec3 zero_rotation (0.0f, 0.0f, 1.0f);
	vec3 dir = glm::normalize(direction);

	const vec3 xaxis ( 1.0f, 0.0f, 0.0f );
	const vec3 yaxis ( 0.0f, 1.0f, 0.0f );
	const vec3 zaxis ( 0.0f, 0.0f, 1.0f );
	if(vecEquals(zero_rotation, dir)) return vec3(0.0f);
	float x = glm::orientedAngle(zero_rotation, dir, xaxis);
	x = (x < 0)? TAU - x : x;
	zero_rotation = zero_rotation * glm::rotate(x, xaxis);
	if(vecEquals(zero_rotation, dir)) return vec3(x, 0.0f, 0.0f);
	float y = glm::orientedAngle(zero_rotation, dir, yaxis);
	y = (y < 0)? TAU - y : y;
	zero_rotation = zero_rotation * glm::rotate(y, yaxis);
	if(vecEquals(zero_rotation, dir)) return vec3(x, y, 0.0f);
	float z = glm::orientedAngle(zero_rotation, dir, zaxis);
	z = (z < 0)? TAU - z : z;

	zero_rotation = zero_rotation * glm::rotate(z, zaxis); 
	assert(vecEquals(zero_rotation, dir));

	return vec3(x, y, z);
}

} /* namespace glm */	
#endif /* GLMUTILS_HPP */
