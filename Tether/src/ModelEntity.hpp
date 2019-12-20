#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"
#include "Timestamp.h"
#include "CumulativeMat.hpp"

#include "Collider.hpp"

#include "collisionl2.hpp"

#include "glmutils.hpp"
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;


class ModelEntity : public Entity , public Collider
{
	Model m_model;

	vec3 m_rot;
	vec3 m_rotv;

	bool model_properties_need_update;
public:
	ModelEntity(const Model& model);
	~ModelEntity();

	Model& model();

	void move(spacevec d);

	const vec3& rot() const;
	const vec3& rotv() const;
	
	void rotate(vec3 rotation);
	void spin(vec3 rotational_velocity);

	void resetRotation(bool reset_rotv = true);

	spacevec getPos() const;

	
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
	Model* colModel();
	void collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);
	void colReact();
};

#endif /* ENTITYDIAMOND_HPP */
