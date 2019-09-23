/*
 * SpeedMod.h
 *
 *  Created on:	26.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_SPEEDMOD_H_
#define SRC_SPEEDMOD_H_

#include "MathStuff.h"

class SpeedMod
{
public:
	float slowdownFromTerrain(float h);
	SpeedMod();
	~SpeedMod();
};

#endif /* SRC_SPEEDMOD_H_ */
