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

	mat4 transmat;
public:
	ModelEntity(const Model& model);
	~ModelEntity();

	Model& model();

	void move(spacevec d);

	vec3 rot() const;
	vec3 rotv() const;
	
	void rotate(vec3 rot);
	void spin(vec3 rot);

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
	void colSetRealPos(const spacevec& newpos);
	void colSetRealV(const spacevec& newv);
	Model* colModel();
	void collide(Collider* other, float time, TickServiceProvider& tsp);
};

#endif /* ENTITYDIAMOND_HPP */
