/*
 * SoundManager.cpp
 *
 *  Created on:	05.08.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "SoundManager.h"

SoundManager::SoundManager()
{
	headshotSound=sf::SoundBuffer();
	headshotSound.loadFromFile("res/headshot.wav");
	crippleSound=sf::SoundBuffer();
	crippleSound.loadFromFile("res/cripple.wav");
}

const sf::SoundBuffer& SoundManager::getHeadshotSound()
{
	return headshotSound;
}

SoundManager::~SoundManager()
{
	// TODO Auto-generated destructor stub
}

const sf::SoundBuffer& SoundManager::getCrippleSound()
{
	return crippleSound;
}
