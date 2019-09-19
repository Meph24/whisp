#ifndef COLLIDEL2_HPP
#     define COLLIDEL2_HPP



bool intersectsPlane(	const vec3& pp,
						const vec3& pn,
						const vec3& rp,
						const vec3& rv,
						vec3* pos_out
					)
{
	float divisor = glm::dot(rv, pn);

	if(divisor == 0.0f)
		return false;

	if(pos_out)
		*pos_out = rp + rv * (glm::dot(rp-pp, pn / divisor);

	return true;
}

bool intersectsPlane(	const vec3& pp, 
						const vec3& p1, 
						const vec3& p2,
						const vec3& rp,
						const vec3& rv,
						vec3& pos_out
					  )
{	
	intersectsPlane(pp, glm::cross(p1,p2), rp, rv, pos_out)
}

pair<float, float> intersectionCoefficients(	const vec3& pp, 
												const vec3& p1, 
												const vec3& p2,
												const vec3& intersection_pos,
												bool* solved
										   )
{
	pair<float, float> coefficients;
	//solve the equation
	//pp + x * p1 + y * p2 == intersection_pos
	//the coefficients x and y have to satisfy the Paralellogram property
	// 0 < x < 1 and 0 < y < 1
	
	//pp => 0
	vec3 pv1 = p1 - pp;
	vec3 pv2 = p2 - pp;
	vec3 iv = intersection_pos - pp;

	//solve only for x and y
	mat2x3 solve_mat (pv1.x, pv1.y, pv2.x, pv2.y, iv.x, iv.y);
	solve_mat = glm::solveGauss(solve_mat);
	coefficients->first = solve_mat[0][2];
	coefficients->second = solve_mat[1][2];

	if(solved)
	{
		//check back with z
		*solved = 
			(pv1.z * coefficients->first + pv2.z * coefficients->second == iv.z)
			? true : false;
	}

	return coefficients;
}

bool checkIntersectionTriangle(	const vec3& pp, 
								const vec3& p1, 
								const vec3& p2,
								const vec3& intersection_pos,
								std::pair<float, float>* coefficients_out
								)
{
	//solve the equation
	//pp + a * p1 + b * p2 == intersection_pos 
	//to get coefficients a and b
	bool equation_solvable = false;
	coefficients_out = intersectionsCoefficients(pp, p1, p2, intersection_pos, &equation_solvable);

	if(!equation_solvable) return false;	

	//the coefficients a and b have to satisfy the Triangle property
	// x > 0 and y > 0 and y + x < 1
	return		x > 0
			&&	y > 0
			&&	x+y < 1;
}

bool checkIntersectionParallelogram(const vec3& pp, 
									const vec3& p1, 
									const vec3& p2,
									const vec3& intersection_pos,
									std::pair<float, float>* coefficients_out
								   )
{
	//solve the equation
	//pp + a * p1 + b * p2 == intersection_pos 
	//to get coefficients a and b
	bool equation_solvable = false;
	coefficients_out = intersectionsCoefficients(pp, p1, p2, intersection_pos, &equation_solvable);

	if(!equation_solvable) return false;	

	//the coefficients a and b have to satisfy the Paralellogram property
	// 0 < x < 1 and 0 < y < 1

	
	return		x > 0 && x < 1
			&&	y > 0 && y < 1;
}


collideL2LinearInterpolation(ModelEntity& m0, ModelEntity& m1, float delta_t)
{
	//relativate pos and vel
	//m0 is relaxed
	vec3 m0_pos = vec3(0.0f);
	vec3 m0_v = m0.v;
	//m1 is relativized
	vec3 m1_t0_pos = m1.pos - m0.pos;
	vec3 m1_v = m1.v - m0.v;

	//m0 is not moving because of relativization
	//only m1 therefor has t0 and t1 position
	vec3 delta_pos = m1_v * delta_t;
	vec3 m1_t1_pos = m1_t0_pos + delta_pos;
	
	vector<vec3> m1_t0_vertices = m1.vertices(m1_t0_pos);
	vector<vec3> m1_t0_vertices = m1.vertices(m1_t1_pos);

	vector<std::array<vec3, 3> > m0_triangles = m0

	//swept volume of m1
	//all the edges of m1 are needed
	vector<pair<unsigned int, unsigned int> > edge_indices = m1.edgeIndices();
}

vector<vec3> vertices, vector<pair<unsigned int, unsigned int> >

#endif /* COLLIDEL2_HPP */
