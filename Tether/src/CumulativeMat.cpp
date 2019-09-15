#include "CumulativeMat.hpp"

#include <sstream>
#include <glm/ext.hpp>

using std::stringstream;

CumulativeMat::CumulativeMat()
{
	loadIdentity();
}

CumulativeMat::CumulativeMat(const glm::mat4& other)
	: glm::mat4(other)
{}

CumulativeMat& CumulativeMat::operator=(const glm::mat4& other)
{
	glm::mat4& mr = *this;
	mr = other;
	return *this;
}

mat4& CumulativeMat::current()
{
	return *this;
}

void CumulativeMat::push()
{
	m_matstack.push_back(*this);
}

void CumulativeMat::pop()
{
	*this = m_matstack.back();
	m_matstack.pop_back();
}

void CumulativeMat::loadIdentity()
{
	mat4& mr = *this;
	mr = mat4(1.0f);
}

void CumulativeMat::loadZero()
{
	mat4& mr = *this;
	mr = mat4();
}

const vector<mat4>& CumulativeMat::matstack() const
{
	return m_matstack;
}

size_t CumulativeMat::stacksize() const
{
	return m_matstack.size();
}

string CumulativeMat::stackString() const
{
	stringstream ss;
	size_t i = 0;
	for(auto e : m_matstack)
	{
		ss << i <<": -----------\n";
		ss << glm::to_string(e);
		i++;
	}
	return ss.str();
}

ostream& operator<< (ostream& os, const CumulativeMat& m)
{
	os << glm::to_string(m);
	return os;
}
