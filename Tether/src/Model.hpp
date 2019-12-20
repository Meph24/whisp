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
	mat4 transmat;
public:
	struct Extent
	{
		vec3 min, max;
	};
private:
	Extent m_extent;
public:
	Model(Mesh* mesh);
	Model(Mesh* mesh, const mat4& transmat);

	Model(const Model& other) = default;
	Model& operator=(const Model& other) = default;

	~Model();

	const Mesh& mesh() const;

	mat4& transMat();
	const vector<Vertex>& vertices();
	vector<EdgeRef> edges();
	vector<FaceRef> faces();

	const Extent& extent();

	void draw();
	void drawBuffered(); //with buffered objects
	void drawNative();	//native gl calls

	void updateDraw();

private:
	void updateExtent();
	void updateVertices();
};
#endif /* MODEL_HPP */
