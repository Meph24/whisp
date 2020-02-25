#ifndef MODELOBJECT_HPP
#     define MODELOBJECT_HPP

#include <vector>

#include "Model.hpp"

using std::vector;

class ModelObject
{
	const Model& m_model;
	mutable vector<Vertex> m_vertices;
	mutable mat4 vertex_transformation;

	mat4 to_apply_transformation;
	void updateVertices() const;


	mutable pair<vec3, vec3> m_extent;

	mutable bool extent_is_outdated;
	void updateExtent() const;
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
