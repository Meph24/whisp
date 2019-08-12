#ifndef MESH_HPP
#     define MESH_HPP

#include "vec3.h"
#include <vector>
#include <array>

using std::array;
using std::vector;

struct Mesh
{
	vector<vec3> vertices;

	//Traversal Rule is TRIANGLES, which just means
	//indices come in groups of 3, which describe a triangle
	vector<unsigned int> indices;

	vec3 m_extent;
	
		template<typename VertexIterator, typename IndexIterator>
	Mesh(
		VertexIterator vertex_begin, VertexIterator vertex_end,
		IndexIterator index_begin, IndexIterator index_end
		)
	:
		vertices(vertex_begin, vertex_end),
		indices(index_begin, index_end)
	{
		vertices.shrink_to_fit();
		indices.shrink_to_fit();
		m_extent = calculateExtent();
	}

	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;

	const vec3& extent() const;


	class TriangleIterator
	{
		Mesh* mesh;
		size_t iter;
		size_t maxcount;
		array<vec3, 3> triangle;
		bool value_loaded;
	public:
		TriangleIterator();
		TriangleIterator(size_t triangle_count, Mesh* mesh, size_t iter = 0);
		TriangleIterator(const TriangleIterator& other) = default;
		TriangleIterator& operator= (const TriangleIterator& other) = default;
		TriangleIterator& operator++();//prae
		TriangleIterator& operator++(int);//post
		array<vec3, 3>& operator* ();
		array<vec3, 3>* operator-> ();
		bool operator == (const TriangleIterator& other) const;
		bool operator != (const TriangleIterator& other) const;
	};

	array<vec3, 3> triangleByIndex(size_t i) const;
	size_t triangleCount() const;

	TriangleIterator begin();
	TriangleIterator end();


private:
	vec3 calculateExtent();
};


#endif /* MESH_HPP */
