#include "diamondMesh.hpp"

#include <vector>

#include "MathStuff.h"

using std::vector;

Mesh diamondMesh(int facets, float radius, float height)
{
	float r = radius;
	float h = height; //height

	vector<vec3> vertices(facets + 2, vec3(0.0f, 0.0f, 0.0f));
	vertices[0] = vec3(0.0f, -(h/2), 0.0f);
	vertices[1] = vec3(0.0f, h/2, 0.0f);

	// 3 for every triangle
	// 2 because 2 points of the diamond
	vector<unsigned int> indices(facets*3*2, 0);

	const float in_between_y = vertices[0].y + ((vertices[1].y - vertices[0].y) / 2);
	const float segment_constant = TAU / facets;
	for ( int i = 0; i < facets; ++i )
	{
		//vertices
		float circle_x = sin(segment_constant * i) * r;
		float circle_y = cos(segment_constant * i) * r;
		vertices[2+i] = vec3(circle_x, in_between_y, circle_y);

		//0 and 1 are the vertices on the points, top and bottom, respectively
		//top facet
		indices[6*i+0] = 0;
		indices[6*i+1] = 2 + (i+1)%facets;
		indices[6*i+2] = 2 + i;
		//botom facet
		indices[6*i+3] = 1;
		indices[6*i+4] = 2 + (i+1)%facets;
		indices[6*i+5] = 2 + i;
	}

	return Mesh(	vertices.begin(), vertices.end(),
					indices.begin(), indices.end()
			   );

}
