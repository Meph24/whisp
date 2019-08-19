/*
 * RecoilReturning.h
 *
 *  Created on:	05.01.2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_RECOILRETURNING_H_
#define SRC_RECOILRETURNING_H_

#include "RecoilManager.h"

class RecoilReturning : public RecoilManager
{
	vec3 retP;
protected:
	virtual vec3 getRecoil(float time);
public:
	RecoilReturning(vec3 returnPart,float fullyReturnTime,float fastestShotTime);
	~RecoilReturning();
};

#endif /* SRC_RECOILRETURNING_H_ */
