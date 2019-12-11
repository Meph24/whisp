#ifndef TORUSMESH_HPP
#     define TORUSMESH_HPP

#include "Mesh.hpp"

float torusX(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	);
float torusY(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	);
float torusZ(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	);

/**
 * @brief Returns the mesh in form of a torus.
 *
 * A Torus is a shape commonly referred to as a donut.
 * It is a ring made out of a tube.
 * We refer to the ring as the torus.
 * We refer to the tube as tube.
 * Since a torus is a circular object, it needs to be discretized by samples.
 * Samples here then refer to the amount of vertices on a respective circle
 * (torus or tube)
 *
 * @param torus_radius, defined by the distance from the torus center
 *		to the center of the tube. Inner/Outer radius of the torus is
 *		then defined by torus_radius -/+ tube_radius respectively.
 * @param torus_samples
 * @param tube_radius
 * @param tube_samples
 *
 * @return 
 */
Mesh torusMesh(	float torus_radius, unsigned int half_torus_samples,
				float tube_radius, unsigned int tube_samples);


#endif /* TORUSMESH_HPP */
