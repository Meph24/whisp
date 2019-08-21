#ifndef MODEL_HPP
#     define MODEL_HPP

#include "Mesh.hpp"

#include <GL/glew.h>

class Model
{
private:
	float m_scale;
	Mesh* m_mesh;
	
	enum V_BUFFERS
	{
		POSITION = 0,
		INDEX,

		NUM
	};

	GLuint vertexArrayObject;
	GLuint vertexArrayBuffers[V_BUFFERS::NUM];
	unsigned int drawCount;

public:
	Model(Mesh* mesh, float scale = 1.0f);
	~Model();

	const Mesh& mesh() const;
	const float& scale() const;

	/**
	 * @return Vec3 with maximum occupated space in each direction from mid (radial).
	 */
	vec3 extent() const;
	float groundDistance() const;

	void draw();
	void drawBuffered(); //with buffered objects
	void drawNative();	//native gl calls

	void updateDraw();

};
#endif /* MODEL_HPP */
