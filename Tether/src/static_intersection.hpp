#ifndef STATIC_INTERSECTION_HPP
#     define STATIC_INTERSECTION_HPP


#include <glm/glm.hpp>
#include "glmutils.hpp"

using glm::vec2;
using glm::vec3;


/**
 * @brief Calculates, if a Ray intersects with a plane.
 *
 * A Ray expands infinitely in both directions.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	Plane Root is a point on the plane.
 * @param pn	Plane Normal.
 * @param rr	Ray Root is a point on the ray.
 * @param rv	Ray Vector, rr + rv is a point on the ray, but not rr.
 * @param pos_out	vec3* optional output of 
 *						the calculated intersection position,
 * @param ray_coeff_out	float* optional ouput of 
 *						the calculated coefficient t for which
 *						pos_out = rr + rv * t   is true,
 *
 * @return bool wether or not an intersection exists
 */
bool rayIntersectsPlane	(	const vec3& pr,
							const vec3& pn,
							const vec3& rr,
							const vec3& rv,
							vec3* pos_out = nullptr,
							float* ray_coeff_out = nullptr
						);


/**
 * @brief Calculates, if a Ray intersects with a plane.
 *
 * A Ray expands infinitely in both directions.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	Plane Root is a point on the plane.
 * @param p1	Plane Vector 1, pr + p1 is point on the plane.
 * @param p2	Plane Vector 2, pr + p2 is point on the plane,
 *					p1 and p2 not parallel
 * @param rr	Ray Root is a point on the ray.
 * @param rv	Ray Vector, rr + rv is a point on the ray, but not rr.
 * @param pos_out	optional output of 
 *						the calculated intersection position,
 * @param ray_coeff_out	optional ouput of 
 *						the calculated coefficient t for which
 *						pos_out = rr + rv * t   is true,
 *
 * @return bool wether or not an intersection exists
 */
bool rayIntersectsPlane	(	const vec3& pr, 
							const vec3& p1, 
							const vec3& p2,
							const vec3& rr,
							const vec3& rv,
							vec3* pos_out = nullptr,
							float* ray_coeff_out = nullptr
						);



/**
 * @brief Calculates, if a line intersects with a plane.
 *
 * A line is finite, starts on one point and extents until another point.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	Plane Root is a point on the plane.
 *					p1 and p2 not parallel
 * @param pn	Plane Normal.
 * @param lr	Line Root is an end point of the line.
 * @param lv	Line Vector, lr+lv is the other end point of the line.
 * @param pos_out	optional output of 
 *						the calculated intersection position,
 * @param line_coeff_out	optional ouput of 
 *						the calculated coefficient t for which
 *						pos_out = lr+lv*t   is true,
 *
 * @return bool wether or not an intersection exists
 */
bool lineIntersectsPlane	(	const vec3& pr, 
								const vec3& pn,
								const vec3& lr,
								const vec3& lv,
								vec3* pos_out = nullptr,
								float* line_coeff_out = nullptr
							);


/**
 * @brief Calculates, if a line intersects with a plane.
 *
 * A line is finite, starts on one point and extents until another point.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	Plane Root is a point on the plane.
 * @param p1	Plane Vector 1, pr+p1 is point on the plane.
 * @param p2	Plane Vector 2, pr+p2 is point on the plane,
 *					p1 and p2 not parallel
 * @param lr	Line Root is an end point of the line.
 * @param lv	Line Vector, lr+lv is the other end point of the line.
 * @param pos_out	optional output of 
 *						the calculated intersection position,
 * @param line_coeff_out	optional ouput of 
 *						the calculated coefficient t for which
 *						pos_out = lr+lv*t   is true,
 *
 * @return bool wether or not an intersection exists
 */
bool lineIntersectsPlane	(	const vec3& pr, 
								const vec3& p1, 
								const vec3& p2,
								const vec3& lr,
								const vec3& lv,
								vec3* pos_out = nullptr,
								float* line_coeff_out = nullptr
							);



/**
 * @brief Calculates coefficients for plane vectors to meet a certain point.
 *
 * @param pr	Plane Root is a point on the plane.
 * @param p1	Plane Vector 1, pr + p1 is point on the plane.
 * @param p2	Plane Vector 2, pr + p2 is point on the plane,
 *					p1 and p2 not parallel
 * @param p		Point, thats part of the plane, which we want to know koefficients
 *					x and y to satisfy : pr + p1*x + p2*y = p
 * @param solved optional output wether the coefficients could be
 *					calculated or not
 *
 * @return vec2 of coefficients x and y
 */
vec2 planePointCoefficients(const vec3& pr, 
							const vec3& p1, 
							const vec3& p2,
							const vec3& p,
							bool* solved_out=nullptr
						   );


