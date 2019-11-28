#ifndef RANDOMMODEL_HPP
#     define RANDOMMODEL_HPP

#include <random>
#include "glmutils.hpp"


#include "Mesh.hpp"
#include "Model.hpp"

namespace randommodel {

unsigned int randomIndex(unsigned int maxIndex);
float randomFloat(float low, float high);
float randVertexCoordinate();
vec3 randVertex();
Mesh randomMesh(unsigned int vertex_count, float triangle_rate = 3.0);

template<typename container_type>
Model randomModel(	const container_type& container,
					float scale_range_low = 0.5,
					float scale_range_high = 5.0f)
{
	return Model(	container[rand() % container.size()].get(),
					glm::scale( vec3(
								randomFloat(0.5f, 2.0f),
								randomFloat(0.5f, 2.0f),
								randomFloat(0.5f, 2.0f)
							  ))	
				);
}

} /* namespace randommodel */
#endif /* RANDOMMODEL_HPP */
