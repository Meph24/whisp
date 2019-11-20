#ifndef FLTCMP_HPP
#     define FLTCMP_HPP

#include <cmath>

#define FLTCMP_DEFAULT_EPSILON 0.00001

inline bool flteqE(float a, float b, float epsilon)
{
	return std::fabs(a-b) < epsilon;
}


inline bool flteq(float a, float b)
{
	return flteqE(a, b, FLTCMP_DEFAULT_EPSILON);
}
#endif /* FLTCMP_HPP */
