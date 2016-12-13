#include "Camera.h"


Camera::Camera(const glm::vec3& pos, float fov, float aspect, float zNear, float zFar)
{
	perspective = glm::perspective(fov, aspect, zNear, zFar);

	position = pos;
	forward = glm::vec3(0, 0, 1);
	up = glm::vec3(0, 1, 0);
}

glm::mat4 Camera::GetViewProjection() const
{
	return perspective * glm::lookAt(position, position + forward, up);
}

Camera::~Camera()
{

}
