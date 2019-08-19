/*
 * SoundManager.h
 *
 *  Created on:	05.08.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_SOUNDMANAGER_H_
#define SRC_SOUNDMANAGER_H_

#include <SFML/Audio.hpp>

class SoundManager
{
	sf::SoundBuffer headshotSound;
	sf::SoundBuffer crippleSound;
public:
	const sf::SoundBuffer& getHeadshotSound();
	const sf::SoundBuffer& getCrippleSound();
	SoundManager();
	~SoundManager();
};

#endif /* SRC_SOUNDMANAGER_H_ */
