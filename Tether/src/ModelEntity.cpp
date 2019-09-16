#include "ModelEntity.hpp"

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.hpp"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
#include "Frustum.h"

using glm::vec4;
using glm::vec3;
using glm::mat4;

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

vector<vec4> ModelEntity::shape(const vec3& pos) const
{
	vector<vec4> v;

	for(auto e : model.mesh().vertices)
	{
		glm::vec4 vertex (e.x, e.y, e.z, 1.0f);
		v.push_back(vertex * (mat4) cummat);
	}

	return v;
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
	//if(!viewFrustum->inside(bb)) return;
	
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

