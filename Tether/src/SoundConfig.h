/*
 * SoundConfig.h
 *
 *  Created on:	Mar 23, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_SOUNDCONFIG_H_
#define SRC_SOUNDCONFIG_H_

class SoundConfig
{
	float baseVolume;
	float volumeMaxOffset;
	float basePitch;
	float pitchMaxOffset;

public:
	SoundConfig(float baseVolume, float volumeMaxOffset, float basePitch, float pitchMaxOffset);
	~SoundConfig();

	//generates a volume value randomly according to specified values
	float getVolume();
	//generates a pitch value randomly according to specified values
	float getPitch();
};

#endif /* SRC_SOUNDCONFIG_H_ */
