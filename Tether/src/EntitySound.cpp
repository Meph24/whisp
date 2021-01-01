/*
 * EntitySound.cpp
 *
 *  Created on:	05.08.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "EntitySound.h"

#include "WarnErrReporter.h"
#include "Frustum.h"
#include "TickServiceProvider.h"
#include "ChunkManager.h"
#include "FloatSeconds.hpp"

EntitySound::EntitySound(Entity* attachedTo,const sf::SoundBuffer& soundTemplate,float pitch,bool enable3D):
attached(attachedTo)
{
	if(attached)
	{
		attached->follow(this);
		pos=attached->pos;
		v=attached->v;
	}
	else
	{
		WarnErrReporter::missingAttachmentErr("EntitySound attachedTo==0");
	}
	sound.setBuffer(soundTemplate);
	sound.setRelativeToListener(!enable3D);//enable 3D Audio; yes this is correct!
	sound.setMinDistance(2);
	sound.setVolume(100);
	sound.setPitch(pitch);
	sound.setAttenuation(0.5f);
	playSoon=true;
//	std::cout<<"sound created"<<std::endl;
}

void EntitySound::notifyRemoval(Entity* e)
{
	if(attached!=e)
	{
		WarnErrReporter::wrongAttachmentErr("EntitySound attached!=e");
	}
	attached=0;
	pos=e->pos;
	v=e->v;
}

void EntitySound::draw(const SimClock::time_point& draw_time, Frustum* viewFrustum,IWorld& iw,DrawServiceProvider* dsp)
{
	float tickOffset=(float) FloatSeconds(draw_time-last_ticked);
	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=iw.toMeters(interPos);
	sound.setPosition(interPosMeters.x,interPosMeters.y,interPosMeters.z);
	if(playSoon)
	{
		sound.play();
//		std::cout<<"SOUND PLAY"<<std::endl;
		playSoon=false;
	}
}

void EntitySound::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	if(attached)
	{
		pos=attached->pos;
		v=attached->v;
	}
	last_ticked = next_tick_begin;
	if((!playSoon)&&sound.getStatus()==sf::Sound::Status::Stopped)
	{
//		std::cout<<"sound request destroy"<<std::endl;
		requestDestroy(&tsp->world());
	}
}

EntitySound::~EntitySound()
{
	if(attached) attached->unfollow(this);
//	if(sound.getStatus()==sf::Sound::Status::Playing)
//		std::cout<<"sound destroyed while playing"<<std::endl;
//	else if(playSoon)
//		std::cout<<"sound destroyed while waiting for draw"<<std::endl;
//	else
//		std::cout<<"sound destroyed normally"<<std::endl;
}

