/*
 * ProjectileBulletLike.cpp
 *
 *  Created on:	31.01.2019
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "ProjectileBulletLike.h"

ProjectileBulletLike::ProjectileBulletLike()
{
	// TODO Auto-generated constructor stub

}

ProjectileBulletLike::~ProjectileBulletLike()
{
	// TODO Auto-generated destructor stub
}
#include <iostream>
bool ProjectileBulletLike::collide(HittableBulletLike* hittable,ProjectileCollision collision,TickServiceProvider* tsp)
{
	std::cout<<"hello projectile"<<std::endl;
	return false;//projectile stops
}
