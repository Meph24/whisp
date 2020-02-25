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

} /* namespace randommodel */
#endif /* RANDOMMODEL_HPP */
