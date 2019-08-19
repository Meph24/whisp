#include "Model.hpp"

#include <cmath>
#include <GL/glew.h>

Model::Model(Mesh* mesh, float scale)
	:
	m_scale(scale),
	m_mesh(mesh)
{
	updateDraw();
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
	glDeleteBuffers(V_BUFFERS::NUM, vertexArrayBuffers);
	glDeleteVertexArrays(1, &vertexArrayObject);
}

const Mesh& Model::mesh() const
{
	return *m_mesh;
}

const float& Model::scale() const
{
	return m_scale;
}

vec3 Model::extent() const
{
	return m_scale * m_mesh->extent();
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

	int i = 0;
	for(array<vec3, 3> triangle : *m_mesh)
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
		i++;
		for(vec3 vertex : triangle)
		{
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
	}	

	glEnd();	
}

void Model::draw()
{
	glPushMatrix();

	drawNative();
//	drawBuffered();

	glPopMatrix();
}
