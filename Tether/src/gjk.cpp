#include "gjk.hpp"

#include "glmutils.hpp"

#include <iostream>

using std::cout;

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

RelColliders::RelColliders(DualPointer<Collider> c0, DualPointer<Collider> c1, TickServiceProvider& tsp)
		: c0(c0), c1(c1)
		, m_pos1(tsp.getIWorld()->toMeters(c1.e->pos - c0.e->pos))
		, m_v1(tsp.getIWorld()->toMeters(c1.e->v - c0.e->v))
	{}

vec3 RelColliders::pos0(float tick_seconds) const
{
	return vec3(0.0f);
}
vec3 RelColliders::pos1(float tick_seconds) const
{
	return m_pos1 + m_v1*tick_seconds;
}

float rootFindingSample(const RelColliders& relcolliders, float tick_seconds)
{

	cout << "~~~ rootFinding Sample ~~~:\n";
	vector<Model::ConvexPart> m0_convex_parts, m1_convex_parts;
	m0_convex_parts = relcolliders.c0.pIF->convexParts();
	m1_convex_parts = relcolliders.c1.pIF->convexParts();

	cout << "ConvexParts : " << m0_convex_parts.size() << " and " << m1_convex_parts.size() << '\n';

	float dist = std::numeric_limits<float>::max();

	//TODO AABB pruning between convex parts
	for(auto m0_cp : m0_convex_parts)
	{
		for(auto m1_cp : m1_convex_parts)
		{
			auto m0_vertices = relcolliders.c0.pIF->vertices(tick_seconds);
			auto m1_vertices = relcolliders.c1.pIF->vertices(tick_seconds);
			vec3 relpos = relcolliders.pos1(tick_seconds) - relcolliders.pos0(tick_seconds);
			MinkowskiGenerator mg (	m0_vertices.begin(), m0_vertices.end(),
									m1_vertices.end(), m1_vertices.end(),
									m0_cp.indices.begin(), m0_cp.indices.end(),
									m1_cp.indices.begin(), m1_cp.indices.end(),
									relpos
									);

			cout << '>' << std::flush;
			float new_dist = distance(mg.begin(), mg.end());
			cout << '<' << std::flush;
			if(new_dist < dist) dist = new_dist;
		}
	}
	return dist;
}

bool firstRoot(const RelColliders& relcolliders, float t0, float t1, float& time_out, int initial_samples, float epsilon)
{
	cout << " ------- first Root -------\n";
	if(initial_samples < 2) return false;
	//time, distance

	float time0, time1, dist0, dist1;
	time0 = t0; time1 = t1;

	dist0 = rootFindingSample(relcolliders, t0);


	if(dist0 <= 0.0f)
	{
		//we already entered at a state of collision
		//this doesn't count, as the collision had to be detected in the step before
		//therefore aborting with a false state, but returning time, still
		time_out = t0;
		return false;
	}

	float timespan = t1 - t0;
	float timestep = timespan / (initial_samples - 1);
	for( int sample = 1 ; sample <= initial_samples; sample++ )
	{
		time1 = t0+timestep*sample;
		dist1 = rootFindingSample(relcolliders, time1);
		if (dist1 <= 0.0f)
		{
			cout << "\nFound Intersection : \n";
			break;
		}

		if(sample == initial_samples)
		{
			cout << '.';
			return false;
		}

		dist0 = dist1;
		time0 = time1;
	}

	while(time1-time0 > epsilon)
	{
		float time05 = time0 + (time1-time0) * 0.5;
		float dist05 = rootFindingSample(relcolliders, time05);
		if(dist05 <= 0.0f)
		{
			time1 = time05;
			dist1 = dist05;
		}
		else
		{
			time0 = time05;
			dist0 = dist05;
		}
	}

	time_out = time0;
	return true;
}



} /* namespace gjk */
