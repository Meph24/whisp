#include "TransModelEntity.hpp"

#include <algorithm>

#include "TickServiceProvider.h"
#include "Frustum.h"
#include "IWorld.h"

#include "InteractFilterAlgoSym.h"

#include "collisionl2.hpp"

vector<Model::ConvexPart> TransModelEntity::convexParts() const
{
	return mo.model().convexParts();
}

TransModelEntity::TransModelEntity(const Model& model)
	: mo(model),
	rot(0.0f), scale(1.0f), drot(0.0f), dscale(0.0f)
{
	pos.set0();
	v.set0();
}

const ModelObject& TransModelEntity::modelObject() const
{
	return mo;
}

AABB TransModelEntity::aabb(float tick_seconds, TickServiceProvider* tsp)
{
	return AABB(	pos,
					pos + v * tick_seconds,
					tsp->getIWorld()->fromMeters(mo.extent().first),
					tsp->getIWorld()->fromMeters(mo.extent().second));
}

void TransModelEntity::draw(	Timestamp ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 )
{
	float tickOffset = ts-lastTick;
	if(!viewFrustum->inside(bb,iw)) return;
	spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
	vec3 interPosMeters = iw.toMeters(interPos);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();

	//apply position
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	//rotation already done by matrixmult in model.draw()
	//glRotatef(m_rot.x, 1.0f, 0.0f, 0.0f);
	//glRotatef(m_rot.y, 0.0f, 1.0f, 0.0f);
	//glRotatef(m_rot.z, 0.0f, 0.0f, 1.0f);
	mo.drawHere();

	glPopMatrix();
}

void TransModelEntity::tick(	Timestamp t,
						TickServiceProvider* tsp
					 )
{
	float tick_seconds = t - lastTick;

	tick_begin_pos = pos;
	tick_begin_v = v;
	tick_begin_rot = rot;
	tick_begin_drot = drot;
	tick_begin_scale = scale;
	tick_begin_dscale = dscale;

	IWorld* iw = tsp->getIWorld();
	
	bb = aabb(tick_seconds, tsp);

	//simulation testing code
	//TODO remove
	
	if(glm::length(iw->toMeters(pos)) > 100)
		v = -v;

	pos += v*tick_seconds;
	rot += drot* tick_seconds;
	scale += dscale * tick_seconds;
	mo.setTransform(		glm::rotate(rot.x, vec3(1,0,0))
							*	glm::rotate(rot.y, vec3(0,1,0))
							*	glm::rotate(rot.z, vec3(0,0,1))
							*	glm::scale(scale));

	iw->collideAlgo->doChecks(
		(Collider*) this, (Entity*) this,
		tick_seconds, *tsp);
}

void TransModelEntity::collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{

	//TODO the content of this function is not the intended behavior
	//it is the behavior of ModelEntities
	//change to appropriate behavior
	//also TODO ist : factor in the Collider::TYPE in choosing a collision algorithm
	vector<collisionl2::SubmodelCollision> collisions = 
		collisionl2::linearInterpolation_R0(0.0f, delta_time, tsp.getIWorld(), *this, *(other.pIF));

	if(collisions.empty()) return;

	auto min_e = std::min_element(collisions.begin(), collisions.end());
	
	react(min_e->time);
	other.pIF->react(min_e->time);
}

Collider::TYPE TransModelEntity::type() const
{
	return Collider::TYPE::transformable;
}
	
vector<Vertex> TransModelEntity::vertices (float tick_time) const
{
	return mo.vertices();
}

vector<EdgeRef> TransModelEntity::edges(float tick_time) const
{
	return mo.model().edges();
}

vector<FaceRef> TransModelEntity::faces(float tick_time) const
{
	return mo.model().faces();
}

spacevec TransModelEntity::position(float tick_time) const
{
	return tick_begin_pos + tick_begin_v * tick_time;
}

void TransModelEntity::react(float tick_time)
{
	pos = position(tick_time*0.9999);
	v.set0();
	drot = vec3(0.0f);
	dscale = vec3(0.0f);
}

