/*
 * ItemGun.cpp
 *
 *  Created on:	Jan 17, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

/*
#include "ItemGun.h"

#include "ICamera3D.h"
#include "MatrixLib2.h"
#include "TickServiceProvider.h"
#include "ItemAmmo.h"
#include "ChunkManager.h"

#include <cmath>

ItemGun::ItemGun():
recoilM({0,0,0},0,0)//TODO
{
	ID.group=ITEM_GROUP_UNIQUE_TICK;
}

void ItemGun::tick(Timestamp t,TickServiceProvider * tsp)
{

}

#include "myAssert.h"
void ItemGun::fire(TickServiceProvider* tsp)
{
	assert(chamber!=0);
	assert(chamber->gunC.doIFitInside(ammoC));
	assert(chamber->amount==1);

	ItemAmmo * ammo=chamber;
	chamber=0;
	float rand1=0;//TODO -1 1
	float rand2=0;//TODO -1 1
	float cartridgeEnergy=ammo->explEnergy*barrelEfficiency;
	float weaponEnergy=additionalEnergy+additionalEnergy*rand2*ammo->randomEnergyDev;
	float totalEnergy=cartridgeEnergy+cartridgeEnergy*rand1*ammo->randomEnergyDev+weaponEnergy;
	float vel=totalEnergy*2/(ammo->bulletData.mass*ammo->bulletQuantity);
	vel=sqrt(vel);
	float maxSpreadX=0;//TODO calculate hor		degrees
	float maxSpreadY=0;//TODO calculate vert	degrees
	int bullets=ammo->bulletQuantity;
	for(int i = 0 ; i<bullets ; i++)
	{
		float rand3=0;//TODO -1 1
		float rand4=0;//TODO -1 1
		ICamera3D * cam=tsp->getHolderCamera();
//TODO		spacevec pos=tsp->getChunkManager()->fromMeters(vec3(cam->posX,cam->posY,cam->posZ));

		MatrixLib2 ml;
		ml.loadIdentity();

		ml.rotatef(-cam->beta, 0, 1, 0);
		ml.rotatef(-cam->alpha, 1, 0, 0);
		ml.rotatef(-cam->gamma, 0, 0, 1);

		float greater=maxSpreadX>maxSpreadY?maxSpreadX:maxSpreadY;
		if(greater!=0)
			ml.scalef(maxSpreadX/greater,maxSpreadY/greater,1);//allow horizontal spread to be different to vertical
		ml.rotatef(rand4*90, 0, 0, 1);//produces elliptical spread
		ml.rotatef(rand3*greater, 1, 0, 0);//distance from center of aim is chosen by rand3

		spacevec vProj;
		vProj.x = tsp->getChunkManager()->fromMeters(- ml.curMatrix[8] * vel); // ml.curMatrix[0] * vBase.x + ml.curMatrix[4] * vBase.y + ml.curMatrix[8] * vBase.z;
		vProj.y = tsp->getChunkManager()->fromMeters(- ml.curMatrix[9] * vel); // ml.curMatrix[1] * vBase.x + ml.curMatrix[5] * vBase.y + ml.curMatrix[9] * vBase.z;
		vProj.z = tsp->getChunkManager()->fromMeters(- ml.curMatrix[10] * vel);// ml.curMatrix[2] * vBase.x + ml.curMatrix[6] * vBase.y + ml.curMatrix[10] * vBase.z;

		//TODO timestampEntityProjectile * proj=new EntityProjectile(replaceThisTimestamp,ammo,pos,vProj);
		//TODO tsp->spawnEntity(proj);
	}
}
void ItemGun::loadChamber(ItemAmmo* ammo)
{
	assert(ammo!=0);
	assert(ammo->gunC.doIFitInside(ammoC));
	assert(ammo->amount==1);
	assert(chamber==0);

	chamber=ammo;
}

void ItemGun::trigger(bool pulled)
{
	triggerIsPulled=pulled;
}

ItemGun::~ItemGun()
{
	// TODO Auto-generated destructor stub
}

*/
