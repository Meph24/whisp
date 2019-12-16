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

#include <iostream>

using std::cerr;

using glm::vec4;
using glm::vec3;
using glm::mat4;

Model& ModelEntity::model()
{
	return m_model;
}

ModelEntity::ModelEntity(const Model& model)
:
	m_model(model),
	m_rot(0.0f, 0.0f, 0.0f),
	m_rotv(0.0f, 0.0f, 0.0f)
{
	surviveClearing = true;

	v.set0();
}

ModelEntity::~ModelEntity()
{}

void ModelEntity::move(spacevec distance)
{
	this->pos += distance;
}

const vec3& ModelEntity::rot() const
{
	return m_rot;
}

const vec3& ModelEntity::rotv() const
{
	return m_rotv;
}

void ModelEntity::resetRotation(bool reset_rotv)
{
	if(reset_rotv) m_rotv = vec3(0.0f, 0.0f, 0.0f);
	rotate(-1 * m_rot);
}

void ModelEntity::rotate(vec3 rotation)
{
	m_rot += rotation;
	m_model.transMat() = m_model.transMat() * 
		(
		  glm::rotateDeg(rotation.x, glm::vec3(1, 0, 0))
		* glm::rotateDeg(rotation.y, glm::vec3(0, 1, 0))
		* glm::rotateDeg(rotation.z, glm::vec3(0, 0, 1))
		);
}

void ModelEntity::spin(vec3 rotational_velocity)
{
	m_rotv += rotational_velocity;
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
	//rotation already done by matrixmult in model.draw()
	//glRotatef(m_rot.x, 1.0f, 0.0f, 0.0f);
	//glRotatef(m_rot.y, 0.0f, 1.0f, 0.0f);
	//glRotatef(m_rot.z, 0.0f, 0.0f, 1.0f);
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

	//the collider needs to save the old state of the entity
	//to use it for its calculations
	Collider::State old_state {pos, v};
	Collider::saveState(old_state);

	ChunkManager* cm = tsp->getChunkManager();

	//entity attribute changes go here
	//this is code for collision simulation
	//entities turn around after flying away too far
	if(glm::length(cm->toMeters(pos)) > 100)
		v = v * -1.0f;

	//apply pos by velocity
	move(v*time);

	//apply rotation by rotational velocity
	rotate(m_rotv * time);

	bb = AABB(pos, cm->fromMeters(m_model.extent()));

	tsp->getIWorld()->collideAlgo->doChecks(
			(Collider*) this, (Entity*) this,
			time, *tsp);
}

// implementation of the Collider Interface

Model* ModelEntity::colModel()
{
	return &(model());
}

void ModelEntity::collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	vec3 o0_pos, o1_pos, o0_v, o1_v;
	o0_pos = vec3(0.0f, 0.0f, 0.0f);

	//rel
	o1_pos = tsp.getIWorld()->toMeters( other.pIF->colSavedPos() - colSavedPos());
	o0_v = vec3(0.0f, 0.0f, 0.0f);

	//rel
	o1_v = tsp.getIWorld()->toMeters( other.pIF->colSavedV() - colSavedV() );


	vector<collisionl2::SubmodelCollision> collisions = 
		collisionl2::linearInterpolation(
				0.0, delta_time,
				*(colModel()), *(other.pIF->colModel()),
				o0_pos, o1_pos, o0_v, o1_v
			);

	if(collisions.empty()) return;

	//l3 collision resolvance (simple)
	//get the first collision time

	//move the objects to the time of the first collision
	// only by percentage to avoid further collisions
	auto min_e = std::min_element(collisions.begin(), collisions.end());

	if(min_e->time > delta_time)
	{
		cerr << "ERROR : Collision to later time than tick!\n";
	}

	float step_to_collision =min_e->time * 0.9999;
	

	this->pos = colSavedPos() + colSavedV() * step_to_collision;
	other.e->pos = other.pIF->colSavedPos() + other.pIF->colSavedV() * step_to_collision;

	//cancel further movements
	this->colReact();
	other.pIF->colReact();
}

void ModelEntity::colReact()
{
	v.set0();
	spin(-1*rotv());
}

