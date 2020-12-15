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
#include "FloatSeconds.hpp"

#include "glmutils.hpp"

ITexture * EntityProjectileBulletLike::tex=new TextureDummy();


EntityProjectileBulletLike::EntityProjectileBulletLike(BulletLikeSource * origin,BulletLikeType t,const SimClock::time_point& spawn_time, spacevec position,spacevec velocity):
typeB(t),source(origin)
//,fromItem(item)
{
	typeH=FLAG_HIT_TYPE_BULLET_LIKE;
	posOld=position;
	last_ticked = spawn_time;
	pos=position;
	v=velocity;
}

EntityProjectileBulletLike::~EntityProjectileBulletLike()
{
//	delete fromItem;
}

void EntityProjectileBulletLike::draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw,DrawServiceProvider * dsp)
{
	float tickOffset=(float)FloatSeconds(draw_time - last_ticked);
	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;//TODO frustum culling?
	vec3 interPosMeters=iw.toMeters(interPos);
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
void EntityProjectileBulletLike::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();

	float time = (float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;

	ITerrain * it=tsp->getITerrain();

	spacevec gravity=it->getGravity(pos);
	if(it->hitsGround(posOld,pos))
	{
		requestDestroy(iw);
		return;
	}

	posOld = pos;
	spacevec vOld=v;
	v += gravity * time;
	double spd=iw->toMetersD(v.lengthHP());
	double drag=spd*typeB.drag;
	v*=(1-drag*time);//TODO find exact or at least time-consistent solution (current solution behaves very wrong with high resistance values and is tickrate-dependent)
	pos+=(v+vOld)*time*0.5f;

	spacevec size;
	size.set0();
	bb=AABB(pos,size,pos-posOld);
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
	ze->checkProjectile(this,*tsp);//TODO
	//some calculations
	//float vSqBefore=glm::sqlen(tsp->getIWorld()->toMeters(v));
	//float vSqAfter=0;//TODO
	//float deltaE=0.5*typeB.mass*(vSqBefore*vSqBefore-vSqAfter*vSqAfter);
	float dmg=typeB.dmgPerJd0;//deltaE*(typeB.dmgPerJd0*(1-deformation)+typeB.dmgPerJd1*deformation);

//	std::cout<<"hello projectile; dmg: "<<dmg<<std::endl;
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
