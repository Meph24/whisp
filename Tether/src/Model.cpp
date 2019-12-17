#include "Model.hpp"

#include <algorithm>
#include <cmath>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "glmutils.hpp"

Model::Model(Mesh* mesh)
	: m_mesh(mesh)
	, current_transformation(1.0f)
	, transmat(current_transformation)
{
	updateVertices();
	updateExtent();
}

Model::Model(Mesh* mesh, const mat4& transformation_matrix)
	: m_mesh(mesh)
	, current_transformation(transformation_matrix)
	, transmat (transformation_matrix)
{
	updateVertices();
	updateExtent();
}

mat4& Model::transMat()
{
	return transmat;
}

void Model::updateVertices()
{
	m_vertices.clear();
	m_vertices.reserve(m_mesh->vertices.size());
	for(const vec3& v : m_mesh->vertices)
	{
		Vertex vertex(v.x, v.y, v.z, 1.0f);
		m_vertices.emplace_back(vertex*transmat);
	}
	current_transformation=transmat;
}

const vector<Vertex>& Model::vertices()
{
	if(transmat!=current_transformation)
	{
		updateVertices();
	}
	return m_vertices;
}

vector<EdgeRef> Model::edges()
{
	vector<EdgeRef> ret;
	size_t num_faces = m_mesh->faceCount();
	for ( size_t i = 0; i < num_faces; ++i )
	{
		//guaranteed sorted
		array<unsigned int, 3> face_indices = m_mesh->faceIndicesByIndex(i);
		EdgeRef er;
		er.sortedIn(face_indices[0], face_indices[1]);
		ret.push_back(er);
		er.sortedIn(face_indices[1], face_indices[2]);
		ret.push_back(er);
		er.sortedIn(face_indices[0], face_indices[2]);
		ret.push_back(er);
	}

	std::sort(ret.begin(), ret.end());
	std::unique(ret.begin(), ret.end());
	return ret;
}

vector<FaceRef> Model::faces()
{
	vector<FaceRef> ret;
	size_t num_faces = m_mesh->faceCount();
	for ( size_t i = 0; i < num_faces; ++i )
	{
		array<unsigned int, 3> face_indices = m_mesh->faceIndicesByIndex(i);
		FaceRef fr;
		fr.sortedIn(	face_indices[0],
						face_indices[1],
						face_indices[2]		);
		ret.push_back(fr);
	}
	return ret;
}

void Model::updateDraw()
{
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);


	glGenBuffers(V_BUFFERS::NUM, vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[V_BUFFERS::POSITION]);
	glBufferData(	GL_ARRAY_BUFFER, 
					m_mesh->vertices.size()*sizeof(m_mesh->vertices[0]), 
					m_mesh->vertices.data(),//&(m_mesh.vertices)[0], 
					GL_STATIC_DRAW
				);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	//glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffers[TEXCOORD_VB]);
	//glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(texCoords[0]), &texCoords[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//add for index stuff
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArrayBuffers[V_BUFFERS::INDEX]);
	glBufferData(	GL_ELEMENT_ARRAY_BUFFER, 
					m_mesh->indices.size()*sizeof(m_mesh->indices[0]),
				   	m_mesh->indices.data(),
				   	GL_STATIC_DRAW
				);
	//index stuff end

	glBindVertexArray(0);
}

Model::~Model()
{
	//glDeleteBuffers(V_BUFFERS::NUM, vertexArrayBuffers);
	//glDeleteVertexArrays(1, &vertexArrayObject);
}

const Mesh& Model::mesh() const
{
	return *m_mesh;
}

void Model::updateExtent()
{
	float max_x, max_y, max_z;
	max_x = max_y = max_z = 0;
	auto vert = vertices();
	for(Vertex v : vert)
	{
		if(fabs(v.x) > max_x) max_x = v.x;
		if(fabs(v.y) > max_y) max_y = v.y;
		if(fabs(v.z) > max_z) max_z = v.z;
	}
	m_extent = vec3(max_x, max_y, max_z);
}

const vec3& Model::extent()
{
	if(current_transformation!=transmat)
	{
		updateExtent();
	}
	return m_extent;
}

float Model::groundDistance()
{
	return -1*extent().y;
}

void Model::drawBuffered()
{
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_mesh->indices.size(), GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
}

void Model::drawNative()
{
	glBegin(GL_TRIANGLES);
	
	size_t faces = m_mesh->faceCount();
	for(size_t i = 0; i < faces; i++)
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
		for(vec3 vertex : m_mesh->faceByIndex(i))
		{
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
	}	
	glEnd();	
}

void Model::draw()
{
	glPushMatrix();
	//transpose for layout
	glMultMatrixf((float*) glm::value_ptr(glm::transpose(transmat)));
	drawNative();
//	drawBuffered();

	glPopMatrix();
}
