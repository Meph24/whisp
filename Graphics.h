#pragma once

#include <vector>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"
#include "Camera.h"

class Graphics
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Camera* camera;
	Transform* transform;


	//test_begin
	float counter;
	//test_begin

public:
	Graphics();
	~Graphics();


	void setClearParameters(float r, float g, float b, float a = 255, float depth = 1.0f);
	void windowClear();
	void windowClear(float r, float g, float b, float a = 255, float depth = 1.0f);

	void render();
};
