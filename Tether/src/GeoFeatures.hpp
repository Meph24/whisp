#ifndef GEOFEATURES_HPP
#     define GEOFEATURES_HPP

#include <array>
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4x2;
using glm::mat4x3;

using std::array;

typedef vec4 Vertex;
typedef mat4x2 Edge;
typedef mat4x3 Face;

struct EdgeRef : public array<unsigned int, 2>
{
	EdgeRef() = default;
	EdgeRef(const array<unsigned int, 2>& other);
	EdgeRef(unsigned int,unsigned int);
	EdgeRef& operator=(const array<unsigned int, 2>& other);

	void sortedIn(unsigned int, unsigned int);
	void sortedIn(const array<unsigned int, 2>& other);

	template<typename Index_RandIter>
	Edge edge	(	Index_RandIter value_container_begin,
					Index_RandIter value_container_end
				) const
	{
		Index_RandIter iter0, iter1;
		iter0 = value_container_begin + (*this)[0];
		if(iter0 = value_container_end) return Edge();
		iter1 = value_container_begin + (*this)[1];
		if(iter1 = value_container_end) return Edge();
		
		return Edge	(	Vertex((*iter0), 1.0f),
						Vertex((*iter1), 1.0f)	
					);
	}
};

struct FaceRef : public array<unsigned int, 3>
{
	FaceRef() = default;
	FaceRef(const array<unsigned int, 3>& other);
	FaceRef(unsigned int,unsigned int,unsigned int);
	FaceRef& operator=(const array<unsigned int, 3>& other);

	void sortedIn(unsigned int, unsigned int, unsigned int);
	void sortedIn(const array<unsigned int, 3>& other);

	template<typename Index_RandIter>
	Face face	(	Index_RandIter value_container_begin,
					Index_RandIter value_container_end
				) const
	{
		Index_RandIter iter0, iter1, iter2;
		iter0 = value_container_begin + (*this)[0];
		if(iter0 = value_container_end) return Face();
		iter1 = value_container_begin + (*this)[1];
		if(iter1 = value_container_end) return Face();
		iter2 = value_container_begin + (*this)[2];
		if(iter2 = value_container_end) return Face();

		return Face	(	Vertex((*iter0), 1.0f),
						Vertex((*iter1), 1.0f),
						Vertex((*iter2), 1.0f)	
					);
	}
};

#endif /* GEOFEATURES_HPP */
