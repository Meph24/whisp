/*
 * BulletLikeType.cpp
 *
 *  Created on:	Mar 29, 2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "BulletLikeType.h"


BulletLikeType::BulletLikeType(float m, float d0, float d1, float myDrag):
mass(m),dmgPerJd0(d0),dmgPerJd1(d1),drag(myDrag)
{
}

BulletLikeType::~BulletLikeType()
{
	// TODO Auto-generated destructor stub
}

