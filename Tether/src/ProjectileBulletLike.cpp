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
bool ProjectileBulletLike::collide(DualPointer<Projectile> self,HittableBulletLike* hittable,ProjectileCollision collision,TickServiceProvider* tsp)
{
	std::cout<<"hello projectile"<<std::endl;
	//some calculations
	float vBefore=tsp->getChunkManager()->toMeters(self.e->v);
	float vAfter=0;//TODO
	float deltaE=0.5*mass*(vBefore*vBefore-vAfter*vAfter);
	float dmg=deltaE*(dmgPerJd0*(1-deformation)+dmgPerJd1*deformation);

	return false;//TODO projectile currently stops immediately
}
