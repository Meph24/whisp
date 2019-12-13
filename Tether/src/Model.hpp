#ifndef MODEL_HPP
#     define MODEL_HPP

#include "Mesh.hpp"
#include <GL/glew.h>

#include "CumulativeMat.hpp"
#include "GeoFeatures.hpp"

class Model
{
private:
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

	mat4 current_transformation;
	vector<Vertex> m_vertices;
	CumulativeMat transmat;
	vec3 m_extent;
public:
	Model(Mesh* mesh);
	Model(Mesh* mesh, const mat4& transmat);

	Model(const Model& other) = default;
	Model& operator=(const Model& other) = default;

	~Model();

	const Mesh& mesh() const;

	CumulativeMat& transMat();
	const vector<Vertex>& vertices();
	vector<EdgeRef> edges();
	vector<FaceRef> faces();

	/**
	 * @return Vec3 with maximum occupated space in each direction from mid (radial).
	 */
	const vec3& extent();
	float groundDistance();

	void draw();
	void drawBuffered(); //with buffered objects
	void drawNative();	//native gl calls

	void updateDraw();

private:
	void updateExtent();
	void updateVertices();
};
#endif /* MODEL_HPP */
