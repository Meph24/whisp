#include "ModelEntity.hpp"

#include <cmath>

#include "Mesh.hpp"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "Frustum.h"
#include "MatrixLib2.h"

ModelEntity::ModelEntity(const Model& model)
:
	model(model)
{
	surviveClearing = true;

	v.set0();
}

ModelEntity::~ModelEntity()
{}

void ModelEntity::move(spacevec d)
{
	this->pos += this->pos;
}

void ModelEntity::rotate(vec3 rot)
{
	this->m_rot += rot;
}

void ModelEntity::scale(vec3 scale)
{
	this->m_scale += scale;
}

vec3 ModelEntity::getRotation() const
{
	return m_rot;
}

vec3 ModelEntity::getScale() const
{
	return m_scale;
}

spacevec ModelEntity::getPos() const
{
	return this->pos;
}

float ModelEntity::groundedDistance() const
{
	return model.groundDistance();
}

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

	//entity attribute changes go here

	bb = AABB(pos, cm->fromMeters(model.extent()));
}

