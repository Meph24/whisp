#ifndef MODELOBJECT_HPP
#     define MODELOBJECT_HPP

#include <vector>

#include "Model.hpp"

using std::vector;

class ModelObject
{
	const Model& m_model;
	mat4 vertex_transformation;

	float size_from_mid;
	pair<vec3, vec3> m_radial_extent;

	void calcRadialExtentAndSizeFromMid(const Model&);

public:
	ModelObject(const Model&);

	vector<Vertex> vertices() const;
	const Model& model() const;

	void setTransform(const mat4& transformation_matrix);

	pair<vec3, vec3> extent() const;
	float sizeFromMid() const;

	void drawNative() const;
	void drawHere() const;
};

#endif /* MODELOBJECT_HPP */
