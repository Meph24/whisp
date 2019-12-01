/*
 * EntityProjectile.cpp
 *
 *  Created on:	Jan 18, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityProjectileBulletLike.h"

#include "TickServiceProvider.h"
#include "Zombie_Enemy.h"
#include "TextureDummy.h"
#include "BulletLikeSource.h"
#include "Frustum.h"
#include "IWorld.h"
#include "ChunkManager.h"
#include "InteractFilterAlgoAsym.h"

#include "glmutils.hpp"

ITexture * EntityProjectileBulletLike::tex=new TextureDummy();


EntityProjectileBulletLike::EntityProjectileBulletLike(BulletLikeSource * origin,BulletLikeType t,Timestamp spawnTime,spacevec position,spacevec velocity):
typeB(t),source(origin)
//,fromItem(item)
{
	typeH=FLAG_HIT_TYPE_BULLET_LIKE;
	posOld=position;
	lastTick=spawnTime;
	pos=position;
	v=velocity;
}

EntityProjectileBulletLike::~EntityProjectileBulletLike()
{
//	delete fromItem;
}

void EntityProjectileBulletLike::draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)
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
void EntityProjectileBulletLike::tick(Timestamp t, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();

	float time=t-lastTick;
	lastTick=t;

	ChunkManager * cm=tsp->getChunkManager();
	spacevec gravity=cm->getGravity(pos);
	if(cm->hitsGround(posOld,pos))
	{
		requestDestroy(cm);
		return;
	}

	posOld = pos;
	spacevec vOld=v;
	v += gravity * time;
	double spd=cm->toMetersD(v.lengthHP());
	double drag=spd*typeB.drag;
	v*=(1-drag*time);//TODO find exact or at least time-consistent solution (current solution behaves very wrong with high resistance values and is tickrate-dependent)
	pos+=(v+vOld)*time*0.5f;

	spacevec size;
	size.set0();
	bb=AABB(posOld,size,pos-posOld);
	iw->projectileAlgo->doChecks((Projectile *)this, (Entity *)this,time,*tsp);

}
#include <iostream>
#include "WarnErrReporter.h"
bool EntityProjectileBulletLike::collide(HittableBulletLike* hittable,ProjectileCollision collision,TickServiceProvider* tsp)
{

	Zombie_Enemy * ze=dynamic_cast<Zombie_Enemy *>(collision.hitVictim.e);
	if(ze==0)
	{
		WarnErrReporter::wrongTypeErr("hard-coded cast to zombie failed");
		return true;
	}
	ze->checkProjectile(this,tsp);//TODO
	//some calculations
	float vSqBefore=glm::sqlen(tsp->getChunkManager()->toMeters(v));
	float vSqAfter=0;//TODO
	float deltaE=0.5*typeB.mass*(vSqBefore*vSqBefore-vSqAfter*vSqAfter);
	float dmg=deltaE*(typeB.dmgPerJd0*(1-deformation)+typeB.dmgPerJd1*deformation);

	std::cout<<"hello projectile; dmg: "<<dmg<<std::endl;
	if(!source) std::cout<<"source 0"<<std::endl;
	else if(!hittable) std::cout<<"hittable 0"<<std::endl;
	else if(dmg>0) source->hitCallback(dmg,false,true,hittable);
	return false;//TODO projectile currently stops immediately


	//TODO remove above temp code and implement this (old proposal):
	//params=BulletHittable::getBulletTargetParams(...)
	//cont,damageProperties=BulletProjectile::applyHit(params);
	//BulletHittable::applyDamage(damageProperties);

}

EntityProjectileBulletLike* EntityProjectileBulletLike::asProjectileBulletLike()
{
	return this;
}

void EntityProjectileBulletLike::setTexture(ITexture* texture)
{
	if(tex) delete tex;
	tex=texture;
}
