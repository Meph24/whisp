#ifndef MODELOBJECT_HPP
#     define MODELOBJECT_HPP

#include <vector>

#include "Model.hpp"

using std::vector;

class ModelObject
{
	const Model& m_model;
	vector<Vertex> m_vertices;
	mat4 vertex_transformation;
	pair<vec3, vec3> m_extent;

	mat4 to_apply_transformation;
	void updateVertices();
	void updateExtent();
	void update();

public:
	ModelObject(const Model&);

	const vector<Vertex>& vertices() const;
	const Model& model() const;

	void setTransform(const mat4& transformation_matrix);

	const pair<vec3, vec3>& extent() const;

	void drawNative() const;
	void drawHere() const;
};

#endif /* MODELOBJECT_HPP */
