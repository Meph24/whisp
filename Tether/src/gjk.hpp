#ifndef GJK_HPP
#     define GJK_HPP

//#define GJK_RUNTIME_ASSERTS

#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <limits>
#include <iterator>

#include "GeoFeatures.hpp"
#include "TickServiceProvider.h"
#include "IWorld.h"
#include "Model.hpp"
#include "static_intersection.hpp"

#include "Entity.h"

using std::cout;
using std::ostream;

using std::vector;

using std::pair;

namespace gjk {

struct MinkowskiPoint : public vec3
{
	unsigned int i0, i1;

	MinkowskiPoint() = default;
	MinkowskiPoint(const vec3& point, unsigned int i0, unsigned int i1);
};

bool operator==(const MinkowskiPoint& first, const MinkowskiPoint& second);
bool operator!=(const MinkowskiPoint& first, const MinkowskiPoint& second);

ostream& operator<< (ostream& os, const MinkowskiPoint& p);

template<typename VertexIterator0, typename VertexIterator1, typename IndexIterator0, typename IndexIterator1>
class MinkowskiGenerator
{
	VertexIterator0 v0_begin; VertexIterator0 v0_end; VertexIterator1 v1_begin; VertexIterator1 v1_end;
	IndexIterator0 i0_begin; IndexIterator0 i0_end; IndexIterator1 i1_begin; IndexIterator1 i1_end;

	vec3 relpos;

public:
	MinkowskiGenerator(
			VertexIterator0 v0_begin, VertexIterator0 v0_end, VertexIterator1 v1_begin, VertexIterator1 v1_end,
			IndexIterator0 i0_begin, IndexIterator0 i0_end, IndexIterator1 i1_begin, IndexIterator1 i1_end,
			vec3 relative_position = vec3(0.0f)
			)
		: v0_begin(v0_begin) , v0_end(v0_end)
		, v1_begin(v1_begin) , v1_end(v1_end)
		, i0_begin(i0_begin) , i0_end(i0_end)
		, i1_begin(i1_begin) , i1_end(i1_end)
		, relpos(relative_position)
	{}

	//Input-Iterator
	struct Iterator : public std::iterator<std::input_iterator_tag, MinkowskiPoint>
	{
		const MinkowskiGenerator* source;
		IndexIterator0 iter0; IndexIterator1 iter1;
		MinkowskiPoint point;

		void updatePoint()
		{
			point = MinkowskiPoint (
					*(source->v0_begin + (*iter0)) - (source->relpos + *(source->v1_begin + (*iter1))),
					*iter0, *iter1);
		}
	public:
		Iterator(const MinkowskiGenerator* source)
			: source(source)
			, iter0(source->i0_begin), iter1(source->i1_begin)
		{
			updatePoint();
		}
		Iterator(const Iterator& other) = default;
		Iterator& operator= (const Iterator&) = default;
		~Iterator() = default;

		//prae
		Iterator& operator++()
		{
			if(iter0 == source->i0_end) return *this;

			iter1++;

			if(iter1 == source->i1_end)
			{
				iter1 = source->i1_begin;
				iter0++;
				if(iter0 == source->i0_end) return *this;
			}

			updatePoint();

			return *this;
		}
		//post
		Iterator operator++(int)
		{
			Iterator newiter = *this;
			++(*this);
			return newiter;
		}

		const MinkowskiPoint& operator*() const
		{
			return point;
		}

		const MinkowskiPoint& operator->() const
		{
			return point;
		}

		bool operator== (const Iterator& other) const
		{
			if(other.iter0 == source->i0_end && iter0 == source->i0_end) return true;
			if(iter0 == other.iter0 && iter1 == other.iter1) return true;
			return false;
		}

		bool operator!= (const Iterator& other) const
		{
			return !(*this == other);
		}
	};

	Iterator begin() const
	{
		return Iterator(this);
	}
	Iterator end() const
	{
		Iterator iter(this);
		iter.iter0 = this->i0_end;
		return iter;
	}

};

template<typename MinkowskiPointIterator>
MinkowskiPoint maxSupport(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, const vec3& direction)
{
	auto ret = *std::max_element(mp_begin, mp_end,
				[direction](const MinkowskiPoint& first, const MinkowskiPoint& second)
				{
					float f = glm::dot(first, direction);
					float s = glm::dot(second, direction);
					return f<s;
				});
	return ret;
}

enum SimplexType
{
	NONE = 0,
	POINT = 1,
	LINE = 2,
	TRIANGLE = 3,
	TETRAHEDRON = 4,

