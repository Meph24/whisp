#ifndef ENTITYDIAMOND_HPP
#     define ENTITYDIAMOND_HPP

#include "MatrixLib.h"

#include "Entity.h"
#include "Model.hpp"
#include "Timestamp.h"

class EntityDiamond : public Entity
{
	Model model;
public:
	EntityDiamond(spacevec position, const Model& model);
	~EntityDiamond();

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
