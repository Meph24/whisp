#include "Zombie_Gun.h"

#include "ICamera3D.h"
#include "EntityProjectileBulletLike.h"
#include "ChunkManager.h"
#include "EntityPlayer.h"
#include "ItemAmmo.h"
#include "CumulativeMat.hpp"
#include "glmutils.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "FloatSeconds.hpp"

using glm::vec3;


Zombie_Gun::Zombie_Gun(
		const SimClock::time_point& init_time,
		std::string weaponName, 
		float reloadTime,
		const std::string& filename,
		float pitchModifier,
		ItemAmmo * type,
		bool fullAutomatic,
		vec3 Recoil,
		vec3 RecoilSpread
		)
	: last_time(init_time)
	, rld(reloadTime)
	, timer(0)
	, pType(type)
	, nm()
	, recoilM(vec3(1,0,1), 0.5f,reloadTime)
	, sBuf()
	, name(weaponName)
	, pitch(pitchModifier)
	, fullAuto(fullAutomatic)
	, trigger(false)
	, recoil(Recoil)
	, recoilSpread(RecoilSpread)
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
#define recoilDampeningTime 0.5f
void Zombie_Gun::tryShoot(const SimClock::time_point& call_time,ICamera3D * cam,EntityPlayer * player, IWorld& iw)
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

	CumulativeMat ml;
	ml.loadIdentity();

	ml = ml 
		* glm::rotateDeg((float)(-cam->beta), vec3(0, 1, 0))
		* glm::rotateDeg((float)(-cam->alpha), vec3(1, 0, 0))
		* glm::rotateDeg((float)(-cam->gamma), vec3(0, 0, 1));
	for(int i=0;i<pType->bulletQuantity;i++)
	{
		float rand3=(rand()%1024)/512.0f-1.0f;//TODO remove
		float rand4=(rand()%1024)/512.0f-1.0f;//TODO remove
		ml.push();

		if(greater!=0)
			ml = glm::scale(ml, vec3(maxSpreadX/greater,maxSpreadY/greater,1));//allow horizontal spread to be different to vertical

		ml = ml 
			* glm::rotateDeg((float)rand4*90, vec3(0, 0, 1))//produces elliptical spread
			* glm::rotateDeg((float)rand3*greater, vec3(1, 0, 0));//distance from center of aim is chosen by rand3

		//TODO use normal vectors for shots like
		//vec3 test=cam->getNormal({0,0,-1});
		//v.x = test.x*10;
		//v.y = test.y*10;
		//v.z = test.z*10;
		vec3 velvec(velX, velY, velZ);
		v = ml * velvec;	

		EntityProjectileBulletLike * zp= new EntityProjectileBulletLike(player,pType->bulletData,call_time,player->getCamPos(),iw.fromMeters(v)+player->v);
		iw.requestEntitySpawn((Entity *)zp);

		ml.pop();
	}

	noiseTimer+=timer*3.2f;
	vec3 rand={(float)nm.GetValue(noiseTimer,0,0),(float)nm.GetValue(noiseTimer,0,55),0};
	vec3 recoilRand=recoilSpread*rand;
	recoilM.registerRecoil(recoil,recoilRand,{0,0,0});
}

void Zombie_Gun::tick(const SimClock::time_point& call_time,ICamera3D * cam,EntityPlayer * player, IWorld& iw)
{
	float sec = (float)FloatSeconds(call_time-last_time);
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
			tryShoot(call_time,cam,player,iw);
		}
	}
	last_time=call_time;
}

void Zombie_Gun::stopShooting()
{
	trigger=false;
}
