/*
 * HittableBulletLike.h
 *
 *  Created on:	31.01.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_HITTABLEBULLETLIKE_H_
#define SRC_HITTABLEBULLETLIKE_H_

#include "faction.h"

class HittableBulletLike
{
public:
	faction fac=FACTION_UNDEFINED;

	HittableBulletLike();
	virtual ~HittableBulletLike();


	virtual void executeHit();//TODO

};

#endif /* SRC_HITTABLEBULLETLIKE_H_ */
