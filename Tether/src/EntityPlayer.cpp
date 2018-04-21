/*
 * EntityPlayer.cpp
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityPlayer.h"

EntityPlayer::EntityPlayer(spacevec startPos,float screenH,float screenW,float characterSpeed):
speed(characterSpeed)
{
	pos=startPos;
	v={{0,0},{0,0},{0,0}};
	cam=new CameraTP();
	cam->alpha=0.0001f;
	cam->beta=0.0001f;
	cam->gamma=0.0001f;
	cam->height=screenH;
	cam->width=screenW;
	cam->maxView=1024*8;
	setTP(false);
	cam->zoom=1;//TODO provide interface
}

EntityPlayer::~EntityPlayer()
{
	delete cam;
}

void EntityPlayer::draw(float tickOffset,Frustum * viewFrustum,ChunkManager* cm, DrawServiceProvider* dsp)
{
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

Frustum * EntityPlayer::getViewFrustum(ChunkManager * cm)
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
	ret->planes[4]=cam->getFarPlane();
	for(int i=0;i<5;i++)//TODO do not hard-code
	{
		ret->planes[i]=cam->getNearPlane();//TODO remove after debugging
		ret->planes[i].distanceInChunks/=cm->getChunkSize();
	}
	return ret;
}

void EntityPlayer::tick(float time, TickServiceProvider* tsp)
{
	spacevec size;
	size.x=characterHeightConv*0.5f;
	size.y=characterHeightConv*1.5f;
	size.z=size.x;
	bb=AABB(pos,size,v*(-time));
}
