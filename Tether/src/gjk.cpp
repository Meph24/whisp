#include "gjk.hpp"

#include "glmutils.hpp"

namespace gjk {

MinkowskiPoint::MinkowskiPoint(vec3 point, unsigned int i0, unsigned int i1)
	: point(point), i0(i0), i1(i1) {}


bool operator==(const MinkowskiPoint& first, const MinkowskiPoint& second)
{
	return first.i0 == second.i0 && first.i1 == second.i1;
}

bool operator!=(const MinkowskiPoint& first, const MinkowskiPoint& second)
{
	return !(first==second);
}

ostream& operator<< (ostream& os, const MinkowskiPoint& p)
{
	os << glm::to_string(p.point) << '(' << p.i0 << ',' << p.i1 << ')';
	return os;
}

} /* namespace gjk */
