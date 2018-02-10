/*
 * ItemGun.cpp
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "ItemGun.h"
#include "ICamera3D.h"
#include "EntityProjectile.h"
#include "MatrixLib2.h"
#include "MatrixLib.h"
#include <cmath>

ItemGun::ItemGun()
{
	ID.group=ITEM_GROUP_UNIQUE_TICK;
}

void ItemGun::tick(float time,TickServiceProvider * tsp)
{

}

void ItemGun::fire(ItemAmmo * ammo, TickServiceProvider* tsp)
{
	float rand1=0;//TODO -1 1
	float rand2=0;//TODO -1 1
	float cartridgeEnergy=ammo->explEnergy*barrelEfficiency;
	float weaponEnergy=additionalEnergy+additionalEnergy*rand2*ammo->randomEnergyDev;
	float totalEnergy=cartridgeEnergy+cartridgeEnergy*rand1*ammo->randomEnergyDev+weaponEnergy;
	float vel=totalEnergy*2/(ammo->bulletMass*ammo->bulletQuantity);
	vel=sqrt(vel);
	float maxSpreadX=0;//TODO calculate hor
	float maxSpreadY=0;//TODO calculate vert
	int bullets=ammo->bulletQuantity;
	for(int i = 0 ; i<bullets ; i++)
	{
		float rand3=0;//TODO -1 1
		float rand4=0;//TODO -1 1
		ICamera3D * cam=tsp->getHolderCamera();
		vec3 pos(cam->posX,cam->posY,cam->posZ);
		vec3 vBase=(0,0,-vel);//rand3*vel*maxSpreadX,rand4*vel*maxSpreadY,-vel);//TODO spread pattern should be a circle
		MatrixLib2 ml(2);
		ml.loadIdentity();
		ml.rotatef(-cam->beta, 0, 1, 0);
		ml.rotatef(-cam->alpha, 1, 0, 0);
		ml.rotatef(-cam->gamma, 0, 0, 1);
		ml.scalef(maxSpreadX,maxSpreadY,1);//TODO
		ml.rotatef(rand3*180, 0, 0, 1);
		vec3 vProj(0,0,0);//TODO fuck it why is () not working
		vProj.x = ml.curMatrix[0] * vBase.x + ml.curMatrix[4] * vBase.y + ml.curMatrix[8] * vBase.z;
		vProj.y = ml.curMatrix[1] * vBase.x + ml.curMatrix[5] * vBase.y + ml.curMatrix[9] * vBase.z;
		vProj.z = ml.curMatrix[2] * vBase.x + ml.curMatrix[6] * vBase.y + ml.curMatrix[10] * vBase.z;

		EntityProjectile * proj=new EntityProjectile(ammo,pos,vProj);
		tsp->spawnEntity(proj);
	}
}

ItemGun::~ItemGun()
{
	// TODO Auto-generated destructor stub
}

