#include "Zombie_Gun.h"

#include "ICamera3D.h"
#include "EntityProjectileBulletLike.h"
#include "ChunkManager.h"
#include "EntityPlayer.h"
#include "ItemAmmo.h"


Zombie_Gun::Zombie_Gun(Timestamp initTimestamp,std::string weaponName, float reloadTime,const std::string& filename,float pitchModifier,ItemAmmo * type,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread):
lastTimestamp(initTimestamp),rld(reloadTime), timer(0),pType(type),nm(),recoilM({1,0,1},0.5f,reloadTime),sBuf(),name(weaponName),pitch(pitchModifier),fullAuto(fullAutomatic),trigger(false),recoil(Recoil),recoilSpread(RecoilSpread)
{
	sBuf.loadFromFile(filename);
	curSound=0;
	maxSound=16;
	sounds=new sf::Sound * [maxSound];
	for(int i=0;i<maxSound;i++) sounds[i]=new sf::Sound(sBuf);

}


Zombie_Gun::~Zombie_Gun()
{
	for(int i=0;i<maxSound;i++) delete sounds[i];
	delete[] sounds;
	delete pType;
}

#include <iostream>
#include "MatrixLib2.h"
#define recoilDampeningTime 0.5f
void Zombie_Gun::tryShoot(Timestamp callTimestamp,ICamera3D * cam,EntityPlayer * player, ITexture * tex,ChunkManager * cm)
{
	trigger=true;
	if (timer <= 0)
	{
		timer = rld;
	}
	else return;

	sounds[curSound]->setVolume(10);
	sounds[curSound]->setRelativeToListener(true);//disable 3D Audio
	sounds[curSound]->setPosition(0,0,0);
	sounds[curSound]->setPitch(pitch*((rand()%16)/256.0f+1));
	sounds[curSound]->play();
	curSound=(curSound+1)%maxSound;

	vec3 v;
	float velZ = -pType->explEnergy;
	float velY = 0;
	float velX = 0;
	int shotgun=0;
	int cheat=0;
	if(pType->bulletQuantity>1) shotgun=1;
	if(pType->bulletQuantity==180) cheat=1;
	float maxSpreadX=0.5f/60.0f*(1+(1.5f*120-1)*shotgun*(1+cheat*7));//9.5f;//1/60.0f;//TODO properly implement
	float maxSpreadY=0.5f/60.0f*(1+(1.5f*120-1)*shotgun*(1+cheat*3));//9.5f;//1/60.0f;//TODO remove, test only
	float greater=maxSpreadX>maxSpreadY?maxSpreadX:maxSpreadY;

	MatrixLib2 ml(2);
	ml.loadIdentity();

	ml.rotatef(-cam->beta, 0, 1, 0);
	ml.rotatef(-cam->alpha, 1, 0, 0);
	ml.rotatef(-cam->gamma, 0, 0, 1);
	for(int i=0;i<pType->bulletQuantity;i++)
	{
		float rand3=(rand()%1024)/512.0f-1.0f;//TODO remove
		float rand4=(rand()%1024)/512.0f-1.0f;//TODO remove
		ml.pushMatrix();

		if(greater!=0)
			ml.scalef(maxSpreadX/greater,maxSpreadY/greater,1);//allow horizontal spread to be different to vertical
		ml.rotatef(rand4*90, 0, 0, 1);//produces elliptical spread
		ml.rotatef(rand3*greater, 1, 0, 0);//distance from center of aim is chosen by rand3

		//TODO use normal vectors for shots like
		//vec3 test=cam->getNormal({0,0,-1});
		//v.x = test.x*10;
		//v.y = test.y*10;
		//v.z = test.z*10;
		v.x = ml.curMatrix[0] * velX + ml.curMatrix[4] * velY + ml.curMatrix[8] * velZ;
		v.y = ml.curMatrix[1] * velX + ml.curMatrix[5] * velY + ml.curMatrix[9] * velZ;
		v.z = ml.curMatrix[2] * velX + ml.curMatrix[6] * velY + ml.curMatrix[10] * velZ;

		EntityProjectileBulletLike * zp= new EntityProjectileBulletLike(player,pType->bulletData,callTimestamp,player->getCamPos(),cm->fromMeters(v)+player->v);
		cm->requestEntitySpawn((Entity *)zp);

		ml.popMatrix();
	}

	noiseTimer+=timer*3.2f;
	vec3 rand={(float)nm.GetValue(noiseTimer,0,0),(float)nm.GetValue(noiseTimer,0,55),0};
	vec3 recoilRand=recoilSpread*rand;
	recoilM.registerRecoil(recoil,recoilRand,{0,0,0});
}

void Zombie_Gun::tick(Timestamp callTimestamp,ICamera3D * cam,EntityPlayer * player, ITexture * tex,ChunkManager * cm)
{
	float sec=callTimestamp-lastTimestamp;
	vec3 recoilMod=recoilM.getRecoilDiff(sec);
	cam->alpha-=recoilMod.x;
	cam->beta+=recoilMod.y;
	if(timer>0)
	{
		timer -= sec;
		if (timer < 0)
		{
			timer = 0;
		}
	}
	if(fullAuto)
	{
		if(trigger)
		{
			tryShoot(callTimestamp,cam,player,tex, cm);
		}
	}
	lastTimestamp=callTimestamp;
}

void Zombie_Gun::stopShooting()
{
	trigger=false;
}
