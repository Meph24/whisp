#include "ModelEntity.hpp"

#include <cmath>
#include <algorithm>

#include "glmutils.hpp"
#include <glm/gtx/transform.hpp>

#include "Mesh.hpp"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "Frustum.h"

#include "InteractFilterAlgoSym.h"

using glm::vec4;
using glm::vec3;
using glm::mat4;

Model& ModelEntity::model()
{
	return m_model;
}

ModelEntity::ModelEntity(const Model& model)
:
	m_model(model)
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
	cummat = cummat * 
		(
		  glm::rotateDeg(rot.x, glm::vec3(1, 0, 0))
		* glm::rotateDeg(rot.y, glm::vec3(0, 1, 0))
		* glm::rotateDeg(rot.z, glm::vec3(0, 0, 1))
		);
}

void ModelEntity::scale(vec3 scale)
{
	cummat = glm::scale(cummat, scale);
}

spacevec ModelEntity::getPos() const
{
	return this->pos;
}

float ModelEntity::groundedDistance()
{
	return m_model.groundDistance();
}

void ModelEntity::draw(	
					Timestamp ts, 
					Frustum* viewFrustum, 
					ChunkManager* cm, 
					DrawServiceProvider* dsp
				 )
{
	float tickOffset = ts-lastTick;
	//if(!viewFrustum->inside(bb)) return;
	
	spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
	vec3 interPosMeters = cm->toMeters(interPos);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();

	//apply position
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	m_model.draw();

	glPopMatrix();
}

void ModelEntity::tick
				(
					Timestamp t,
					TickServiceProvider* tsp
				)
{
	float time = t - lastTick;
	lastTick = t;

	Collider::State old_state {pos, v};
	Collider::saveState(old_state);

	ChunkManager* cm = tsp->getChunkManager();

	//entity attribute changes go here
	if(glm::length(cm->toMeters(pos)) > 100)
		v = v * -1.0f;
	pos += v*time;

	bb = AABB(pos, cm->fromMeters(m_model.extent()));

	tsp->getIWorld()->collideAlgo->doChecks(
			(Collider*) this, (Entity*) this,
			time, *tsp);
}

// implementation of the Collider Interface
void ModelEntity::colSetRealPos(const spacevec& newpos)
{
	pos = newpos;
}

void ModelEntity::colSetRealV(const spacevec& newv)
{
	v = newv;
}

Model* ModelEntity::colModel()
{
	return &(model());
}

void ModelEntity::collide(Collider* other, float time, TickServiceProvider& tsp)
{
	vector<collisionl2::SubmodelCollision> collisions = 
		collisionl2::linearInterpolation(
				0.0, time,
				*(colModel()), *(other->colModel()),
				tsp.getIWorld()->toMeters(colSavedPos()),
				tsp.getIWorld()->toMeters(other->colSavedPos()),
				tsp.getIWorld()->toMeters(colSavedV()),
				tsp.getIWorld()->toMeters(other->colSavedV())
			);

	if(collisions.empty()) return;

	//l3 collision resolvance (simple)
	//get the first collision time

	//move the objects to the time of the first collision
	// only by percentage to avoid further collisions
	float step_to_collision = std::min_element(collisions.begin(), collisions.end())->time * 0.9999;
	this->colSetRealPos(colSavedPos() + colSavedV() * step_to_collision);
	other->colSetRealPos(other->colSavedPos() + other->colSavedV() * step_to_collision);

	//cancel further movements
	spacevec vnull; vnull.set0();
	this->colSetRealV( vnull );
	other->colSetRealV( vnull );
}

