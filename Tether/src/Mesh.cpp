#include "Mesh.hpp"
#include <string>
#include <iostream>

using std::cout;
using std::string;

Mesh::Mesh()
	:
		m_extent(0.0,0.0,0.0)
{}

const vec3& Mesh::extent() const
{
	return m_extent;	
}

vec3 Mesh::calculateExtent()
{
	float max_x, max_y, max_z;
	max_x = max_y = max_z = 0.0;
	
	//TODO change with impl of rotation
	//for now without rotation
	for(auto v : vertices)
	{
		float x = abs(v.x);
		max_x = (x > max_x)? x : max_x;
		
		float y = abs(v.y);
		max_y = (y > max_y)? y : max_y;

		float z = abs(v.z);
		max_z = (z > max_z)? z : max_z;
	}
	return vec3(max_x, max_y, max_z);
}


Mesh::TriangleIterator::TriangleIterator()
	:
		mesh(nullptr),
		iter(0),
		maxcount(0),
		triangle	{
						vec3({0.0f,0.0f,0.0f}),
						vec3({0.0f,0.0f,0.0f}),
						vec3({0.0f,0.0f,0.0f})
					},
		value_loaded(false)
{}

Mesh::TriangleIterator::TriangleIterator(size_t triangle_count, Mesh* mesh, size_t iter)
	:
	mesh(mesh),
	iter(iter),
	maxcount(triangle_count),
	value_loaded(false)
{
	if(iter < maxcount)
		triangle = mesh->triangleByIndex(iter);
}


Mesh::TriangleIterator& Mesh::TriangleIterator::operator++() //prae
{
	if(iter >= maxcount)
		return *this;
	
	iter++;
	value_loaded = false;
	return *this;
}

Mesh::TriangleIterator& Mesh::TriangleIterator::operator++(int) //post
{
	if(iter >= maxcount)
	{
		return *this;
	}
	
	iter++;
	value_loaded = false;
	return *this;
}

array<vec3, 3>& Mesh::TriangleIterator::operator* ()
{
	if(!value_loaded)
	{
		value_loaded = true;
		triangle = mesh->triangleByIndex(iter);
	}
	return triangle;
}

array<vec3, 3>* Mesh::TriangleIterator::operator->()
{
	if(!value_loaded)
	{
		value_loaded = true;
		triangle = mesh->triangleByIndex(iter);
	}
	return &triangle;
}

bool Mesh::TriangleIterator::operator== (const Mesh::TriangleIterator& other) const
{
	return	(
				iter >= maxcount && 
				other.iter >= other.maxcount
			) //both are end iterators
			||
			(
				mesh == other.mesh &&
				iter == other.iter &&
				maxcount == other.maxcount
			);
}

bool Mesh::TriangleIterator::operator!= (const Mesh::TriangleIterator& other) const
{
	return !(*this == other);
}

array<vec3, 3> Mesh::triangleByIndex(size_t i) const
{
	if(i >= triangleCount())
		return	{
					vec3{0.0f,0.0f,0.0f}, 
					vec3{0.0f,0.0f,0.0f}, 
					vec3{0.0f,0.0f,0.0f}
				};
	return {
			vertices[indices[3*i]], 
			vertices[indices[3*i+1]],
			vertices[indices[3*i+2]]
		   };
}


size_t Mesh::triangleCount() const
{
	return indices.size() / 3;
}

Mesh::TriangleIterator Mesh::begin()
{
	return TriangleIterator(triangleCount(), this, 0);
}

Mesh::TriangleIterator Mesh::end()
{
	return TriangleIterator(triangleCount(), this, triangleCount());
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
	if(!is.eof())
	{
		is >> s;
		cout << ">>>>" << s << '\n';
		while(s != "|")
		{
			x = std::stof(s);
			is >> y >> z;
			vec3 newv (x, y , z);
			m.vertices.push_back(newv);
			cout << "vec  " << newv << '\n';
			if(is.eof()) break;
			is >> s;
		}
		unsigned int i;	
		while(!is.eof())
		{
			is >> i;
			m.indices.push_back(i);
		}
	}
	m.m_extent = m.calculateExtent();
	return is;
}
