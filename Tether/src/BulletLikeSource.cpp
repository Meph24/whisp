/*
 * BulletLikeSource.cpp
 *
 *  Created on:	30.03.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "BulletLikeSource.h"

BulletLikeSource::BulletLikeSource()
{
	// TODO Auto-generated constructor stub

}

BulletLikeSource::~BulletLikeSource()
{
	// TODO Auto-generated destructor stub
}

void BulletLikeSource::hitCallback(float dmg, bool kill, bool projDestroyed,HittableBulletLike* victim)
{
	//TODO warn? should not be executed, should be overridden
}
