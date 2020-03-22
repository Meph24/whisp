#ifndef GJK_HPP
#     define GJK_HPP

#include "GeoFeatures.hpp"

#include <algorithm>
#include <utility>

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

template<typename MinkowskiGeneratorType>
MinkowskiPoint maxSupport(const MinkowskiGeneratorType& mg, const vec3& direction)
{
	auto max = std::max_element(mg.begin(), mg.end(),
				[direction](const vec3& first, const vec3& second)
				{
					return glm::dot(first, direction) < glm::dot(second, direction);
				});
	
	auto max_indices = max.indices();
	return MinkowskiPoint(*max, max_indices.first, max_indices.second);
}

} /* namespace gjk */

#endif /* GJK_HPP */
