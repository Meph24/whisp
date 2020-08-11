#include "Model.hpp"

#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glmutils.hpp"

#include <iostream>



array<vec3, 24 > Model::color_list = 
{
	vec3(1.0f,0.0f,0.0f), //red
	vec3(0.0f,1.0f,0.0f), //green
	vec3(1.0f,1.0f,0.0f), //yellow
	vec3(1.0f,0.0f,1.0f), //magenta
	vec3(0.0f,1.0f,1.0f), //cyan
	vec3(1.0f,1.0f,1.0f), //white

	vec3(0.75f,0.0f,0.0f), //red
	vec3(0.0f,0.75f,0.0f), //green
	vec3(0.75f,0.75f,0.0f), //yellow
	vec3(0.75f,0.0f,0.75f), //magenta
	vec3(0.0f,0.75f,0.75f), //cyan
	vec3(0.75f,0.75f,0.75f), //white

	vec3(0.5f,0.0f,0.0f), //red
	vec3(0.0f,0.5f,0.0f), //green
	vec3(0.5f,0.5f,0.0f), //yellow
	vec3(0.5f,0.0f,0.5f), //magenta
	vec3(0.0f,0.5f,0.5f), //cyan
	vec3(0.5f,0.5f,0.5f), //white

	vec3(0.25f,0.0f,0.0f), //red
	vec3(0.0f,0.25f,0.0f), //green
	vec3(0.25f,0.25f,0.0f), //yellow
	vec3(0.25f,0.0f,0.25f), //magenta
	vec3(0.0f,0.25f,0.25f), //cyan
	vec3(0.25f,0.25f,0.25f), //white
};

Model::ConvexPart::ConvexPart(const Model& source)
	: source(source) {}

Model::ConvexPart::ConvexPart(const Model& source, set<unsigned int> indices)
	: source(source)
	, indices(indices) {}


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

	for(set<unsigned int> v: mesh.convex_partitions)
	{
		convex_parts.push_back(ConvexPart(*this, v));
	}
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
		ret.push_back(EdgeRef(fr[0], fr[1]));
		ret.push_back(EdgeRef(fr[1], fr[2]));
		ret.push_back(EdgeRef(fr[0], fr[2]));
	}

	std::sort(ret.begin(), ret.end());
	std::unique(ret.begin(), ret.end());
	return ret;
}

vector<FaceRef> Model::faces() const
{
	vector<FaceRef> ret;
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

const std::vector<Model::ConvexPart>& Model::convexParts() const
{
	return convex_parts;
}

void Model::drawNative() const
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBegin(GL_TRIANGLES);

	unsigned int i = 0;

	for(const FaceRef& fr : faces())	
	{

		vec3 color = color_list[0];
		//find the convex_part the object is in
		unsigned int j = 0;
		for(auto p : convex_parts)
		{
			FaceRef tempfr = fr;
			std::sort(tempfr.begin(), tempfr.end());
			vector<unsigned int> result(3);
			vector<unsigned int>::iterator result_end;
			result_end = std::set_intersection(p.indices.begin(), p.indices.end(), tempfr.begin(), tempfr.end(), result.begin());
			unsigned int size = result_end - result.begin();
			result.resize(size);
			if(result.size() == 3)
			{
				unsigned int color_index = (j%(color_list.size()-1))+1;
				color = color_list[color_index];
				break;
			}
			++j;
		}
		glColor3f(color.r, color.g, color.b);

		for(VertexRef vr: fr)
		{
			const Vertex& vertex = m_vertices[vr];
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		i++;
	}	
	glEnd();	
	glDisable(GL_CULL_FACE);
}

void Model::drawHere() const
{
	drawNative();
}
