#include "Model.hpp"

#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glmutils.hpp"

#include <iostream>


Model::Model(const Mesh& mesh)
{
	m_vertices.clear();
	m_vertices.reserve(mesh.vertices.size());
	for(const vec3& v : mesh.vertices)
	{
		Vertex newv (v.x, v.y, v.z, 1.0f);
		m_vertices.push_back(Vertex(v.x, v.y, v.z, 1.0f));
	}

	m_indices = mesh.indices;

	m_vertices.shrink_to_fit();
	m_indices.shrink_to_fit();

	updateExtent();
}

const vector<Vertex>& Model::vertices() const
{
	return m_vertices;
}

vector<EdgeRef> Model::edges() const
{
	vector<EdgeRef> ret;

	for ( const FaceRef& fr : faces() )
	{
		//guaranteed sorted
		EdgeRef er;
		er.sortedIn(fr[0], fr[1]);
		ret.push_back(er);
		er.sortedIn(fr[1], fr[2]);
		ret.push_back(er);
		er.sortedIn(fr[0], fr[2]);
		ret.push_back(er);
	}

	std::sort(ret.begin(), ret.end());
	std::unique(ret.begin(), ret.end());
	return ret;
}

vector<FaceRef> Model::faces() const
{
	vector<FaceRef> ret;
	size_t num_faces = m_indices.size() / 3;
	for ( size_t i = 0; i < m_indices.size(); i+=3 )
	{
		FaceRef fr ( m_indices[i], m_indices[i+1], m_indices[i+2] );
		ret.push_back(fr);
	}
	return ret;
}

Model::~Model()
{
	//glDeleteBuffers(V_BUFFERS::NUM, vertexArrayBuffers);
	//glDeleteVertexArrays(1, &vertexArrayObject);
}

void Model::updateExtent()
{
	auto vert = vertices();

	if(vert.empty()) return;	
	vec3& min = m_extent.first;
	min = vec3(vert.front());
	vec3& max = m_extent.second;
	max = vec3(vert.front());

	for(Vertex v : vert)
	{
		if(v.x > max.x) max.x = v.x;
		else if( v.x < min.x) min.x = v.x;
		if(v.y > max.y) max.y = v.y;
		else if( v.y < min.y) min.y = v.y;
		if(v.z > max.z) max.z = v.z;
		else if( v.z < min.z) min.z = v.z;
	}
}

const pair<vec3, vec3>& Model::extent() const
{
	return m_extent;
}

void Model::drawNative() const
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBegin(GL_TRIANGLES);

	unsigned int i = 0;

	for(const FaceRef& fr : faces())	
	{
		//switch color for every triangle
		//so edges can be seen
		float thisblue;
		switch(i%4)
		{
			default:
			   	thisblue = 0.0f;
			break;
			case 0:
				thisblue = 0.0f;
			break;
			case 1:
				thisblue = 0.25f;
			break;
			case 2:
				thisblue = 0.5f;
			break;
			case 3:
				thisblue = 0.75f;
			break;
		}
		thisblue = thisblue+(0.01f*(i%25));
		glColor3f(1.0f, 0.0f, thisblue);

		for(VertexRef vr: fr)
		{
			const Vertex& vertex = m_vertices[vr];
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		i++;
	}	
	glEnd();	
}

void Model::drawHere() const
{
	drawNative();
}
