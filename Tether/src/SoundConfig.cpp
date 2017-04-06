/*
 * SoundConfig.cpp
 *
 *  Created on:	Mar 23, 2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "SoundConfig.h"

SoundConfig::SoundConfig(float baseV, float volumeMaxO, float baseP, float pitchMaxO):
baseVolume(baseV),volumeMaxOffset(volumeMaxO),basePitch(baseP),pitchMaxOffset(pitchMaxO)
{

}

SoundConfig::~SoundConfig()
{

}

float SoundConfig::getVolume()
{
	return baseVolume;//TODO random
}
float SoundConfig::getPitch()
{
	return basePitch;//TODO random
}




