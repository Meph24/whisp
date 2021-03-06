#include "ModelEntity.hpp"

#include <cmath>
#include <algorithm>
#include <set>

#include "glmutils.hpp"
#include <glm/gtx/transform.hpp>

#include "Mesh.hpp"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "Frustum.h"

#include "InteractFilterAlgoSym.h"

#include <iostream>

#include "FloatSeconds.hpp"

using std::cerr;

using glm::vec4;
using glm::vec3;
using glm::mat4;

vector<Model::ConvexPart> ModelEntity::convexParts() const
{
	return m_model.convexParts();
}

const Model& ModelEntity::model()
{
	return m_model;
}

ModelEntity::ModelEntity(const Model& model)
:
	m_model(model)
{
	surviveClearing = false;
	v.set0();
}

ModelEntity::~ModelEntity()
{}

void ModelEntity::draw(	
					const SimClock::time_point& draw_time,
					Frustum* viewFrustum,
					IWorld& iw,
					Perspective& perspective
				 )
{
	float tickOffset = (float) FloatSeconds(draw_time-last_ticked);
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
	m_model.drawHere();

	glPopMatrix();
}

AABB ModelEntity::aabb(float tick_seconds, TickServiceProvider* tsp)
{
	const pair<vec3, vec3>& ext = m_model.extent();	

	return AABB(pos , 
				pos + v*tick_seconds, 
				tsp->world().fromMeters(ext.first),
				tsp->world().fromMeters(ext.second)
				);
}

void ModelEntity::tick	(
							const SimClock::time_point& next_tick_begin,
							TickServiceProvider* tsp
						)
{
	float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;

	//the collider needs to save the old state of the entity
	//to use it for its calculations

	bb = aabb(tick_seconds, tsp);
}

vector<Vertex> ModelEntity::vertices (float tick_time) const
{
	return m_model.vertices();
}

vector<EdgeRef> ModelEntity::edges(float tick_time) const
{
	return m_model.edges();
}

vector<FaceRef> ModelEntity::faces(float tick_time) const
{
	return m_model.faces();
}

void ModelEntity::serialize(sf::Packet& p, bool complete)
{
	//TODO
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

void ModelEntity::deserialize(sf::Packet& p, SyncableManager& sm)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

void ModelEntity::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

void ModelEntity::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
	bool thisIsImplemented=false;
	assert(thisIsImplemented);
}

u32 ModelEntity::getClassID()
{
	return CLASS_ID_ModelEntity;
}
