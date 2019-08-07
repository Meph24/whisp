/*
 * SpeedMod.h
 *
 *  Created on:	26.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_SPEEDMOD_H_
#define SRC_SPEEDMOD_H_
#include "MatrixLib.h"
class SpeedMod
{
public:
	flt slowdownFromTerrain(flt h);
	SpeedMod();
	~SpeedMod();
};

#endif /* SRC_SPEEDMOD_H_ */
