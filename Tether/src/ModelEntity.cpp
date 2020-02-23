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
	m_model.draw();

	glPopMatrix();
}

AABB ModelEntity::aabb(float tick_seconds, TickServiceProvider* tsp)
{
	Model::Extent ext = m_model.extent();	

	return AABB(pos , 
				pos + v*tick_seconds, 
				tsp->getIWorld()->fromMeters(ext.min),
				tsp->getIWorld()->fromMeters(ext.max)
				);
}

void ModelEntity::tick	(
							Timestamp t,
							TickServiceProvider* tsp
						)
{
	float tick_seconds = t - lastTick;
	lastTick = t;

	//the collider needs to save the old state of the entity
	//to use it for its calculations
	Collider::State old_state {pos, v};
	Collider::saveState(old_state);

	IWorld* iw = tsp->getIWorld();
	bb = aabb(tick_seconds, tsp);

	//entity attribute changes go here
	//this is code for collision simulation
	//entities turn around after flying away too far
	if(glm::length(iw->toMeters(pos)) > 100)
		v = v * -1.0f;

	//apply pos by velocity
	move(v*tick_seconds);

	//apply rotation by rotational velocity
	rotate(m_rotv * tick_seconds);

	iw->collideAlgo->doChecks(
			(Collider*) this, (Entity*) this,
			tick_seconds, *tsp);
}

// implementation of the Collider Interface

void collide(DualPointer<Collider> other, float delta_time, TickServiceProvider& tsp)
{
	vec3 o0_pos, o1_pos, o0_v, o1_v;
	//rel
	o0_pos = vec3(0.0f, 0.0f, 0.0f);
	o1_pos = tsp.getIWorld()->toMeters( other.pIF->getPosition(0.0f) - getPosition(0.0f));

	o0_v = tsp.getIWorld()->toMeters(getPosition(delta_time) - getPosition(0.0f)) / delta_time;
	o1_v = tsp.getIWorld()->toMeters(other.pIF->getPosition(delta_time) - other.pIF->getPosition(0.0f)) / delta_time;

	//rel
	o1_v = o1_v - o0_v;
	o0_v = vec3(0.0f, 0.0f, 0.0f);
 
	//rel
	vec3 o1_v = (o0)
	o1_v = tsp.getIWorld()->toMeters( );


	vector<collisionl2::SubmodelCollision> collisions = 
		collisionl2::linearInterpolation( delta_time, tsp.getIWorld(), (Collider*) this, other.pIF);
}


Collider::TYPE ModelEntity::colliderType() const { return Collider::TYPE::rigid; }

unsigned int ModelEntity::numVertices() const {return m_model.mesh().vertices.size();}

vector<Vertex> ModelEntity::vertices (float tick_time, const vector<unsigned int>* indices = nullptr) const
{
	if(!indices)
	{
		vertices = m_model.mesh().vertices;
	}
	else
	{
		vector<Vertex> vertices;
vertices.reserve(indices->size());
		for(unsigned int i : *indices)
		{
			vertices.push_back(m_model.mesh().vertices[i];
		}
	}
	return vertices;
}


vector<EdgeRef> ModelEntity::edges(float tick_time, const vector<unsigned int>* indices = nullptr) const
{
	if(!indices)
	{
		return m_model.edges();
	}
	else
	{
		std::set<unsigned int> inclusion (indices.begin(), indices.end());
		for(auto& e : m_model.edges())
		{
			vector<EdgeRef> edges;
			if(inclusion.find(e[0]) == inclusion.end() || inclusion.find(e[1] == inclusion.end()))
				continue;
			else
			{
				edges.push_back(e);
			}
		}

		return edges;
	}

}

vector<FaceRef> ModelEntity::faces(float tick_time, const vector<unsigned int>* indices = nullptr) const
{
	if(!indices)
	{
		return m_model.faces();
	}
	else
	{
		std::set<unsigned int> inclusion (indices.begin(), indices.end());
		for(auto& e : m_model.faces())
		{
			vector<FaceRef> faces;
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


void spacevec ModelEntity::getPosition(float tick_time) const
{
	return oldPos + tick_time * oldV
}


virtual void ModelEntity::collisionReaction(float tick_time, const vec3& pos)
{
	pos = getPosition(tick_time);
	v.set0();
}
