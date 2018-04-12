/*
 * AnimationCycle.h
 *
 *  Created on:	Apr 12, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ANIMATIONCYCLE_H_
#define SRC_ANIMATIONCYCLE_H_

class AnimationCycle
{
	float hiddenProgress;
	void updateInternals(float timePassed);
	float cap(float x);
	int remaining;
public:
	float cycleLenInv;
	float progress;

	float getCurStep(float phaseOffset);//returns step between 0 and 1
	float getCurStepTau(float phaseOffset);//returns what you should pass into sin() function

	void update(float timePassed);//for use in tick
	void updateTemp(float timeOffset);//for use in draw

	AnimationCycle(float cycleLength,float startPhase,int maxCycles=-1);//-1 means no cycle limit
	~AnimationCycle();
};

#endif /* SRC_ANIMATIONCYCLE_H_ */
