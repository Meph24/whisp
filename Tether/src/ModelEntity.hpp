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

	//temporary solution to find extent in a certain direction
	// here the direction is hardcoded to below
	virtual float groundedDistance();

	virtual void draw(	Timestamp ts, 
						Frustum* viewFrustum, 
						ChunkManager* cm, 
						DrawServiceProvider* dsp
					 );

	virtual void tick(	Timestamp t,
						TickServiceProvider* tsp
					 );

	//ColliderInterface
	Model* colModel();
	void collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp);
	void colReact();
};

#endif /* ENTITYDIAMOND_HPP */
