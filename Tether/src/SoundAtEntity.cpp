/*
 * SoundAtEntity.cpp
 *
 *  Created on:	17.03.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "SoundAtEntity.h"

SoundAtEntity::SoundAtEntity(entityID e,sf::SoundBuffer * buf):
sound(*buf),
eID(e),
status(0)
{

}

SoundAtEntity::~SoundAtEntity()
{
	// TODO Auto-generated destructor stub
}

void SoundAtEntity::update(float myVolume,float myPitch,mat4 listener)
{
	sound.setPitch(myPitch*pitch);
	vec3 where=listener*lastPos;

	float len=where.normGetLen();
	if(len<0.1f)
	{
		sound.setPosition(0,0,0);
	}
	else
	{
		if(len<0.5f) len=0.5f;
		sound.setPosition(where.x,where.y,where.z);
	}

	//http://sound.stackexchange.com/questions/25529/what-is-0-db-in-digital-audio
	//http://www.sengpielaudio.com/calculator-distance.htm
	sound.setVolume(myVolume*volume/len);
}

bool SoundAtEntity::update(EntityContainer * container,float secSinceTick,float secSinceUpdate,float myVolume,float myPitch,mat4 listener)
{
	if(status&2)
	{
		lastPos+=lastSpeed*secSinceUpdate;
		update(myVolume,myPitch,listener);
	}
	else
	{
		entityPtr ep=container->getEntity(eID);
		if(ep.now)
		{
			lastPos=ep.now->pos+ep.now->speed*secSinceTick;
			lastSpeed=ep.now->speed;
			update(myVolume,myPitch,listener);
		}
		else
		{
			status+=2;
			lastPos+=lastSpeed*secSinceUpdate;
			update(myVolume,myPitch,listener);
		}
	}
	if(!(status&1))
	{
		status++;
		sound.play();
		return false;
	}
	return sound.getStatus()==sf::SoundSource::Status::Stopped;
}

