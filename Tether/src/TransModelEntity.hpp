#ifndef TRANSMODELENTITY_HPP
#     define TRANSMODELENTITY_HPP

#include<vector>

#include "glmutils.hpp"

#include "Entity.h"
#include "Collider.hpp"

#include "ModelObject.hpp"

using std::vector;
using glm::vec3; using glm::mat4;


class TransModelEntity : public Entity, public Collider
{
	ModelObject mo;

	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;
	vec3 tick_begin_scale, tick_begin_dscale;
public:
	TransModelEntity(const Model& model);

	const ModelObject& modelObject() const;

	vec3 rot, scale, drot, dscale;

	bool physics;

	AABB aabb(float tick_seconds, TickServiceProvider* tsp);

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						Perspective& perspective
					 );

	void tick(	const SimClock::time_point& t,
						TickServiceProvider* tsp
					 );

	void collide(DualPointer<Collider> other, microseconds delta_time, TickServiceProvider& tsp);

	vector<Model::ConvexPart> convexParts() const;

	vector<Vertex> vertices (microseconds tick_time);
	vector<EdgeRef> edges() const;
	vector<FaceRef> faces() const;

	void react(microseconds tick_time);

	void onSpawn( TickServiceProvider* tsp );
};

#endif /* TRANSMODELENTITY_HPP */
