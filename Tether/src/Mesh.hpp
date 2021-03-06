#ifndef MESH_HPP
#     define MESH_HPP

#include "glmutils.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <array>

using std::array;
using std::vector;
using std::set;
using std::istream;
using std::ostream;
using glm::vec3;


struct Mesh
{
	vector<vec3> vertices;

	//Traversal Rule is TRIANGLES, which just means
	//indices come in groups of 3, which describe a triangle
	vector<unsigned int> indices;

	vector<set<unsigned int>> convex_partitions;
	
	Mesh() = default;

		template<typename VertexIterator, typename IndexIterator>
	Mesh(
		VertexIterator vertex_begin, VertexIterator vertex_end,
		IndexIterator index_begin, IndexIterator index_end
		)
	: vertices(vertex_begin, vertex_end)
	, indices(index_begin, index_end)
	{
		vertices.shrink_to_fit();
		indices.shrink_to_fit();
	}

	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;

	void setConvexPartitions(const vector<set<unsigned int>>& convex_partitions);

	array<unsigned int,3> faceIndicesByIndex(size_t i) const;
	array<vec3, 3> faceByIndex(size_t i) const;
	size_t faceCount() const;

public:
	friend ostream& operator<< (ostream& os, const Mesh& m);
	friend istream& operator>> (istream& os, Mesh& m);
};

ostream& operator<< (ostream& os, const Mesh& m);
istream& operator>> (istream& os, const Mesh& m);

#endif /* MESH_HPP */
