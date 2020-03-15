#ifndef GEOFEATURES_HPP
#     define GEOFEATURES_HPP

#include <array>
#include "glmutils.hpp"

using glm::vec3;
using glm::vec4;
using glm::mat4x2;
using glm::mat4x3;

using std::array;

typedef vec4 Vertex;

struct Edge : array<Vertex, 2>
{
	using array<Vertex, 2>::array;
	Edge(Vertex, Vertex);
	Edge(const array<Vertex, 2>&);
	mat4x2 matrix() const;	
	Edge transform(const glm::mat4& transmat) const;
	Edge operator*(const glm::mat4& transmat) const;
};

struct Face : array<Vertex, 3>
{
	using array<Vertex, 3>::array;
	Face(Vertex, Vertex, Vertex);
	Face(const array<Vertex, 3>&);
	mat4x3 matrix() const;
	Face transform(const glm::mat4& transmat) const;
	Face operator*(const glm::mat4& transmat) const;
};

typedef unsigned int VertexRef;

struct EdgeRef : public array<unsigned int, 2>
{
	EdgeRef() = default;
	EdgeRef(const array<unsigned int, 2>& other);
	EdgeRef(unsigned int,unsigned int);
	EdgeRef& operator=(const array<unsigned int, 2>& other);

	template<typename Index_RandIter>
	Edge edge	(	Index_RandIter value_container_begin,
					Index_RandIter value_container_end
				) const
	{
		Index_RandIter iter0, iter1;
		iter0 = value_container_begin + (*this)[0];
		if(iter0 == value_container_end) return Edge();
		iter1 = value_container_begin + (*this)[1];
		if(iter1 == value_container_end) return Edge();
		
		return Edge (	Vertex(*iter0),
						Vertex(*iter1)	);
	}
};

struct FaceRef : public array<unsigned int, 3>
{
	FaceRef() = default;
	FaceRef(const array<unsigned int, 3>& other);
	FaceRef(unsigned int,unsigned int,unsigned int);
	FaceRef& operator=(const array<unsigned int, 3>& other);

	template<typename Index_RandIter>
	Face face	(	Index_RandIter value_container_begin,
					Index_RandIter value_container_end
				) const
	{
		Index_RandIter iter0, iter1, iter2;
		iter0 = value_container_begin + (*this)[0];
		if(iter0 == value_container_end) return Face();
		iter1 = value_container_begin + (*this)[1];
		if(iter1 == value_container_end) return Face();
		iter2 = value_container_begin + (*this)[2];
		if(iter2 == value_container_end) return Face();

		return Face(	Vertex(*iter0),
						Vertex(*iter1),
						Vertex(*iter2)
					);
	}
};

#endif /* GEOFEATURES_HPP */
