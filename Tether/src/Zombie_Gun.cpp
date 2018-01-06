#include "Zombie_Gun.h"


Zombie_Gun::Zombie_Gun(std::string weaponName, float reloadTime,const std::string& filename,float pitchModifier,Zombie_AmmoType * type,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread):
rld(reloadTime), timer(0),pType(type),nm(),recoilM({1,0,1},0.5f,reloadTime),sBuf(),name(weaponName),pitch(pitchModifier),fullAuto(fullAutomatic),trigger(false),recoil(Recoil),recoilSpread(RecoilSpread)
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
Zombie_Projectile * Zombie_Gun::tryShoot(ICamera3D * cam, ITexture * tex)
{
	trigger=true;
	if (timer <= 0)
	{
		timer = rld;
	}
	else return 0;

	sounds[curSound]->play();
	sounds[curSound]->setPitch(pitch*((rand()%16)/256.0f+1));
	curSound=(curSound+1)%maxSound;

	Zombie_Projectile * zp= new Zombie_Projectile(cam, tex,pType);
	noiseTimer+=timer*3.2f;
	vec3 rand={nm.GetValue(noiseTimer,0,0),nm.GetValue(noiseTimer,0,55),0};
	vec3 recoilRand=recoilSpread*rand;
	recoilM.registerRecoil(recoil,recoilRand,{0,0,0});
	return zp;
}

Zombie_Projectile * Zombie_Gun::tick(float sec,ICamera3D * cam, ITexture * tex)
{
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
			Zombie_Projectile * zp=tryShoot(cam,tex);
			return zp;
		}
	}
	return 0;
}

void Zombie_Gun::stopShooting()
{
	trigger=false;
}
