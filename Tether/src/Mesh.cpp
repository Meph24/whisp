#include "Mesh.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include "glmutils.hpp"

using std::cout;
using std::string;


array<unsigned int, 3> Mesh::faceIndicesByIndex(size_t i) const
{
	if(i >= faceCount())
		return	{0,0,0};

	return {indices[i*3], indices[i*3+1], indices[i*3+2]};
}

array<vec3, 3> Mesh::faceByIndex(size_t i) const
{
	if(i >= faceCount())
		return	{ vec3(0.f), vec3(0.f), vec3(0.f) };

	return	{
				vertices[indices[3*i]], 
				vertices[indices[3*i+1]],
				vertices[indices[3*i+2]]
			};
}


size_t Mesh::faceCount() const
{
	return indices.size() / 3;
}


ostream& operator<< (ostream& os, const Mesh& m)
{
	for(auto v : m.vertices)
	{
		os << v.x << " " << v.y << " " << v.z << " ";
	}
	os << "| ";
	for(auto i : m.indices)
	{
		os << i << " ";
	}
	return os;
}

istream& operator>> (istream& is, Mesh& m)
{
	float x, y ,z;
	string s;
	if(is >> s)
	{
		cout << ">>>>" << s << '\n';
		while(s != "|")
		{
			x = std::stof(s);
			is >> y >> z;
			vec3 newv (x, y , z);
			m.vertices.push_back(newv);
			cout << "vec  " << glm::to_string(newv) << '\n';
			if(is.eof()) break;
			is >> s;
		}
		unsigned int i;	
		while(is >> i)
		{
			m.indices.push_back(i);
		}
	}
	return is;
}
