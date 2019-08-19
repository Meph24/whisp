#include "ModelEntity.hpp"

#include <cmath>

#include "Mesh.hpp"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "Frustum.h"

ModelEntity::ModelEntity(spacevec position, const Model& model)
:
	model(model)
{
	surviveClearing = true;

	bb = AABB(position);
	pos = position;
	v.set0();
}

ModelEntity::~ModelEntity()
{}


void ModelEntity::draw(	
					Timestamp ts, 
					Frustum* viewFrustum, 
					ChunkManager* cm, 
					DrawServiceProvider* dsp
				 )
{
	float tickOffset = ts-lastTick;
//	if(!viewFrustum->inside(bb)) return;
	
	spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
	vec3 interPosMeters = cm->toMeters(interPos);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();

	//apply position
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	model.draw();

	glPopMatrix();
}

void ModelEntity::tick
				(
					Timestamp t,
					TickServiceProvider* tsp
				)
{
	lastTick = t;
	ChunkManager* cm = tsp->getChunkManager();

	pos=cm->clip(pos, true);
	bb = AABB(pos, cm->fromMeters(model.extent()));
}

