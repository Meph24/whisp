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

using std::cerr;

using glm::vec4;
using glm::vec3;
using glm::mat4;

const Model& ModelEntity::model()
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

void ModelEntity::draw(	
					Timestamp ts, 
					Frustum* viewFrustum,
					IWorld& iw,
					DrawServiceProvider* dsp
				 )
{
	float tickOffset = ts-lastTick;
	if(!viewFrustum->inside(bb,iw)) return;
	std::cout<<"tickOffset: "<<tickOffset<<std::endl;
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
				tsp->getIWorld()->fromMeters(ext.first),
				tsp->getIWorld()->fromMeters(ext.second)
				);
}

void ModelEntity::tick	(
							Timestamp t,
							TickServiceProvider* tsp
						)
{
	float tick_seconds = t - lastTick;
	lastTick = t;

	tick_begin_pos = pos;
	tick_begin_v = v;

	//the collider needs to save the old state of the entity
	//to use it for its calculations

	IWorld* iw = tsp->getIWorld();
	bb = aabb(tick_seconds, tsp);

	//entity attribute changes go here
	//this is code for collision simulation
	//entities turn around after flying away too far
	if(glm::length(iw->toMeters(pos)) > 100)
		v = v * -1.0f;

	//apply pos by velocity
	pos += v*tick_seconds;

	iw->collideAlgo->doChecks(
			(Collider*) this, (Entity*) this,
			tick_seconds, *tsp);
}

// implementation of the Collider Interface

void ModelEntity::collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	vector<collisionl2::SubmodelCollision> collisions = 
		collisionl2::linearInterpolation_R0(0.0f, delta_time, tsp.getIWorld(), *this, *(other.pIF));

	if(collisions.empty()) return;

	auto min_e = std::min_element(collisions.begin(), collisions.end());
	
	if (min_e->time > delta_time)
		cerr << "ERROR : Collision to later time than tick!\n";

	react(min_e->time);
	other.pIF->react(min_e->time);
}


Collider::TYPE ModelEntity::type() const { return Collider::TYPE::rigid; }

vector<Vertex> ModelEntity::vertices (float tick_time, const vector<unsigned int>* indices) const
{
	if(!indices)
	{
		return m_model.vertices();
	}
	else
	{
		vector<Vertex> out_vertices;
		out_vertices.reserve(indices->size());
		for(unsigned int i : *indices)
		{
			out_vertices.push_back(m_model.vertices()[i]);
		}
		return out_vertices;
	}
}


vector<EdgeRef> ModelEntity::edges(float tick_time, const vector<unsigned int>* indices) const
{
	if(!indices)
	{
		return m_model.edges();
	}
	else
	{
		std::set<unsigned int> inclusion (indices->begin(), indices->end());
		vector<EdgeRef> edges;
		for(auto& e : m_model.edges())
		{
			if(inclusion.find(e[0]) == inclusion.end() || inclusion.find(e[1]) == inclusion.end())
				continue;
			else
			{
				edges.push_back(e);
			}
		}
		return edges;
	}

}

vector<FaceRef> ModelEntity::faces(float tick_time, const vector<unsigned int>* indices) const
{
	if(!indices)
	{
		return m_model.faces();
	}
	else
	{
		std::set<unsigned int> inclusion (indices->begin(), indices->end());
		vector<FaceRef> faces;
		for(auto& e : m_model.faces())
		{
			if(	inclusion.find(e[0]) == inclusion.end() || 
				inclusion.find(e[1]) == inclusion.end() || 
				inclusion.find(e[2]) == inclusion.end()	)
				continue;
			else
			{
				faces.push_back(e);
			}
		}

		return faces;
	}
}


spacevec ModelEntity::position(float tick_time) const
{
	return tick_begin_pos + tick_begin_v * tick_time;
}


void ModelEntity::react(float tick_time)
{
	pos = position(tick_time*0.9999);
	v.set0();
}
