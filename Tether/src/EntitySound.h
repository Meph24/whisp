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


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	EntitySound(sf::Packet p,TickServiceProvider * tsp, SyncableManager& sm);//deserialize constructor
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* SRC_ENTITYSOUND_H_ */
