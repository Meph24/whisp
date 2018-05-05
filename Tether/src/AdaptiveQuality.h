/*
 * AdaptiveQuality.h
 *
 *  Created on:	04.05.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ADAPTIVEQUALITY_H_
#define SRC_ADAPTIVEQUALITY_H_

class AdaptiveQuality
{
public:
	float target;
	float max;
	float min;

	float lastQuality;

	float getQuality(float curTime);

	AdaptiveQuality(float minQuality,float maxQuality,float targetTime);
	~AdaptiveQuality();
};

#endif /* SRC_ADAPTIVEQUALITY_H_ */
