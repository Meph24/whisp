/*
 * BulletLikeType.h
 *
 *  Created on:	Mar 29, 2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_BULLETLIKETYPE_H_
#define SRC_BULLETLIKETYPE_H_

class BulletLikeType
{
public:

	float mass;
	float dmgPerJd0;//at deform=0
	float dmgPerJd1;//at deform=1
	//TODO float penResist;//Energy deposition multiplier
	float drag;//calulated from area,Cd,mass,constants

	BulletLikeType();
	~BulletLikeType();
};

#endif /* SRC_BULLETLIKETYPE_H_ */
