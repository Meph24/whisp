#ifndef GLMUTILS_HPP
#     define GLMUTILS_HPP

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

inline float sqlen(const glm::vec3& vec)
{
	return	  vec.x*vec.x
			+ vec.y*vec.y
			+ vec.z*vec.z;
}

inline glm::vec3 operator*(mat4 m,vec3 b)
{
	vec4 temp = m*vec4(b.x, b.y, /*TODO REMOVE b.z*/ 5.0f, 1.0f);
	return vec3(temp.x, temp.y, temp.z);
}


inline glm::vec3 operator*(glm::vec3 b, glm::mat4 a){ return a*b; }

inline glm::vec4 operator+(glm::vec4 v4, glm::vec3 v3){return v4 + vec4(v3, 1.0f);}

inline std::ostream& operator<< (std::ostream& os, const glm::vec2& glm_object)
{
	os << glm::to_string(glm_object);
	return os;
}

inline std::ostream& operator<< (std::ostream& os, const glm::vec3& glm_object)
{
	os << glm::to_string(glm_object);
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::vec4& glm_object)
{
	os << glm::to_string(glm_object);
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const glm::mat4& glm_object)
{
	os << glm::to_string(glm_object);
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

inline bool equalsE(mat4 a, mat4 b, float epsilon)
{
	float* ap = glm::value_ptr(a);
	float* bp = glm::value_ptr(b);

	for ( int i = 0; i < 16; ++i )
	{
		if(!flteq(ap[i], bp[i]))
		return false;
	}
	return true;
}

inline bool equals(mat4 a, mat4 b)
{
	return equalsE(a, b, GLMUTILS_EQUALS_DEFAULT_EPSIOLON);
}

} /* namespace glm */	
#endif /* GLMUTILS_HPP */
