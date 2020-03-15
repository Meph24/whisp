#include "torusMesh.hpp"

#include <cmath>
#include "glmutils.hpp"
#include "MathStuff.h"

float torusX(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	)
{
	return (torus_radius + tube_radius * cos(tube_angle)) 
			* cos(torus_angle);
}

float torusY(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	)
{
	return (torus_radius + tube_radius * cos(tube_angle)) 
			* sin(torus_angle);
}
float torusZ(	float torus_radius, float torus_angle, 
				float tube_radius, float tube_angle	)
{
	return tube_radius * sin(tube_angle);
}

Mesh torusMesh(	float torus_radius, unsigned int torus_samples,
				float tube_radius, unsigned int tube_samples)
{
	vector<vec3> vertices;
	vertices.reserve(torus_samples * tube_samples);
	vector<unsigned int> indices;
	indices.reserve(3 * 2 * torus_samples * tube_samples);
	
	float torus_angle = 0.0;
	float torus_angle_constant = TAU / torus_samples;
	float tube_angle = 0.0;
	float tube_angle_constant = TAU / tube_samples;

	//vertices

	for ( unsigned int torus_index = 0; torus_index < torus_samples; ++torus_index )
	{
		torus_angle = torus_angle_constant * torus_index;

		for ( unsigned int tube_index = 0; tube_index < tube_samples; ++tube_index )
		{
			tube_angle = tube_angle_constant * tube_index;

			vertices.emplace_back(	torusX(	torus_radius, torus_angle,
											tube_radius, tube_angle		),
									torusY(	torus_radius, torus_angle,
											tube_radius, tube_angle		),
									torusZ(	torus_radius, torus_angle,
											tube_radius, tube_angle		)
									);
		}
	}



	//for every vertex we generate 2 triangles
	//tuple (r, t) with r = torus_index and t = tube_index
	//((0, 0), (0, 1), (1, 0)) and ((0, 0), (1, 0), (1, -1))
	
	// r = 0
	unsigned int torus_index;
	unsigned int torus_offset; 
	//t = 0
	unsigned int tube_index;
	//r = 1
	unsigned int next_torus_index;
	unsigned int next_torus_offset;
	//t = 1
	unsigned int next_tube_index;
	//t = -1
	unsigned int last_tube_index;

	// (0,0) == vi
	unsigned int vi_00;
	// (0,1)
	unsigned int vi_01;
	// (1, 0)
	unsigned int vi_10;
	// (1, -1)
	unsigned int vi_11;

	//indices
	for ( unsigned int vi = 0; vi < vertices.size(); ++vi )
	{

		//TODO remove , this is debugging code
		//if(vi > 10) break;
		
		torus_index = vi / tube_samples;
		torus_offset = torus_index * tube_samples;

		tube_index = vi % tube_samples;

		next_torus_index = (torus_index + 1) % torus_samples;
		next_torus_offset = next_torus_index * tube_samples;

		next_tube_index = (tube_index + 1) % tube_samples;

		//because we subtract on unsigned ints we need a little offset
		last_tube_index = (tube_samples + tube_index - 1) % tube_samples;

		vi_00 = vi;
		vi_01 = torus_offset + next_tube_index;
		vi_10 = next_torus_offset + tube_index;
		vi_11 = next_torus_offset + last_tube_index;
	
		//the ordering stems from winding conventions	
		indices.push_back(vi_00);
		indices.push_back(vi_10);
		indices.push_back(vi_01);

		indices.push_back(vi_00);
		indices.push_back(vi_11);
		indices.push_back(vi_10);
	}

	return Mesh(vertices.begin(), vertices.end(),
				indices.begin(), indices.end());
}
