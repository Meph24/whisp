/*
 * ZombiePlayer.h
 *
 *  Created on:	20.02.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIEPLAYER_H_
#define SRC_ZOMBIEPLAYER_H_

#include "ICamera3D.h"

class Zombie_Player {
public:

	int currentGun;
	int score=0;
	float playerHP;
	float hitmark = 0;
	ICamera3D * cam;
	Zombie_Player();
	~Zombie_Player();
};

#endif /* SRC_ZOMBIEPLAYER_H_ */
