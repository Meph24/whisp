#include "static_intersection.hpp"

bool rayIntersectsPlane	(	const vec3& pr,
							const vec3& pn,
							const vec3& rp,
							const vec3& rv,
							vec3* pos_out,
							float* ray_coeff_out
						)
{
	float divisor = glm::dot(rv, pn);

	if(divisor == 0.0f)
		return false;

	float coeff = (glm::dot(rp-pr, pn / divisor);

	if(ray_coeff_out)
		*ray_coeff_out = coeff;

	if(pos_out)
		*pos_out = rp + rv * coeff;

	return true;
}

bool rayIntersectsPlane	(	const vec3& pr, 
							const vec3& p1, 
							const vec3& p2,
							const vec3& rp,
							const vec3& rv,
							vec3* pos_out,
							float* ray_coeff_out
						)
{	
	intersectsPlane(pr, glm::cross(p1,p2), rp, rv, pos_out, ray_coeff_out);
}


bool lineIntersectsPlane	(	const vec3& pr, 
								const vec3& pn,
								const vec3& lr,
								const vec3& lv,
								vec3* pos_out = nullptr,
								float* line_coeff_out = nullptr
							)
{
	float ray_coeff;
	if(!rayIntersectsPlane(pr, pn, lr, lv, pos_out, &ray_coeff))
		return false;

	if (ray_coeff < 0.0f && ray_coeff > 1.0f)
		return false;
	
	if(line_coeff_out) *line_coeff_out = ray_coeff;
	return true;
}

bool lineIntersectsPlane	(	const vec3& pr, 
								const vec3& p1, 
								const vec3& p2,
								const vec3& lr,
								const vec3& lv,
								vec3* pos_out = nullptr,
								float* line_coeff_out = nullptr
							)
{
	return lineIntersectsPlane(pr, glm::cross(p1, p2), lr, lv, pos_out, line_coeff_out);
}

vec2 planePointCoefficients(const vec3& pr, 
							const vec3& p1, 
							const vec3& p2,
							const vec3& p,
							bool* solved_out
						   )
{
	vec2 coeff;
	//solve the equation
	//pr + x * p1 + y * p2 == intersection_pos
	
	vec3 pv1 = p1 - pr;
	vec3 pv2 = p2 - pr;
	vec3 iv = intersection_pos - pr;


	//solve only for x and y
	//by solving gauss and following its calculation steps
	//	we get a formula for x and y
	//	pv1.x pv2.x | iv.x		1 0 | x 	
	//	pv1.x pv2.y | iv.y ---> 0 1 | y
	//	x = (iv.x*pv2.y-pv2.x*iv.y)/(pv1.x*pv2.y-pv2.x*pv1.x)
	//	y = (pv1.x*iv.y-iv.x*pv1.x)/(pv1.x*pv2.y-pv2.x*pv1.x)


	float divisor = (pv1.x*pv2.y-pv2.x*pv1.x);

	if(divisor == 0.0f)
	{
		if(solved_out) *solved_out = false;
		return coeff; //empty return
	}
	coeff.x = (iv.x*pv2.y-pv2.x*iv.y)/divisor;
	coeff.y = (pv1.x*iv.y-iv.x*pv1.x)/divisor;

	if(solved_out)
	{
		//check back with z
		//if calculated coefficients do not aprly correctly to z
		//	then the given intersection point is not on the given plane
		*solved_out = 
			(pv1.z * coeff.x + pv2.z * coeff.y == iv.z)
			? true : false;
	}

	return coeff;
}

bool pointInTriangle(	const vec3& pr, 
						const vec3& p1, 
						const vec3& p2,	
						const vec3& p,
						vec2* plane_coeff_out
					)
{
	//solve the equation
	//pr + a * p1 + b * p2 == intersection_pos 
	//to get coefficients a and b
	vec2 coeff;
	bool equation_solvable = false;
	coeff = planePointCoefficients(pr, p1, p2, intersection_pos, &equation_solvable);

	if(!equation_solvable) return false;	

	if(plane_coeff_out) *plane_coeff_out = coeff;

	//the coefficients a and b have to satisfy the Triangle property
	// a >= 0 and b >= 0 and a + b <= 1
	return		coeff.x >= 0
			&&	coeff.y >= 0
			&&	coeff.x+coeff.y <= 1;
}

bool rayIntersectsTriangle(	const vec3& pr,
							const vec3& p1,
							const vec3& p2,
							const vec3& rp,
							const vec3& rv,
							vec3* pos_out,
							vec3* coeff_out
						  )
{
	vec3 pos;
	if(!rayIntersectsPlane	(	pr, p1, p2, rp, rv, &pos,
								(coeff_out)?(&(coeff_out.z)):nullptr			
							)
	  )
	{
		return false;
	}

	if(pos_out) *pos_out = pos;

	vec2 plane_coeff;
	if(! pointInTriangle(pr, p1, p2, pos, &plane_coeff))
		return false;

	if(coeff_out)
	{
		coeff_out.x = plane_coeff.x;
		coeff_out.y = plane_coeff.y;
		//coeff_out.y was set by rayIntersectsPlane
	}
	return true;
}


bool lineIntersectsTriangle(const vec3& pr,
							const vec3& p1,
							const vec3& p2,
							const vec3& lr,
							const vec3& lv,
							vec3* pos_out = nullptr,
							vec3* coeff_out
								=nullptr
							)
{
	vec3 coeff;
	if(! rayIntersectsTriangle(pr, p1, p2, lr, lv, pos_out, &coeff))
		return false;

	if(coeff.z < 0.0f || coeff.z > 1.0f) return false;

	if(coeff_out) *coeff_out = coeff;
	return true;
}

bool pointInParallelogram(	const vec3& pr, 
							const vec3& p1, 
							const vec3& p2,
							const vec3& intersection_pos,
							vec2* plane_coeff_out
						 )
{
	//solve the equation
	//pr + a * p1 + b * p2 == intersection_pos 
	//to get coefficients a and b
	vec2 coeff;
	bool equation_solvable = false;
	coeff = intersectionsCoefficients(pr, p1, p2, intersection_pos, &equation_solvable);

	if(!equation_solvable) return false;

	if(plane_coeff_out) *plane_coeff_out = coeff;

	//the coefficients a and b have to satisfy the Paralellogram property
	// 0 <= a <= 1 and 0 <= b <= 1
	return		coeff.x >= 0 && coeff.x <= 1
			&&	coeff.y >= 0 && coeff.y < 1;
}

bool rayIntersectsParallelogram(	const vec3& pr,
									const vec3& p1,
									const vec3& p2,
									const vec3& rp,
									const vec3& rv,
									vec3* pos_out,
									vec3* coeff_out
							   )
{
	vec3 pos;
	float ray_coeff;
	if(!rayIntersectsPlane(pr, p1, p2, rp, rv, &pos, &ray_coeff))
	{
		return false;
	}

	if(pos_out) *pos_out = pos;

	vec2 plane_coeff;
	if(! pointInParallelogram(pr, p1, p2, *pos_out, &plane_coeff))
		return false;

	if(coeff_out)
	{
		coeff_out->x = plane_coeff.x;
		coeff_out->y = plane_coeff.y;
		coeff_out->z = ray_coeff;
	}

	return true;
}


bool lineIntersectsParallelogram(	const vec3& pr,
									const vec3& p1,
									const vec3& p2,
									const vec3& lr,
									const vec3& lv,
									vec3* pos_out = nullptr,
									vec3* coeff_out = nullptr
								)
{
	vec3 coeff;
	if(! rayIntersectsParallelogram(pr, p1, p2, lr, lv, pos_out, &coeff))
		return false;
	
	if(coeff.z < 0.0f || coeff.z > 1.0f) return false;

	if(coeff_out) *coeff_out = coeff;
	return true;
}
