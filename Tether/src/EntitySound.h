/*
 * EntitySound.h
 *
 *  Created on:	05.08.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ENTITYSOUND_H_
#define SRC_ENTITYSOUND_H_
#include "Entity.h"
#include <SFML/Audio.hpp>
class EntitySound: public Entity
{
	Entity * attached;
	sf::Sound sound;
	bool playSoon;
public:
	virtual void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum,IWorld& iw, Perspective& perspective);

	virtual void tick(const SimClock::time_point& t,TickServiceProvider * tsp);

	virtual void notifyRemoval(Entity * e);
	EntitySound(Entity * attachedTo,const sf::SoundBuffer& soundTemplate,float pitch=1.0f,bool enable3D=true);
	~EntitySound();
};

#endif /* SRC_ENTITYSOUND_H_ */
