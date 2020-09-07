#include "ModelObject.hpp"

#include <numeric>

ModelObject::ModelObject(const Model& model)
	: m_model(model)
	, m_vertices(model.vertices())
	, vertex_transformation(mat4(1.0f))
	, to_apply_transformation(vertex_transformation)
	, extent_is_outdated(true)
{
	updateVertices();
}

const vector<Vertex>& ModelObject::vertices() const 
{
	if(vertex_transformation != to_apply_transformation)
		updateVertices();
	return m_vertices;
}

const Model& ModelObject::model() const {return m_model;}
void ModelObject::setTransform(const mat4& transformation_matrix)
{
	updateVertices();
	updateExtent();
	to_apply_transformation = transformation_matrix;
}


const pair<vec3, vec3>& ModelObject::extent() const
{
	if(extent_is_outdated)
		updateExtent();
	return m_extent;
}

void ModelObject::updateVertices() const
{
	m_vertices.clear();
	m_vertices.reserve(m_model.vertices().size());
	for(const auto& v : m_model.vertices())
	{
		m_vertices.push_back(v * to_apply_transformation);
	}
	vertex_transformation = to_apply_transformation;
	extent_is_outdated = true;	
}

void ModelObject::updateExtent() const
{
	if (m_vertices.empty()) return;
	
	vec3& min = m_extent.first;
	vec3& max = m_extent.second;

	min.x = std::numeric_limits<float>::infinity();
	min.y = std::numeric_limits<float>::infinity();
	min.z = std::numeric_limits<float>::infinity();
	max.x = -std::numeric_limits<float>::infinity();
	max.y = -std::numeric_limits<float>::infinity();
	max.z = -std::numeric_limits<float>::infinity();

	for(const Vertex& v : vertices())
	{
		if(v.x > max.x) max.x = v.x;
		else if( v.x < min.x) min.x = v.x;
		if(v.y > max.y) max.y = v.y;
		else if( v.y < min.y) min.y = v.y;
		if(v.z > max.z) max.z = v.z;
		else if( v.z < min.z) min.z = v.z;
	}

	extent_is_outdated = false;
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
