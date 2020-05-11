#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"
#include "Timestamp.h"

#include "Collider.hpp"

#include "collisionl2.hpp"

#include "glmutils.hpp"
#include <glm/glm.hpp>

#include "gjk.hpp"

using glm::vec3;
using glm::vec4;


class ModelEntity : public Entity , public Collider
{
	const Model& m_model;

	spacevec tick_begin_pos, tick_begin_v;

public:
	ModelEntity(const Model& model);
	~ModelEntity();

	const Model& model();
	
	virtual void draw(	Timestamp ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 );

	/**
	 * @brief Calculates the AABB for the managed Model in Space.
	 * 
	 * This calculation only is successfull, if changes on pos and v
	 *	of this ModelEntity have not been done already in the current
	 *	tick!
	 *
	 * @param tick_seconds The time-length of the current tick in seconds.
	 * @param tsp A Tick Service Provider for relativation.
	 *
	 * @return AABB for the current tick.
	 */
	AABB aabb(float tick_seconds, TickServiceProvider* tsp);

	virtual void tick(	Timestamp t,
						TickServiceProvider* tsp
					 );

	//ColliderInterface
	
	void collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);

	Collider::TYPE type() const; 

	vector<Model::ConvexPart> convexParts() const;
	
	vector<Vertex> vertices (float tick_time) const;

	vector<EdgeRef> edges(float tick_time) const;

	vector<FaceRef> faces(float tick_time) const;

	void react(float tick_time);
};

#endif /* ENTITYDIAMOND_HPP */
