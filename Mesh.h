#ifndef MESH_H
#define MESH_H

#include "Vertex.h"
#include <GL\glew.h>


class Mesh
{
	
private:
	enum{
		POSITION_VB,
		TEXCOORD_VB,

		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[NUM_BUFFERS];
	unsigned int drawCount;
public:
	Mesh();
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	~Mesh();

	void draw();
};


#endif