#ifndef MODEL_HPP
#     define MODEL_HPP

#include <utility>
#include <vector>
#include "Mesh.hpp"
#include <GL/glew.h>

#include "CumulativeMat.hpp"
#include "GeoFeatures.hpp"

using std::pair;

class Model
{
private:
	vector<Vertex> m_vertices;	
	vector<unsigned int> m_indices;

	pair<vec3, vec3> m_extent;
public:
	Model(const Mesh& mesh);

	Model(const Model& other) = default;
	Model& operator=(const Model& other) = default;

	~Model();

	const Mesh& mesh() const;

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
