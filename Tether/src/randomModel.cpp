#include "randomModel.hpp"

#include <random>

namespace randommodel {

unsigned int randomIndex(unsigned int maxIndex)
{
	return rand() % maxIndex;
}

float randomFloat(float low, float high)
{
	return low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
}

float randVertexCoordinate()
{
	return randomFloat(-1.0f, 1.0f);
}

vec3 randVertex()
{
	return vec3(randVertexCoordinate(),randVertexCoordinate(),randVertexCoordinate());
}

//create a Random Mesh
Mesh randomMesh(unsigned int vertex_count, float triangle_rate)
{
	if(vertex_count < 3) vertex_count = 3;
	vector<vec3> vertices;
	for (unsigned int i = 0; i < vertex_count; ++i )
	{
		vertices.emplace_back(randVertex());
	}

	float f_indices_count = (float)vertex_count * 3.0 * triangle_rate;
	//round down to a multiple of 3
	unsigned int indices_count = f_indices_count;
	indices_count -= (indices_count%3);
	vector<unsigned int> indices;
	for ( unsigned int i = 0; i < indices_count; ++i )
	{
		indices.push_back(randomIndex(vertex_count));
	}

	return Mesh(vertices.begin(), vertices.end(),
				indices.begin(), indices.end());
}

} /* namespace randommodel */
