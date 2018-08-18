/*
 * EntitySound.cpp
 *
 *  Created on:	05.08.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EntitySound.h"


EntitySound::EntitySound(Entity* attachedTo,const sf::SoundBuffer& soundTemplate,float pitch,bool enable3D):
attached(attachedTo)
{
	if(attached)
	{
		attached->follow(this);
		pos=attached->pos;
		v=attached->v;
	}
	//TODO else error
	sound.setBuffer(soundTemplate);
	sound.setRelativeToListener(!enable3D);//enable 3D Audio
	sound.setMinDistance(2);
	sound.setVolume(100);
	sound.setPitch(pitch);
	sound.setAttenuation(0.5f);
	playSoon=true;
	std::cout<<"sound created"<<std::endl;
}

#include "WarnErrReporter.h"
void EntitySound::notifyRemoval(Entity* e)
{
	if(attached!=e)
	{
		//TODO err
	}
	attached=0;
	pos=e->pos;
	v=e->v;
}
#include "WarnErrReporter.h"
void EntitySound::draw(Timestamp t, Frustum* viewFrustum, ChunkManager* cm,DrawServiceProvider* dsp)
{
	float tickOffset=t-lastTick;
	if(!exists) return;//TODO this kind of check should be done by the caller beforehand
	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=cm->toMeters(interPos);
	sound.setPosition(interPosMeters.x,interPosMeters.y,interPosMeters.z);
	if(playSoon)
	{
		sound.play();
		std::cout<<"SOUND PLAY"<<std::endl;
		playSoon=false;
	}
//	else std::cout<<"sound alive @ "<<interPosMeters<<" observer:"<<viewFrustum->observerPos<<std::endl;
}

void EntitySound::tick(Timestamp t, TickServiceProvider* tsp)
{
	if(attached)
	{
		pos=attached->pos;
		v=attached->v;
	}
	lastTick=t;
	if((!playSoon)&&sound.getStatus()==sf::Sound::Status::Stopped)
	{
		std::cout<<"sound request destroy"<<std::endl;
		requestDestroy(tsp->getChunkManager());
	}
}

EntitySound::~EntitySound()
{
	if(attached) attached->unfollow(this);
	if(sound.getStatus()==sf::Sound::Status::Playing)
		std::cout<<"sound destroyed while playing"<<std::endl;
	else if(playSoon)
		std::cout<<"sound destroyed while waiting for draw"<<std::endl;
	else
		std::cout<<"sound destroyed normally"<<std::endl;
}

