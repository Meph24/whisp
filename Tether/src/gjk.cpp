#include "gjk.hpp"

#include "glmutils.hpp"

#include <iostream>

using std::cout;

namespace gjk {

MinkowskiPoint::MinkowskiPoint(const vec3& point, unsigned int i0, unsigned int i1)
	: vec3(point), i0(i0), i1(i1) {}


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
	os << glm::to_string((vec3)p) << '(' << p.i0 << ',' << p.i1 << ')';
	return os;
}

RelColliders::RelColliders(DualPointer<Collider> c0, DualPointer<Collider> c1, TickServiceProvider& tsp)
		: c0(c0), c1(c1)
		, m_pos1(tsp.world().toMeters(c1.e->pos - c0.e->pos))
		, m_v1(tsp.world().toMeters(c1.e->v - c0.e->v))
	{}

vec3 RelColliders::pos0(microseconds tick_time) const
{
	return vec3(0.0f);
}

#include "FloatSeconds.hpp"

vec3 RelColliders::pos1(microseconds tick_time) const
{
	return m_pos1 + m_v1* (double)FloatSeconds(tick_time);
}

float nonConvexDistance(const RelColliders& relcolliders, microseconds tick_time)
{
	vector<Model::ConvexPart> m0_convex_parts, m1_convex_parts;
	m0_convex_parts = relcolliders.c0.pIF->convexParts();
	m1_convex_parts = relcolliders.c1.pIF->convexParts();


	float dist = std::numeric_limits<float>::max();

	//TODO AABB pruning between convex parts
	for(auto m0_cp : m0_convex_parts)
	{
		for(auto m1_cp : m1_convex_parts)
		{
			auto m0_vertices = relcolliders.c0.pIF->vertices(tick_time);
			auto m1_vertices = relcolliders.c1.pIF->vertices(tick_time);
			vec3 relpos = relcolliders.pos1(tick_time) - relcolliders.pos0(tick_time);
			MinkowskiGenerator mg (	m0_vertices.begin(), m0_vertices.end(),
									m1_vertices.begin(), m1_vertices.end(),
									m0_cp.indices.begin(), m0_cp.indices.end(),
									m1_cp.indices.begin(), m1_cp.indices.end(),
									relpos
									);

			float new_dist = gjk::distance(mg.begin(), mg.end());
			if(new_dist < dist) dist = new_dist;
		}
	}
	return dist;
}

bool staticIntersectionAtTickBegin(const RelColliders& relcolliders, microseconds t0, float& time_out)
{
	float distance = nonConvexDistance(relcolliders, t0);
	if(distance > 0.0f)
	{	return false;	}
	return true;
}

bool firstRoot(const RelColliders& relcolliders, microseconds t0, microseconds t1, microseconds& time_out, int initial_samples, microseconds epsilon)
{
	if(initial_samples < 1) return false;
	//time, distance

	microseconds time0, time1;
	float dist0, dist1;

	time0 = t0; time1 = t1;

	dist0 = nonConvexDistance(relcolliders, time0);

	if(dist0 <= 0.0f)
	{
		time_out = t0;
		return true;
	}

	if(initial_samples == 1) return false;

	bool found = false;
	microseconds timespan = time1 - time0;
	microseconds timestep = timespan / (initial_samples - 1);
	int sample;
	for( sample = 1 ; sample < initial_samples; sample++ )
	{
		time1 = t0+timestep*sample;
		dist1 = nonConvexDistance(relcolliders, time1);
		if (dist1 <= 0.0f)
		{
			found = true;
			break;
		}

		dist0 = dist1;
		time0 = time1;
	}

	if(!found) return false;
	else
	{	
		size_t depth = 0;
		while( time1 - time0 > epsilon)
		{
			depth ++;
			microseconds time05 = duration_cast<microseconds>(time0 + (time1-time0) * 0.5);
			float dist05 = nonConvexDistance(relcolliders, time05);
			if(dist05 > 0.0f)
			{
				time0 = time05;
				dist0 = dist05;
			}
			else
			{
				time1 = time05;
			}
		}
		time_out = time0;
		return true;
	}
	return false;
}

} /* namespace gjk */
