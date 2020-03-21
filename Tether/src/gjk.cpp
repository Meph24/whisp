#include "gjk.hpp"

namespace gjk {

MinkowskiPoint::MinkowskiPoint(vec3 point, unsigned int i0, unsigned int i1)
	: point(point), i0(i0), i1(i1) {}

} /* namespace gjk */
