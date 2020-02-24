#ifndef COLLISIONL2_HPP
#     define COLLISIONL2_HPP

#include<array>
#include"glmutils.hpp"
#include<vector>

#include "IWorld.h"
#include "Model.hpp"
#include "ModelEntity.hpp"

using glm::vec3;
using std::array;
using std::vector;

namespace collisionl2
{
	/* cursor */

struct CollisionFeature
{
	enum TYPE
	{
		Unknown = 0,
		Vertex = 1,
		Edge = 2,
		Face = 3,

		NUM
	};

	CollisionFeature();
	CollisionFeature(const VertexRef& v);
	CollisionFeature(const EdgeRef& er);
	CollisionFeature(const FaceRef& fr);

	TYPE type;
	array<unsigned int, 3> mesh_indices;
};

struct SubmodelCollision
{
	float time;
	vec3 pos;
	CollisionFeature feat0, feat1;

	SubmodelCollision();
	SubmodelCollision	(	float time, vec3& pos, 
							CollisionFeature&& cf0,
							CollisionFeature&& cf1
						);

	bool operator==(const SubmodelCollision& other);
	bool operator!=(const SubmodelCollision& other);
	bool operator<(const SubmodelCollision& other);
	bool operator>(const SubmodelCollision& other);
	bool operator<=(const SubmodelCollision& other);
	bool operator>=(const SubmodelCollision& other);
};

void linearInterpolation_R0_FaceVertex(
		vector<SubmodelCollision>& colls_out,
		float t0, float t1,
		const Collider& c0, const Collider& c1,
		const vec3& r0_c1_pos_t0, const vec3& r0_c1_pos_t1
	);

void linearInterpolation_R0_EdgeEdge(
		vector<SubmodelCollision>& colls_out,
		float t0, float t1,
		const Collider& c0, const Collider& c1,
		const vec3& r0_c1_pos_t0, const vec3& r0_c1_pos_t1
	);

vector<SubmodelCollision> linearInterpolation_R0(
		float t0, float t1, IWorld* iworld,	
		const Collider& c0, const Collider& c1
		);

}/* namespace collisionl2 */
#endif /* COLLISIONL2_HPP */
