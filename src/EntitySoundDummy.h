/*
 * EntitySoundDummy.h
 *
 *  Created on:	15.03.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ENTITYSOUNDDUMMY_H_
#define SRC_ENTITYSOUNDDUMMY_H_

#include "Entity.h"


//TODO obsolete

class EntitySoundDummy:
		public Entity
{
public:
	int attachedSounds;
	EntitySoundDummy();
	virtual ~EntitySoundDummy();

	void tick(Entity * past,Entity * future,float sec);
};

#endif /* SRC_ENTITYSOUNDDUMMY_H_ */
