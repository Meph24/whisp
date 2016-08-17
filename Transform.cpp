#include "Transform.h"


Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) :
pos(pos),
rot(rot),
scale(scale)
{
}


Transform::~Transform()
{
}



 glm::mat4 Transform::getModel() const 
{
	glm::mat4 posMatrix = glm::translate(pos);
	glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1, 0, 0));
	glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0, 1, 0));
	glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0, 0, 1));
	glm::mat4 scaleMatrix = glm::scale(scale);

	glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;				//multiply them in reverse order
	
	return posMatrix * rotMatrix * scaleMatrix;								// first scale them right , then turn them, then move them the right spot
}	
