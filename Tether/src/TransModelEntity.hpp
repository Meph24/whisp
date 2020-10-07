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
public:
	TransModelEntity(const Model& model);

	const ModelObject& modelObject() const;

	vec3 rot, scale, drot;

	AABB aabb(float tick_seconds, TickServiceProvider* tsp);

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 );

	void tick(	const SimClock::time_point& t,
						TickServiceProvider* tsp
					 );

	void collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);

	vector<Model::ConvexPart> convexParts() const;

	vector<Vertex> vertices (float tick_time) const;

	vector<EdgeRef> edges(float tick_time) const;

	vector<FaceRef> faces(float tick_time) const;

	void react(float tick_time);
};

#endif /* TRANSMODELENTITY_HPP */
