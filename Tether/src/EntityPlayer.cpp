/*
 * EntityPlayer.cpp
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityPlayer.h"

EntityPlayer::EntityPlayer(Timestamp spawnTime,spacevec startPos,sf::Window * w,float sensX,float sensY,float characterSpeed):
speed(characterSpeed)
{
	lastTick=spawnTime;
	pos=startPos;
	v={{0,0},{0,0},{0,0}};
	cam=new CameraTP();
	cam->alpha=0.0001f;
	cam->beta=0.0001f;
	cam->gamma=0.0001f;
	cam->height=w->getSize().y;
	cam->width=w->getSize().x;
	cam->maxView=1024*8;
	setTP(false);
	cam->zoom=1;//TODO provide interface
	mouseInp = new Zombie_MouseInput(this, w);
	mouseInp->sensitivityX=sensX;
	mouseInp->sensitivityY=sensY;
	keyInp = new Zombie_KeyInput(mouseInp,cam);
	mouseInp->enable();
	HP=maxHP;

	pushRadius=0.4f;
	pushForce=speed/30;
}

EntityPlayer::~EntityPlayer()
{
	delete cam;
	delete keyInp;
	delete mouseInp;
}

void EntityPlayer::draw(Timestamp t,Frustum * viewFrustum,ChunkManager* cm, DrawServiceProvider* dsp)
{
	//float tickOffset=t-lastTick;
	if(isPerspective || cam->dist>minTPdist)
	{
		//TODO draw yourself
		isPerspective=false;
	}
}

void EntityPlayer::applyPerspective(bool fresh,ChunkManager * cm)
{
	spacevec relPos=pos-cm->getMiddleChunk();
	characterHeightConv=cm->fromMeters(characterHeight);
	relPos.y+=characterHeightConv;
	cam->posX=cm->toMeters(relPos.x);
	cam->posY=cm->toMeters(relPos.y);
	cam->posZ=cm->toMeters(relPos.z);
	if(fresh) cam->applyFresh();
	else cam->apply();
	isPerspective=true;
}

void EntityPlayer::setTP(bool on)
{
	if(on)
	{
		if(cam->dist<minTPdist)
		{
			cam->dist=minTPdist;
		}
		cam->minView=0.5f;
	}
	else
	{
		cam->dist=0;
		cam->minView=0.125f;
	}
}

void EntityPlayer::changeTPdist(float amount)
{
	cam->dist+=amount;
	setTP(cam->dist>=minTPdist);
	if(cam->dist>maxTPdist) cam->dist=maxTPdist;
}

spacevec EntityPlayer::getCamPos()
{
	spacevec ret=pos;
	ret.y+=characterHeightConv;
	return ret;
}

Frustum * EntityPlayer::newGetViewFrustum(ChunkManager * cm,float viewDistRestriction)
{
	Frustum * ret=new Frustum();
	ret->observerPos=cm->getMiddleChunk();
	bool lookingUp=cam->alpha<0;
	if(lookingUp)
	{
		ret->planes[0]=cam->getUpperPlane();
		ret->planes[3]=cam->getLowerPlane();
	}
	else
	{
		ret->planes[0]=cam->getLowerPlane();
		ret->planes[3]=cam->getUpperPlane();
	}
	ret->planes[1]=cam->getLeftPlane();
	ret->planes[2]=cam->getRightPlane();
	ret->planes[4]=cam->getFarPlane(viewDistRestriction);//TODO evaluate usefulness
	//   planes[5] would be the near plane, gain from it would be 0 most of the time, so not included here
	for(int i=0;i<5;i++)//TODO do not hard-code
	{
		ret->planes[i].distanceInChunks/=cm->getChunkSize();
	}
	return ret;
}

void EntityPlayer::tick(Timestamp t, TickServiceProvider* tsp)
{
	float time=t-lastTick;
	lastTick=t;
	ChunkManager * cm=tsp->getChunkManager();
	HP += maxHP*time / 200;
	if (HP > maxHP) HP = maxHP;

	spacevec oldPos=pos;
	vec3 wantedV=keyInp->getVelocity()*speed;
	pos+=cm->fromMeters(wantedV)*time;
	pos=cm->clip(pos,true);
	spacevec newPos=pos;
	vec3 moved=cm->toMeters(newPos-oldPos);
	if(moved.lengthSq()>0.0000000001f)
	{
		vec3 norm=moved;
		norm.normalize();
		flt speedModA=(vec3(norm.x,0,norm.z).length());
		vec3 flat=vec3(moved.x,0,moved.z);
		flt h=moved.y/flat.length();
		SpeedMod sm=SpeedMod();
		flt speedModB=sm.slowdownFromTerrain(h);
		pos=cm->clip(oldPos+cm->fromMeters(flat*speedModA*speedModB),true);
	}
	if(time>0.0000000001f)
		v=(pos-oldPos)/time;

	spacevec size;
	size.x=characterHeightConv*0.5f;
	size.y=characterHeightConv*1.5f;
	size.z=size.x;
	bb=AABB(pos,size,v*(-time));
	if(multichunk) alreadyChecked.clear();
	multichunk=bb.isMultichunk();
}

void EntityPlayer::push(spacevec amount)
{
	pos+=amount;//TODO?
	HP -= 15625*(amount.fLengthSq(16));//TODO not hard code chunk size
}

spacevec EntityPlayer::getPos()
{
	return pos;
}
