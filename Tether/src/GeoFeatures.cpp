#include "GeoFeatures.hpp"

#include <algorithm>


Edge::Edge(Vertex v0, Vertex v1)
	: array<Vertex, 2>({v0, v1})
{}

mat4x2 Edge::matrix() const
{
	return	mat4x2(	
			(*this)[0][0],
			(*this)[1][0],
			(*this)[0][1],
			(*this)[1][1],
			(*this)[0][2],
			(*this)[1][2],
			(*this)[0][3],
			(*this)[1][3]
			);

}

Edge Edge::transform(const glm::mat4& transmat) const
{
	Edge newedge;
	size_t i = 0;
	for(Vertex v : (*this))
	{
		newedge[i] = transmat * v;
		++i;
	}
	return newedge;
}

Edge Edge::operator*(const glm::mat4& transmat) const
{
	return transform(transmat);
}

Face::Face(Vertex v0, Vertex v1, Vertex v2)
	: array<Vertex, 3>({v0, v1, v2})
{}

mat4x3 Face::matrix() const
{
	return mat4x3(	
			(*this)[0][0],
			(*this)[1][0],
			(*this)[2][0],
			(*this)[0][1],
			(*this)[1][1],
			(*this)[2][1],
			(*this)[0][2],
			(*this)[1][2],
			(*this)[2][2],
			(*this)[0][3],
			(*this)[1][3],
			(*this)[2][3]
		  );

}

Face Face::transform(const glm::mat4& transmat) const
{
	Face newface;
	size_t i = 0;
	for(Vertex v : (*this))
	{
		newface[i] = transmat * v;
		++i;
	}
	return newface;
}

Face Face::operator*(const glm::mat4& transmat) const
{
	return transform(transmat);
}

EdgeRef::EdgeRef(const array<unsigned int, 2>& other)
	: array<unsigned int, 2> ( other )
{
	std::sort(this->begin(), this->end());
}

EdgeRef::EdgeRef(unsigned int i0, unsigned int i1)
	: array<unsigned int, 2> ( {i0, i1} )
{
	std::sort(this->begin(), this->end());
}

EdgeRef& EdgeRef::operator=(const array<unsigned int, 2>& other)
{
	std::copy(other.begin(), other.end(), begin());
	return *this;
}

FaceRef::FaceRef(const array<unsigned int, 3>& other)
	: array<unsigned int, 3> ( other )
{}

FaceRef::FaceRef(unsigned int i0, unsigned int i1, unsigned int i2)
	: array<unsigned int, 3> ( {i0, i1, i2} )
{
}

FaceRef& FaceRef::operator=(const array<unsigned int, 3>& other)
{
	std::copy(other.begin(), other.end(), begin());
	return *this;
}
