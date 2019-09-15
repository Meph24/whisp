#ifndef CUMULATIVEMAT_HPP
#     define CUMULATIVEMAT_HPP

#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>

using std::vector;
using std::string;
using std::ostream;
using glm::mat4;

class CumulativeMat : public glm::mat4
{
	vector<mat4> m_matstack;

public:
	CumulativeMat();
	CumulativeMat(const glm::mat4&);
	CumulativeMat& operator=(const glm::mat4&);
	mat4& current();
	void push();
	void pop();
	void loadIdentity();
	void loadZero();

	const vector<mat4>& matstack() const;
	size_t stacksize() const;
	string stackString() const;
};

ostream& operator<<(ostream& os, const CumulativeMat& m);
#endif /* CUMULATIVEMAT_HPP */
