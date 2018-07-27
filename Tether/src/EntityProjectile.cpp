/*
 * EntityProjectile.cpp
 *
 *  Created on:	Jan 18, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityProjectile.h"
#include "TickServiceProvider.h"


ITexture * EntityProjectile::tex=new TextureDummy();


EntityProjectile::EntityProjectile(Timestamp spawnTime,ItemAmmo * item,spacevec position,spacevec velocity):
fromItem(item)
{
	type=FLAG_HIT_TYPE_BULLET;
	posOld=position;
	lastTick=spawnTime;
	pos=position;
	v=velocity;
}

EntityProjectile::~EntityProjectile()
{
	delete fromItem;
}

void EntityProjectile::draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)
{
	float tickOffset=t-lastTick;
	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;//TODO frustum culling?
	vec3 interPosMeters=cm->toMeters(interPos);
	tex->bind();
	glColor3f(1.0f,1.0f, 0.1f);
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
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
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
void EntityProjectile::tick(Timestamp t, TickServiceProvider* tsp)
{
	float time=t-lastTick;
	lastTick=t;

	ChunkManager * cm=tsp->getChunkManager();
	spacelen gravity=cm->getGravity();
	if(cm->hitsGround(posOld,pos))
	{
		requestDestroy(cm);
		return;
	}

	posOld = pos;
	spacevec vOld=v;
	v.y -= gravity * time;
	double spd=v.dLength(cm->getChunkSize());
	double drag=spd*fromItem->drag;
	v*=(1-drag*time);//TODO find exact or at least time-consistent solution (current solution behaves very wrong with high resistance values and is tickrate-dependent)
	pos+=(v+vOld)*time*0.5f;

	spacevec size;
	size.x.floatpart=0;
	size.y.floatpart=0;
	size.z.floatpart=0;
	size.x.intpart=0;
	size.y.intpart=0;
	size.z.intpart=0;
	bb=AABB(posOld,size,pos-posOld);
	registerHitCheck((Entity *)this,time,tsp);

}

void EntityProjectile::setTexture(ITexture* texture)
{
	if(tex) delete tex;
	tex=texture;
}
