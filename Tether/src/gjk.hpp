#ifndef GJK_HPP
#     define GJK_HPP

#include "GeoFeatures.hpp"

#include <utility>

using std::pair;

namespace gjk {

template<typename VertexIterator0, typename VertexIterator1, typename IndexIterator0, typename IndexIterator1>
class MinkowskiGenerator
{
	VertexIterator0 v0_begin; VertexIterator0 v0_end; VertexIterator1 v1_begin; VertexIterator1 v1_end;
	IndexIterator0 i0_begin; IndexIterator0 i0_end; IndexIterator1 i1_begin; IndexIterator1 i1_end;

public:
	MinkowskiGenerator(
			VertexIterator0 v0_begin, VertexIterator0 v0_end, VertexIterator1 v1_begin, VertexIterator1 v1_end,
			IndexIterator0 i0_begin, IndexIterator0 i0_end, IndexIterator1 i1_begin, IndexIterator1 i1_end
			)
		: v0_begin(v0_begin) , v0_end(v0_end)
		, v1_begin(v1_begin) , v1_end(v1_end)
		, i0_begin(i0_begin) , i0_end(i0_end)
		, i1_begin(i1_begin) , i1_end(i1_end)
	{}

	//Input-Iterator
	struct Iterator
	{
		const MinkowskiGenerator& source;
		IndexIterator0 iter0; IndexIterator1 iter1;
		Vertex minkowski_vertex;

		void updateVertex()
		{
			minkowski_vertex = *(source.v0_begin + (*iter0)) - *(source.v1_begin + (*iter1));
		}
	public:
		Iterator(const MinkowskiGenerator& source)
			: source(source)
			, iter0(source.i0_begin), iter1(source.i1_begin)
		{
			updateVertex();
		}
		Iterator(const Iterator&) = default;
		Iterator& operator= (const Iterator&) = default;
		~Iterator() = default;

		//prae
		Iterator& operator++()
		{
			if(iter0 == source.i0_end) return *this;

			iter1++;

			if(iter1 == source.i1_end)
			{
				iter1 = source.i1_begin;
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

		Vertex& operator*()
		{
			return minkowski_vertex;
		}

		Vertex& operator->()
		{
			return minkowski_vertex;
		}

		bool operator== (const Iterator& other) const
		{
			if(other.iter0 == source.i0_end && iter0 == source.i0_end) return true;
			if(iter0 == other.iter0 && iter1 == other.iter1) return true;
			return false;
		}

		bool operator!= (const Iterator& other) const
		{
			return !(*this == other);
		}

		pair<Vertex, Vertex> vertices() const
		{
			return std::make_pair(*(source.v0_begin + (*iter0)), *(source.v1_begin + (*iter1)));
		}
		pair<unsigned int, unsigned int> indices() const
		{
			return std::make_pair(*iter0, *iter1);
		}
		Vertex get() const
		{
			return minkowski_vertex;
		}

	};

	Iterator begin() const
	{
		return Iterator(*this);
	}
	Iterator end() const
	{
		Iterator iter(*this);
		iter.iter0 = this->i0_end;
		return iter;
	}

};

} /* namespace gjk */

#endif /* GJK_HPP */
