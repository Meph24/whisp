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

	bool isVertexFace() const;
	bool isEdgeEdge() const;
};

void linearInterpolationFaceVertex(
		vector<SubmodelCollision>& colls_out,
		float delta_time,
		Collider* c0, Collider* c1,
		vec3 o0_pos, vec3 o1_pos, vec3 o0_dpos, vec3 o1_dpos;
	);

void linearInterpolationEdgeEdge(
		vector<SubmodelCollision>& colls_out,
		float delta_time,
		Collider* c0, Collider* c1,
		vec3 o0_pos, vec3 o1_pos, vec3 o0_dpos, vec3 o1_dpos;
	);

vector<SubmodelCollision> linearInterpolation(	
		float delta_time;,
		IWorld* iworld,
		Collider* c0, Collider* c1
	);

}/* namespace collisionl2 */
#endif /* COLLISIONL2_HPP */
