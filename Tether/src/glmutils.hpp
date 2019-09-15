#ifndef GLMUTILS_HPP
#     define GLMUTILS_HPP

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

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
	//float* mat = glm::value_ptr(m);
	//glm::vec3 ret;
	//ret.x = mat[0] * b.x + mat[4] * b.y + mat[8] * b.z + mat[12];
	//ret.y = mat[1] * b.x + mat[5] * b.y + mat[9] * b.z + mat[13];
	//ret.z = mat[2] * b.x + mat[6] * b.y + mat[10] * b.z + mat[14];
	//return ret
	vec4 temp = m*vec4(b.x, b.y, b.z, 1.0f);
	return vec3(temp.x, temp.y, temp.z);
}


inline glm::vec3 operator*(glm::vec3 b, glm::mat4 a){ return a*b; }

inline std::ostream& operator<< (std::ostream& os, const glm::vec3& glm_object)
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


} /* namespace glm */	
#endif /* GLMUTILS_HPP */
