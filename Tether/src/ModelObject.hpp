#ifndef MODELOBJECT_HPP
#     define MODELOBJECT_HPP

#include <vector>

#include "Model.hpp"

using std::vector;

class ModelObject
{
	const Model& m_model;
	mat4 vertex_transformation;

public:
	ModelObject(const Model&);

	vector<Vertex> vertices() const;
	const Model& model() const;

	void setTransform(const mat4& transformation_matrix);

	pair<vec3, vec3> extent() const;

	void drawNative() const;
	void drawHere() const;
};

#endif /* MODELOBJECT_HPP */
