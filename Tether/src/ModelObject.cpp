#include "ModelObject.hpp"

#include <numeric>

ModelObject::ModelObject(const Model& model)
	: m_model(model)
	, m_vertices(model.vertices())
	, vertex_transformation(mat4(1.0f))
	, to_apply_transformation(vertex_transformation)
{
	update();
}

void ModelObject::update()
{
	if(vertex_transformation != to_apply_transformation)
	{
		updateVertices();
		updateExtent();
	}
}

const vector<Vertex>& ModelObject::vertices() const 
{
	return m_vertices;
}

const Model& ModelObject::model() const {return m_model;}
void ModelObject::setTransform(const mat4& transformation_matrix)
{
	to_apply_transformation = transformation_matrix;
	update();
}

const pair<vec3, vec3>& ModelObject::extent() const
{
	return m_extent;
}

#include <algorithm>

void ModelObject::updateVertices()
{
	vector<Vertex> new_vertices;
	std::transform( m_model.vertices().begin(), m_model.vertices().end(), std::back_inserter(new_vertices),
	[this] (const Vertex& v) -> Vertex { return v * this->to_apply_transformation; } );
	new_vertices.shrink_to_fit();
	m_vertices = new_vertices;
	vertex_transformation = to_apply_transformation;
}

void ModelObject::updateExtent()
{
	if (vertices().empty()) return;
	
	vec3& min = m_extent.first;
	vec3& max = m_extent.second;

	min.x = std::numeric_limits<float>::infinity();
	min.y = std::numeric_limits<float>::infinity();
	min.z = std::numeric_limits<float>::infinity();
	max.x = -std::numeric_limits<float>::infinity();
	max.y = -std::numeric_limits<float>::infinity();
	max.z = -std::numeric_limits<float>::infinity();

	for(const Vertex& v : m_vertices )
	{
		if(v.x > max.x) max.x = v.x;
		if(v.x < min.x) min.x = v.x;
		if(v.y > max.y) max.y = v.y;
		if(v.y < min.y) min.y = v.y;
		if(v.z > max.z) max.z = v.z;
		if(v.z < min.z) min.z = v.z;
	}
}

void ModelObject::drawNative() const
{
	glPushMatrix();
	
	glMultMatrixf((float*) glm::value_ptr(glm::transpose(vertex_transformation)));
	m_model.drawHere();


	glPopMatrix();
}

void ModelObject::drawHere() const
{
	drawNative();
}
