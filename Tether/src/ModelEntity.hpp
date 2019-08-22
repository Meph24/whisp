#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "Entity.h"

#include "Spacevec.h"

#include "Model.hpp"
#include "Timestamp.h"

class ModelEntity : public Entity
{
	Model model;
public:
	ModelEntity(const Model& model);
	~ModelEntity();

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
