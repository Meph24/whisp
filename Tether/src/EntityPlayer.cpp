/*
 * EntityPlayer.cpp
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "EntityPlayer.h"

EntityPlayer::EntityPlayer(spacevec startPos,float screenH,float screenW)
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

void EntityPlayer::draw(float tickOffset, spacevec observerPos,ChunkManager* cm, DrawServiceProvider* dsp)
{
	if(isPerspective || cam->dist>minTPdist)
	{
		//TODO draw yourself
		isPerspective=false;
	}
}

spacevec EntityPlayer::applyPerspective(bool fresh,ChunkManager * cm)
{
	cam->posX=cm->toMeters(spacelen({0,pos.x.floatpart}));
	cam->posY=cm->toMeters(spacelen({0,pos.y.floatpart}))+characterHeight;
	cam->posZ=cm->toMeters(spacelen({0,pos.z.floatpart}));
	if(fresh) cam->applyFresh();
	else cam->apply();
	isPerspective=true;
	spacevec ret=pos;
	ret.x.floatpart=0;
	ret.y.floatpart=0;
	ret.z.floatpart=0;
	return ret;
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

void EntityPlayer::tick(float time, TickServiceProvider* tsp)
{

}
