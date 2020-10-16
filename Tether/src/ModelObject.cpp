#include "ModelObject.hpp"

#include <numeric>
#include <utility>

ModelObject::ModelObject(const Model& model)
	: m_model(model)
	, vertex_transformation(mat4(1.0f))
{
	calcRadialExtentAndSizeFromMid(model);
}

float ModelObject::sizeFromMid() const
{
	return size_from_mid;
}
void ModelObject::calcRadialExtentAndSizeFromMid(const Model& model)
{
	float maxlen = std::numeric_limits<float>::min();
	for(const Vertex& v : model.vertices())
	{
		float newlen = glm::length( v );
		if(maxlen < newlen)
		{
			maxlen = newlen;
		}	
	}
	size_from_mid = maxlen;
	vec3 high = glm::normalize(vec3(1, 1, 1)) * maxlen * 1.42 * 1.42;
	m_radial_extent = std::make_pair(-high, high);
}

#include <algorithm>

vector<Vertex> ModelObject::vertices() const
{
	vector<Vertex> new_vertices;
	std::transform( m_model.vertices().begin(), m_model.vertices().end(), std::back_inserter(new_vertices),
	[this] (const Vertex& v) -> Vertex { return this->vertex_transformation * v; } );
	new_vertices.shrink_to_fit();
	return new_vertices;
}

const Model& ModelObject::model() const {return m_model;}
void ModelObject::setTransform(const mat4& transformation_matrix)
{
	vertex_transformation = transformation_matrix;
}

pair<vec3, vec3> ModelObject::extent() const
{
	/*
	if (m_model.vertices().empty()) return std::make_pair( vec3(0.0f), vec3(0.0f) );
	
	pair<vec3, vec3> extent;

	vec3& min = extent.first;
	vec3& max = extent.second;

	min.x = std::numeric_limits<float>::infinity();
	min.y = std::numeric_limits<float>::infinity();
	min.z = std::numeric_limits<float>::infinity();
	max.x = -std::numeric_limits<float>::infinity();
	max.y = -std::numeric_limits<float>::infinity();
	max.z = -std::numeric_limits<float>::infinity();

	for(const Vertex& v : vertices() )
	{
		if(v.x > max.x) max.x = v.x;
		if(v.x < min.x) min.x = v.x;
		if(v.y > max.y) max.y = v.y;
		if(v.y < min.y) min.y = v.y;
		if(v.z > max.z) max.z = v.z;
		if(v.z < min.z) min.z = v.z;
	}
	return extent;
	*/
	return m_radial_extent;
}

void ModelObject::drawNative() const
{
	glPushMatrix();
	
	glMultMatrixf((float*) glm::value_ptr(vertex_transformation));
	m_model.drawHere();


	glPopMatrix();
}

void ModelObject::drawHere() const
{
	drawNative();
}
