/*
 * SoundAtEntity.h
 *
 *  Created on:	17.03.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_SOUNDATENTITY_H_
#define SRC_SOUNDATENTITY_H_

#include <SFML/Audio.hpp>
#include "EntityContainer.h"
#include "MatrixLib.h"

class SoundAtEntity
{
	sf::Sound sound;//TODO for tier 3: no sound objects for sounds too far away
	entityID eID;
	vec3 lastPos;//last calculated position
	vec3 lastSpeed;//last fetched speed
	int status;//0 is fresh, lsb = started, next bit = use local data

	float volume,pitch;

	void update(float myVolume,float myPitch,mat4 listener);
public:
	SoundAtEntity(entityID e,sf::SoundBuffer * buf);
	~SoundAtEntity();

	//return if wants to be destroyed
	bool update(EntityContainer * container,float secSinceTick,float secSinceUpdate,float myVolume,float myPitch,mat4 listener);


};

#endif /* SRC_SOUNDATENTITY_H_ */
