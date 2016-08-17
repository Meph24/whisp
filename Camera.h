#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>

class Camera
{
	glm::mat4 perspective;
	glm::vec3 position, forward, up;

public:
	Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar);
	~Camera();

	glm::mat4 GetViewProjection() const;
};