	NUM_SIMPLEXTYPE
};

inline bool parallel(const vec3& d0, const vec3& d1)
{
	return (d0 / glm::length(d0)) == (d1 / glm::length(d1));
}

inline bool sameDirection(const vec3& d0, const vec3& d1)
{
	return glm::dot(d0,d1) > 0;
}

inline bool doPoint(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& a = supports.back();
	if ( vecEquals (vec3(0.0f), a) ) return true;
	direction = -a;
	return false;
}

inline bool doLine(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& b = supports.front();
	const vec3& a = supports.back();
	vec3 ab = b-a;
	//AB same direction as A0
	if(sameDirection(ab, -a)) //origin near the line
	{
		direction = glm::cross(glm::cross(ab, -a), ab);
		if(vecEquals(direction,vec3(0.0f))) return true;
		return false;
	}
	else //origin nearer to the newest point
	{
		supports = { supports.back() };
		return doPoint(supports, direction);
	}
}

inline bool doTriangle(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& c = supports[0];
	const vec3& b = supports[1];
	const vec3& a = supports[2];
	const vec3 ab = b-a;
	const vec3 ac = c-a;
	const vec3 abc = glm::cross(ab, ac);


	const vec3 ac_orth = glm::cross(abc, ac);
#ifdef GJK_RUNTIME_ASSERTS
		if(!sameDirection(ac_orth, a-b)) cout << "[TRIANGLE LINE AC (sd: BA) DIRECTION ASSERT FAILED!]";
		if(!sameDirection(ac_orth, c-b)) cout << "[TRIANGLE LINE AC (sd: CB) DIRECTION ASSERT FAILED!]";
#endif /* GJK_RUNTIME_ASSERTS */
	
	// we might need to change the direction here, as we defined the smaller feature (simplex with less points) to be including
	// the border points, where the dot is 0
	// therefore we check whether the origin is not in the same direction as towards inside
	// instead of asking if it is in the direction of outside
	if(!sameDirection(-ac_orth, -a))
	{

		supports = {supports[0], supports[2]};
		return doLine(supports, direction);
	}
	//need to check with -ab for winding purposes (direction of the cross)
	else 
	{	
		vec3 ab_orth = glm::cross(abc, -ab);
#ifdef GJK_RUNTIME_ASSERTS
		if(!sameDirection(ab_orth, a-c)) cout << "[TRIANGLE LINE AB (sd: CA) DIRECTION ASSERT FAILED!]";
		if(!sameDirection(ab_orth, b-c)) cout << "[TRIANGLE LINE AB (sd: CB) DIRECTION ASSERT FAILED!]";
#endif /* GJK_RUNTIME_ASSERTS */
		if(!sameDirection(-ab_orth, -a))
		{
			supports = {supports[1], supports[2]};
			auto ret = doLine(supports, direction);
			return ret;
		}
	
		else
		{
			if(!sameDirection(-abc, -a))
			{


				//supports stay the same
				direction = abc;

				if(std::abs(glm::dot(abc, -a)) < 0.0001)
					return true;
#ifdef GJK_RUNTIME_ASSERTS
			if(!sameDirection(-a, direction)) cout << "[TRIANGLE DIRECTION ASSERT FAILED!]";
#endif /* GJK_RUNTIME_ASSERTS */
				return false;
			}
			else
			{
				//supports stay the same
				std::swap(supports[0], supports[1]);
				direction = -abc;
#ifdef GJK_RUNTIME_ASSERTS
			if(!sameDirection(-a, direction)) cout << "[TRIANGLE DIRECTION ASSERT FAILED!]";
#endif /* GJK_RUNTIME_ASSERTS */
				return false;
			}
		}
	}
}

inline bool doTetrahedron(vector<MinkowskiPoint>& supports, vec3& direction)
{

	const vec3& d = supports[0];
	const vec3&	c = supports[1];
	const vec3&	b = supports[2];
	const vec3&	a = supports[3];

	const vec3 ab = b-a;
	const vec3 ac = c-a;
	const vec3 ad = d-a;

	const vec3 abc = glm::cross(ab, ac);
	const vec3 acd = glm::cross(ac, ad);
	const vec3 adb = glm::cross(ad, ab);

	if(!sameDirection(-abc, -a))
	{
		supports = {supports[1], supports[2], supports[3]};
		return doTriangle(supports, direction);
	}
	else if(!sameDirection(-acd, -a))
	{
		supports = {supports[0], supports[1], supports[3]};
		return doTriangle(supports, direction);
	}
	else if(!sameDirection(-adb, -a))
	{
		supports = {supports[2], supports[0], supports[3]};
		return doTriangle(supports, direction);
	}
	else
	{
		return true;
	}

}

inline bool closestSimplex(vector<MinkowskiPoint>& supports, vec3& direction)
{
	switch(supports.size())
	{
		default:
			return false;
		break;
		case SimplexType::POINT:
			return doPoint(supports, direction);
		break;
		case SimplexType::LINE:
			return doLine(supports, direction);
		break;
		case SimplexType::TRIANGLE:
			return doTriangle(supports, direction);
		break;
		case SimplexType::TETRAHEDRON:
			return doTetrahedron(supports, direction);
		break;
	}
}

inline float doDistance(const vector<MinkowskiPoint>& supports)
{

	switch(supports.size())
	{
		default:
			return 0.0;
		case SimplexType::POINT:
			return glm::length((vec3)supports.back());
		break;
		case SimplexType::LINE:
		{
			//get the normal in direction to the origin
			const vec3& a = supports[0];
			const vec3& b = supports[1];
			vec3 ab = b - a;
			vec3 n = glm::cross(glm::cross(ab, -a), ab);
			//normalize that normal
			n /= glm::length(n);
			//the dot product should give us the length of projection of the normal to the origin
			return glm::dot(n, -a);
		}
		break;
		case SimplexType::TRIANGLE:
		{
			//since we know the point is definitiely over the triangle, double projection with the triangles normal will suffice
			const vec3& a = supports[2];
			const vec3& b = supports[1];
			const vec3& c = supports[0];
			vec3 n = glm::cross(b-a, c-a);
			//normalize normal
			n /= glm::length(n);
			return glm::dot(n, -a);
		}
		break;
	}
}

template<typename MinkowskiPointIterator>
bool intersection(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, std::vector<MinkowskiPoint>* supports_out = nullptr)
{
	vector<MinkowskiPoint> supports;
	supports.reserve(4);

	{
		MinkowskiPoint minkowskipoint;
		MinkowskiPointIterator begincopy = mp_begin;
		minkowskipoint = *begincopy;
		supports.emplace_back(minkowskipoint);
	}

	vec3 direction;

	unsigned int iter_depth = 0;
	while(!closestSimplex(supports, direction))
	{
		MinkowskiPoint new_support = maxSupport(mp_begin, mp_end, direction);

		//all the lines the new support impies have to be directed in the search direction
		
		if(glm::dot(new_support, direction) < 0) 
		{
			return false;
		}

#ifdef GJK_RUNTIME_ASSERTS
			if(std::any_of(supports.begin(), supports.end(), [&new_support, direction](const MinkowskiPoint& p){ return !sameDirection(new_support-p, direction);}))
			{
				cout << "[EXPANSION DIRECTION ASSERTION FAILED!]\n{\n";
				cout << "Direction : " << direction << '\n'
					<< "New Support : " << new_support << '\n';
				for(auto p : supports)
				{
					if(!sameDirection(new_support-p, direction))
					{
						cout << ">>>";
					}
					cout << p << " with a dot of " << glm::dot(new_support-p, direction) << '\n';
				}
				cout << "\n}\n";
			}
				
#endif /* GJK_RUNTIME_ASSERTS */



		supports.emplace_back(new_support);

		iter_depth++;
	}

	if(supports_out) *supports_out = supports;
	return true;
}

template<typename MinkowskiPointIterator>
float distance(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, std::vector<MinkowskiPoint>* supports_out = nullptr)
{
	MinkowskiPoint start = *mp_begin;
	vector<MinkowskiPoint> supports = { start }; supports.reserve(4);
	vec3 direction = -start;
	float dist = doDistance(supports);

	while( dist > 0.0f )
	{	
		MinkowskiPoint next_support = maxSupport(mp_begin, mp_end, direction);
		if( std::find(supports.begin(), supports.end(), next_support) == supports.end() ) supports.push_back( next_support );
		if (closestSimplex( supports, direction ))
		{
			//typically closestSimplex() only is true when the origin could be enclosed
			if(supports_out) *supports_out = supports;
			return 0.0f;
		}
		float new_dist = doDistance(supports);
		if( new_dist >= dist )
		{			
			if(supports_out) *supports_out = supports;
			return dist;
		}
		if( supports.size() == 4 )
		{
			//definitely enclosed
			if(supports_out) *supports_out = supports;
			return dist;
		}
		dist = new_dist;
		//new direction has been set by closestSimplex()
	}
	if(supports_out) *supports_out = supports;
	return dist;
}

template<typename MinkowskiPointIterator>
float distance2(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, std::vector<MinkowskiPoint>* supports_out = nullptr)
{
	std::vector<MinkowskiPoint> supports; 
	supports.reserve(4);
	float dist = 0.0f;

	if(intersection(mp_begin, mp_end, &supports)) 
	{
		return dist;
	}

	{
		MinkowskiPoint minkowskipoint;
		MinkowskiPointIterator begincopy = mp_begin;
		minkowskipoint = *begincopy;
		supports.emplace_back(minkowskipoint);
	}

	vec3 direction;


	unsigned int iter_depth = 0;
	vec3 old_direction(-direction);
	float min_dist = std::numeric_limits<float>::max();


	while(!closestSimplex(supports, direction))
	{
		float temp_dist = doDistance(supports);
		if(temp_dist < min_dist)
		{
			min_dist = temp_dist;
		}
		else
		{
			dist = min_dist;
			break;
		}

#ifdef GJK_RUNTIME_ASSERTS
		if(supports.size() > 3)
			cout << "WARNING: In gjk::distance a Tetrahedron was given as closest Simplex. This should not be, as the origin is confirmed not inside the shape!\n";
#endif /*GJK_RUNTIME_ASSERTS*/

		MinkowskiPoint new_support = maxSupport(mp_begin, mp_end, direction);
		//the new point from here must be really farther, not coplanar to the ones we have already
		//this also holds true if the lines we draw is actually just a point, the dot gets 0
		//to test this quicky we can draw a line to the new point and test its dot with the search direction

		if (std::any_of(supports.begin(), supports.end(), [&new_support,&direction](const MinkowskiPoint&p){return !sameDirection(new_support-p, direction);}))
		{
			//we've already found the nearest feature
			dist = doDistance(supports);
			break;
		}

#ifdef GJK_RUNTIME_ASSERTS
			if(std::any_of(supports.begin(), supports.end(), [&new_support, direction](const MinkowskiPoint& p){ return !sameDirection(new_support-p, direction);}))
			{
				cout << "[EXPANSION DIRECTION ASSERTION FAILED!]\n{\n";
				cout << "Direction : " << direction << '\n'
					<< "New Support : " << new_support << '\n';
				for(auto p : supports)
				{
					if(!sameDirection(new_support-p, direction))
					{
						cout << ">>>";
					}
					cout << p << " with a dot of " << glm::dot(new_support-p, direction) << '\n';
				}
				cout << "\n}\n";
			}
				
#endif /* GJK_RUNTIME_ASSERTS */


		supports.emplace_back(new_support);
		old_direction = direction;
		iter_depth++;
	}
	
	if(supports_out) *supports_out = std::move(supports);
	return dist;
}

struct RelColliders
{
	DualPointer<Collider> c0;
	DualPointer<Collider> c1;

private:
	vec3 m_pos1;
	vec3 m_v1;
public:
	RelColliders(DualPointer<Collider> c0, DualPointer<Collider> c1, TickServiceProvider& tsp);

	vec3 pos0(float tick_seconds) const;
	vec3 pos1(float tick_seconds) const;
};


bool staticIntersectionAtTickBegin(const RelColliders& relcolliders, float t0, float& time_out);
float rootFindingSample(const RelColliders& relcolliders, float tick_seconds);
bool firstRoot(const RelColliders& relcolliders, float t0, float t1, float& time_out, int initial_samples = 10, float epsilon = 0.001);


} /* namespace gjk */

#endif /* GJK_HPP */
