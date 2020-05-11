#ifndef MODEL_HPP
#     define MODEL_HPP

#include <utility>
#include <vector>
#include "Mesh.hpp"
#include <GL/glew.h>

#include <set>

#include "GeoFeatures.hpp"

using std::set;
using std::pair;

class Model
{
private:
	static array<vec3, 24 > color_list;

	vector<Vertex> m_vertices;	
	vector<unsigned int> m_indices;

	pair<vec3, vec3> m_extent;

public:
	struct ConvexPart
	{
		const Model& source;
		set<unsigned int> indices;
		ConvexPart(const Model& source);
		ConvexPart(const Model& source, set<unsigned int> indices);

		template<typename IndexIterator>
		ConvexPart(const Model& source, IndexIterator begin_iter, IndexIterator end_iter)
		: source(source)
		, indices(begin_iter, end_iter) {}
	};
private:
	std::vector<ConvexPart> convex_parts;
public:
	Model(const Mesh& mesh);

	Model(const Model& other) = default;
	Model& operator=(const Model& other) = default;

	~Model();

	const Mesh& mesh() const;

	const std::vector<ConvexPart>& convexParts() const;

	const vector<Vertex>& vertices() const;
	vector<EdgeRef> edges() const;
	vector<FaceRef> faces() const;

	const pair<vec3, vec3>& extent() const;

	void drawHere() const;
	void drawBuffered() const; //with buffered objects
	void drawNative() const;	//native gl calls

	void updateDraw();

private:
	void updateExtent();
	void updateVertices();
};
#endif /* MODEL_HPP */
