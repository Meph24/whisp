/*
 * BulletLikeSource.h
 *
 *  Created on:	30.03.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_BULLETLIKESOURCE_H_
#define SRC_BULLETLIKESOURCE_H_
class HittableBulletLike;
class BulletLikeSource
{
public:
	BulletLikeSource();
	virtual ~BulletLikeSource();

	virtual void hitCallback(float dmg,bool kill,bool projDestroyed,HittableBulletLike * victim);

};
#endif /* SRC_BULLETLIKESOURCE_H_ */
