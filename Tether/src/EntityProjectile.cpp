/*
 * EntityProjectile.cpp
 *
 *  Created on:	Jan 18, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EntityProjectile.h"
#include "TickServiceProvider.h"


ITexture * EntityProjectile::tex=new TextureDummy();


EntityProjectile::EntityProjectile(ItemAmmo * item,vec3 position,vec3 velocity):
//pos(position),v(velocity),
fromItem(item),posOld(position)
{
	pos=position;
	v=velocity;
}

EntityProjectile::~EntityProjectile()
{
	delete fromItem;
}

void EntityProjectile::draw()//TODO replace
{
	tex->bind();
	glColor3f(1.0f,1.0f, 0.1f);

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glBegin(GL_QUADS);


	glTexCoord2f(0, 0);
	glVertex3f(0, -0.1f, -0.1f);

	glTexCoord2f(1, 0);
	glVertex3f(0, 0.1f, -0.1f);

	glTexCoord2f(1, 1);
	glVertex3f(0,  0.1f,  0.1f);

	glTexCoord2f(0, 1);
	glVertex3f(0, -0.1f,  0.1f);


	glTexCoord2f(0, 0);
	glVertex3f(-0.1f, 0, -0.1f);

	glTexCoord2f(1, 0);
	glVertex3f(0.1f, 0, -0.1f);

	glTexCoord2f(1, 1);
	glVertex3f(0.1f, 0, 0.1f);

	glTexCoord2f(0, 1);
	glVertex3f(-0.1f, 0, 0.1f);


	glTexCoord2f(0, 0);
	glVertex3f(-0.1f, -0.1f,0);

	glTexCoord2f(1, 0);
	glVertex3f(0.1f, -0.1f,0);

	glTexCoord2f(1, 1);
	glVertex3f(0.1f, 0.1f,0);

	glTexCoord2f(0, 1);
	glVertex3f(-0.1f, 0.1f,0);


	glEnd();
	glPopMatrix();
}

void EntityProjectile::tick(float time, TickServiceProvider* tsp)
{
	ChunkManager * cm=tsp->getChunkManager();
	bool destroy = posOld.y < cm->getHeight(posOld.x,posOld.z);
	posOld = pos;
	vec3 vOld=v;
	v.y -= 9.81f * time;
	flt spdSq=v.lengthSq();
	flt drag=spdSq*fromItem->drag;
	vec3 dragDir=v;
	dragDir.normalize();
	v-=dragDir*drag*time;
	pos+=(v+vOld)*time*0.5f;
	if(destroy) tsp->requestDestroy(this);
}

void EntityProjectile::setTexture(ITexture* texture)
{
	if(tex) delete tex;
	tex=texture;
}
