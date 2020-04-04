#ifndef GJK_HPP
#     define GJK_HPP


#include <algorithm>
#include <utility>
#include <vector>
#include <iostream>
#include <limits>

#include "GeoFeatures.hpp"
#include "TickServiceProvider.h"
#include "IWorld.h"
#include "Model.hpp"

#include "Entity.h"

using std::cout;
using std::ostream;

using std::vector;

using std::pair;

namespace gjk {

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
	struct Iterator
	{
		const MinkowskiGenerator* source;
		IndexIterator0 iter0; IndexIterator1 iter1;
		vec3 minkowski_vertex;


		void updateVertex()
		{
			minkowski_vertex = vec3((*(source->v0_begin + (*iter0))) - (source->relpos + *(source->v1_begin + (*iter1))));
		}
	public:
		Iterator(const MinkowskiGenerator* source)
			: source(source)
			, iter0(source->i0_begin), iter1(source->i1_begin)
		{
			updateVertex();
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
			}

			updateVertex();

			return *this;
		}
		//post
		Iterator operator++(int)
		{
			Iterator newiter = *this;
			++(*this);
			return newiter;
		}

		vec3& operator*()
		{
			return minkowski_vertex;
		}

		vec3& operator->()
		{
			return minkowski_vertex;
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

		pair<Vertex, Vertex> vertices() const
		{
			return std::make_pair(*(source->v0_begin + (*iter0)), *(source->v1_begin + (*iter1)));
		}
		pair<unsigned int, unsigned int> indices() const
		{
			return std::make_pair(*iter0, *iter1);
		}

		vec3 relativePosition() const
		{
			return source->relpos;
		}
		vec3 get() const
		{
			return minkowski_vertex;
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


struct MinkowskiPoint
{
	vec3 point;
	unsigned int i0, i1;

	MinkowskiPoint() = default;
	MinkowskiPoint(vec3 point, unsigned int i0, unsigned int i1);
};

bool operator==(const MinkowskiPoint& first, const MinkowskiPoint& second);
bool operator!=(const MinkowskiPoint& first, const MinkowskiPoint& second);

ostream& operator<< (ostream& os, const MinkowskiPoint& p);

template<typename MinkowskiPointIterator>
MinkowskiPoint makeMinkowskiPoint(const MinkowskiPointIterator& iter)
{
	auto indices = iter.indices();
	return std::move(MinkowskiPoint(iter.get(), indices.first, indices.second));
}

template<typename MinkowskiPointIterator>
MinkowskiPoint maxSupport(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, const vec3& direction)
{
	MinkowskiPointIterator max = std::max_element(mp_begin, mp_end,
				[direction](const vec3& first, const vec3& second)
				{
					return glm::dot(first, direction) < glm::dot(second, direction);
				});
	
	return makeMinkowskiPoint(max);
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
	float d = glm::dot(d0, d1);
	return d > 0;
}

inline bool doPoint(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& a = supports.back().point;
	direction = -a;
	return false;
}

inline bool doLine(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& b = supports.front().point;
	const vec3& a = supports.back().point;
	vec3 ab = b-a;
	//AB same direction as A0
	if(sameDirection(ab, -a)) //origin near the line
	{
		//supports can remain same
		direction = glm::cross(glm::cross(ab, -a), ab);
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
	const vec3& c = supports[0].point;
	const vec3& b = supports[1].point;
	const vec3& a = supports[2].point;
	const vec3 ab = b-a;
	const vec3 ac = c-a;
	const vec3 abc = glm::cross(ab, ac);

	if(sameDirection(glm::cross(abc, ac), -a))
	{
		supports = {supports[0], supports[2]};
		return doLine(supports, direction);
	}
	//need to check with -ab for winding purposes (direction of the cross)
	else if(sameDirection(glm::cross(abc, -ab), -a))
	{
		supports = {supports[1], supports[2]};
		return doLine(supports, direction);
	}
	else
	{
		if(sameDirection(abc, -a))
		{
			//supports stay the same
			direction = abc;
			return false;
		}
		else
		{
			std::swap(supports[0], supports[1]);
			direction = -abc;
			return false;
		}
	}
}

inline bool doTetrahedron(vector<MinkowskiPoint>& supports, vec3& direction)
{
	const vec3& d = supports[0].point;
	const vec3&	c = supports[1].point;
	const vec3&	b = supports[2].point;
	const vec3&	a = supports[3].point;

	const vec3 ab = b-a;
	const vec3 ac = c-a;
	const vec3 ad = d-a;

	const vec3 abc = glm::cross(ab, ac);
	const vec3 acd = glm::cross(ac, ad);
	const vec3 adb = glm::cross(ad, ab);

	if(sameDirection(abc, -a))
	{
		supports = {supports[1], supports[2], supports[3]};
		return doTriangle(supports, direction);
	}
	else if(sameDirection(acd, -a))
	{
		supports = {supports[0], supports[1], supports[3]};
		return doTriangle(supports, direction);
	}
	else if(sameDirection(adb, -a))
	{
		supports = {supports[2], supports[0], supports[3]};
		return doTriangle(supports, direction);
	}
	else
	{
		return true;
	}

}

inline bool doSimplex(vector<MinkowskiPoint>& supports, vec3& direction)
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
			return glm::length(supports.back().point);
		break;
		case SimplexType::LINE:
		{
			//get the normal in direction to the origin
			const vec3& a = supports[0].point;
			const vec3& b = supports[1].point;
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
			const vec3& a = supports[2].point;
			const vec3& b = supports[1].point;
			const vec3& c = supports[0].point;
			vec3 n = glm::cross(b-a, c-a);
			//normalize normal
			n /= glm::length(n);
			return glm::dot(n, -a);
		}
		break;
	}
}

template<typename MinkowskiPointIterator>
bool intersection(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, std::vector<MinkowskiPoint>* supports = nullptr)
{
	if(!supports)
	{
		supports = new std::vector<MinkowskiPoint>();
		supports->reserve(4);
		supports->emplace_back(makeMinkowskiPoint(mp_begin));
	}
	else if(supports->size() > 4 || supports->size() < 1)
	{
		supports->clear();
		supports->reserve(4);
		supports->emplace_back(makeMinkowskiPoint(mp_begin));
	}

	vec3 direction;

	while(!doSimplex(*supports, direction))
	{
		MinkowskiPoint newSupport = maxSupport(mp_begin, mp_end, direction);
		float s = glm::dot(newSupport.point, direction);
		if(s < 0) 
		{
			return false;
		}
		supports->emplace_back(newSupport);
	}
	return true;
}

template<typename MinkowskiPointIterator>
float distance(const MinkowskiPointIterator& mp_begin, const MinkowskiPointIterator& mp_end, std::vector<MinkowskiPoint>* supports = nullptr)
{
	if(!supports)
	{
		supports = new std::vector<MinkowskiPoint>();
		supports->reserve(4);
		supports->emplace_back(makeMinkowskiPoint(mp_begin));
	}
	else if(supports->size() > 4 || supports->size() < 1)
	{
		supports->clear();
		supports->reserve(4);
		supports->emplace_back(makeMinkowskiPoint(mp_begin));
	}

	vec3 direction;

	while(!doSimplex(*supports, direction))
	{
		cout << '+' << std::flush;
		MinkowskiPoint new_support = maxSupport(mp_begin, mp_end, direction);
		if(std::find(supports->begin(), supports->end(), new_support) != supports->end())
		{
			//we've already found the neares feature
			return doDistance(*supports);
		}
		supports->emplace_back(new_support);
	}
	return 0.0;
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

float rootFindingSample(const RelColliders& relcolliders, float tick_seconds);
bool firstRoot(const RelColliders& relcolliders, float t0, float t1, float& time_out, int initial_samples = 10, float epsilon = 0.001);


} /* namespace gjk */

#endif /* GJK_HPP */
