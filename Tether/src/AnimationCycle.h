/*
 * AnimationCycle.h
 *
 *  Created on:	Apr 12, 2018
 *      Author:	HL65536
 *     Version:	2.1
 */

#ifndef SRC_ANIMATIONCYCLE_H_
#define SRC_ANIMATIONCYCLE_H_

#include "NestedSyncable.h"

class AnimationCycle: public NestedSyncable
{
	float tempProgress;//client side
	float modFloat(float x,float by);
	float cap01(float x);
	float cap0max(float x);
	u8 maxCycles;//precision problems with higher numbers
	float cycleLenInv;
	float confirmedProgress;
public:
	float getCurStep(float phaseOffset,bool withTempProgress);//returns step between 0 and 1
	float getCurStepTau(float phaseOffset,bool withTempProgress);//returns what you should pass into sin() function

	void update(float timePassed);//for use in tick
	void updateTemp(float timeOffset);//for use in draw

	AnimationCycle(float cycleLength,float startPhase,u8 maxCycles=0);//0 means no cycle limit
	~AnimationCycle();

	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	AnimationCycle(sf::Packet& p, SyncableManager& sm);//deserialize constructor
};

#endif /* SRC_ANIMATIONCYCLE_H_ */
