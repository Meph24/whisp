#include "GeoFeatures.hpp"

#include <algorithm>

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
	std::sort(this->begin(), this->end());
	return *this;
}

void EdgeRef::sortedIn(unsigned int i0, unsigned int i1)
{
	(*this)[0] = i0;
	(*this)[1] = i1;
}

void EdgeRef::sortedIn(const array<unsigned int, 2>& other)
{
	std::copy(other.begin(), other.end(), begin());
}


FaceRef::FaceRef(const array<unsigned int, 3>& other)
	: array<unsigned int, 3> ( other )
{}

FaceRef::FaceRef(unsigned int i0, unsigned int i1, unsigned int i2)
	: array<unsigned int, 3> ( {i0, i1, i2} )
{
	std::sort(this->begin(), this->end());
}

FaceRef& FaceRef::operator=(const array<unsigned int, 3>& other)
{
	std::copy(other.begin(), other.end(), begin());
	std::sort(this->begin(), this->end());
	return *this;
}

void FaceRef::sortedIn(unsigned int i0, unsigned int i1, unsigned int i2)
{
	(*this)[0] = i0;
	(*this)[1] = i1;
	(*this)[2] = i2;
}

void FaceRef::sortedIn(const array<unsigned int, 3>& other)
{
	std::copy(other.begin(), other.end(), begin());
}