/**
 * @brief Check if a point lies inside a triangle.
 *
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	One triangle vertex.
 * @param p1	pr+p1 is another vertex of the triangle.
 * @param p2	pr+p2 is another vertex of the triangle, but not pr+p1.
 * @param p		The point in question.
 * @param plane_coeff_out optional output of 
 *					the coefficients x and y which satisfy
 *					pr + p1*x + p2*y = p
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool pointInTriangle(	const vec3& pr, 
						const vec3& p1, 
						const vec3& p2,
						const vec3& p,
						vec2* plane_coeff_out
							= nullptr
					);


/**
 * @brief Check if a ray intersects with a triangle.
 *
 * A Ray expands infinitely in both directions.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	One triangle vertex.
 * @param p1	pr+p1 is another vertex of the triangle.
 * @param p2	pr+p2 is another vertex of the triangle, but not pr+p1.
 * @param rr	Ray Root is a point on the ray.
 * @param rv	Ray Vector, rr + rv is a point on the ray, but not rr.
 * @param pos_out	optional output of 
 *						the calculated intersection position,
 * @param coeff_out optional output
 *					of the coefficients x, y and z which satisfy
 *					pr + p1*x + p2*y = rr + rv*z
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool rayIntersectsTriangle(	const vec3& pr,
							const vec3& p1,
							const vec3& p2,
							const vec3& rr,
							const vec3& rv,
							vec3* pos_out = nullptr,
							vec3* coeff_out
								=nullptr
						  );


/**
 * @brief Check if a line intersects with a triangle.
 *
 * A line is finite, starts on one point and extents until another point.
 *
 * @param pr	One triangle vertex.
 * @param p1	pr+p1 is another vertex of the triangle.
 * @param p2	pr+p2 is another vertex of the triangle, but not pr+p1.
 * @param lr	Line Root is an end point of the line.
 * @param lv	Line Vector, lr+lv is the other end point of the line.
 * @param pos_out	optional output of 
 *						the calculated intersection position,
 * @param coeff_out optional output
 *					of the coefficients x, y and z which satisfy
 *					pr + p1*x + p2*y = rr + rv*z
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool lineIntersectsTriangle(const vec3& pr,
							const vec3& p1,
							const vec3& p2,
							const vec3& lr,
							const vec3& lv,
							vec3* pos_out = nullptr,
							vec3* coeff_out
								=nullptr
							);


/**
 * @brief Check if a point lies inside a parallelogram.
 *
 * @param pr	One parallelogram vertex.
 * @param p1	pr+p1 is another vertex of the parallelogram connected to pr.
 * @param p2	pr+p2 is another vertex of the parallelogram connected to pr, 
 *					but not pr+p1.
 * @param p		The point in question.
 * @param plane_coeff_out	optional output of
 *					the coefficients x and y which satisfy
 *					pr + p1*x + p2*y = p
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool pointInParallelogram(	const vec3& pr, 
						const vec3& p1, 
						const vec3& p2,
						const vec3& p,
						vec2* plane_coeff_out = nullptr
					);



/**
 * @brief Check if a point lies with a parallelogram.
 *
 * A Ray expands infinitely in both directions.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	One parallelogram vertex.
 * @param p1	pr+p1 is another vertex of the parallelogram connected to pr.
 * @param p2	pr+p2 is another vertex of the parallelogram connected to pr, 
 *					but not pr+p1.
 * @param rr	Ray Root is a point on the ray.
 * @param rv	Ray Vector, rr + rv is a point on the ray, but not rr.
 * @param pos_out	vec3* optional output of 
 *						the calculated intersection position,
 *
 * @param coeff_out optional output
 *					of the coefficients x, y and z which satisfy
 *					pr + p1*x + p2*y = rr + rv*z
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool rayIntersectsParallelogram(	const vec3& pr,
									const vec3& p1,
									const vec3& p2,
									const vec3& rr,
									const vec3& rv,
									vec3* pos_out = nullptr,
									vec3* plane_coeff_out
										=nullptr
							   );

/**
 * @brief Check if a point lies with a parallelogram.
 *
 * A line is finite, starts on one point and extents until another point.
 * Optional Outputs only valid if function returns true.
 * Function may alter optional output values, despite returning false!
 * Function may alter optional output values, despite returning false!
 *
 * @param pr	One parallelogram vertex.
 * @param p1	pr+p1 is another vertex of the parallelogram connected to pr.
 * @param p2	pr+p2 is another vertex of the parallelogram connected to pr, 
 *					but not pr+p1.
 * @param lr	Line Root is an end point of the line.
 * @param lv	Line Vector, lr+lv is the other end point of the line.
 * @param pos_out	vec3* optional output of 
 *						the calculated intersection position,
 *
 * @param coeff_out optional output
 *					of the coefficients x, y and z which satisfy
 *					pr + p1*x + p2*y = rr + rv*z
 *
 * @return	bool, whether or not an intersection could be found 
 */
bool lineIntersectsParallelogram(	const vec3& pr,
									const vec3& p1,
									const vec3& p2,
									const vec3& lr,
									const vec3& lv,
									vec3* pos_out = nullptr,
									vec3* coeff_out = nullptr
							    );


#endif /* STATIC_INTERSECTION_HPP */

