#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"
#include "Timestamp.h"
#include "CumulativeMat.hpp"


class ModelEntity : public Entity
{
	Model model;
	vec3 m_rot, m_scale;
public:
	ModelEntity(const Model& model);
	~ModelEntity();

	void move(spacevec d);
	void rotate(vec3 rot);
	void scale(vec3 scale);

	vec3 getRotation() const;
	vec3 getScale() const;
	spacevec getPos() const;

	//temporary solution to find extent in a certain direction
	// here the direction is hardcoded to below
	virtual float groundedDistance() const;

	virtual void draw(	Timestamp ts, 
						Frustum* viewFrustum, 
						ChunkManager* cm, 
						DrawServiceProvider* dsp
					 );

	virtual void tick(	Timestamp t,
						TickServiceProvider* tsp
					 );
};

#endif /* ENTITYDIAMOND_HPP */
