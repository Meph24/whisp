#ifndef TRANSFORM_H
#define TRANSFORM_H


#include <glm\gtx\transform.hpp>
#include "glm\glm.hpp"


class Transform
{
	

public:	
	glm::vec3 pos, rot, scale;

	inline const glm::vec3& getPos() const;
	inline const glm::vec3& getRot() const;
	inline const glm::vec3& getScale() const;

	void setPos(glm::vec3& pos);

	void setPosX(float x);
	void setPosY(float y);
	void setPosZ(float z);

	inline void setRot(glm::vec3& rot);
	inline void setScale(glm::vec3& Scale);

	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~Transform();

	glm::mat4 getModel() const;
};

#endif

