#include "TransModelEntity.hpp"

#include <algorithm>

#include "TickServiceProvider.h"
#include "Frustum.h"
#include "IWorld.h"

#include "InteractFilterAlgoSym.h"

#include "collisionl2.hpp"

#include "gjk.hpp"

#include "FloatSeconds.hpp"

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
					tsp->getIWorld()->fromMeters(mo.extent().second)
				);
}

void TransModelEntity::draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 )
{
	float tickOffset = (float)FloatSeconds(ts-last_ticked);
	if(!viewFrustum->inside(bb,iw)) return;
	spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
	vec3 interPosMeters = iw.toMeters(interPos);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();

	//apply position
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	mo.drawHere();

	glPopMatrix();
}

void TransModelEntity::tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 )
{
	float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

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
	
	if(glm::length(iw->toMeters(pos)) > 500)
		v = -v;

	pos += v*tick_seconds;
	rot += drot* tick_seconds;
	scale += dscale * tick_seconds;
	mo.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
							*	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
							*	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
							*   glm::scale( scale )
	);

	iw->collideAlgo->doChecks(
		(Collider*) this, (Entity*) this,
		tick_seconds, *tsp);

	last_ticked = next_tick_begin;
}

void TransModelEntity::onSpawn( TickServiceProvider* tsp )
{
	mo.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
							*	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
							*	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
							*   glm::scale( scale )
	);
	bb = aabb( 0.0f, tsp );
}

void TransModelEntity::collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	gjk::RelColliders relcolliders(makeDualPointer((Entity*) this,(Collider*) this), other, tsp);
	float collision_time;
	//if(! gjk::firstRoot( relcolliders, 0.0f, delta_time, collision_time))
	if(! gjk::firstRoot(relcolliders, 0.0f, delta_time, collision_time, 5))
		return;	

	react(collision_time);
	other.pIF->react(collision_time);
}
	
vector<Vertex> TransModelEntity::vertices (float tick_time)
{
	auto r = tick_begin_rot + tick_time * tick_begin_drot;
	auto sc = tick_begin_scale + tick_time * tick_begin_dscale;
	mo.setTransform(		glm::rotate(glm::radians(r.x), vec3(1,0,0))
							*	glm::rotate(glm::radians(r.y), vec3(0,1,0))
							*	glm::rotate(glm::radians(r.z), vec3(0,0,1))
							*   glm::scale( sc )
	);
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

void TransModelEntity::react(float tick_time)
{
	pos = pos + (v*(tick_time));
	rot = rot + (drot*(tick_time));
	scale = scale + (dscale*(tick_time));
	mo.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
							*	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
							*	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
							*   glm::scale( scale )
	);
	v.set0();
	drot = vec3(0.0f);
}
