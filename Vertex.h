#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

class Vertex
{
	glm::vec3 pos;
	glm::vec2 texCoord;
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord);	

	glm::vec3 getPos() const { return pos; }
	glm::vec2 getTexCoord() const { return texCoord; }
};

#endif VERTEX_H

