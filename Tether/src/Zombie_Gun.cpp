#include "Zombie_Gun.h"


Zombie_Gun::Zombie_Gun(std::string weaponName, float reloadTime,const std::string& filename,float pitchModifier,Zombie_AmmoType * type,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread):
rld(reloadTime), timer(0),pType(type),nm(),sBuf(),name(weaponName),pitch(pitchModifier),fullAuto(fullAutomatic),trigger(false),recoil(Recoil),recoilSpread(RecoilSpread)
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
	std::cout<<timer<<std::endl;

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
	vec3 recoilNow=recoil*(1-recoilTimer/recoilDampeningTime)+recoilSpread*rand;
	cam->alpha-=recoilNow.x;
	cam->beta+=recoilNow.y;
	return zp;
}

Zombie_Projectile * Zombie_Gun::tick(float sec,ICamera3D * cam, ITexture * tex)
{
	if(timer>0)
	{
		recoilTimer+=sec;
		timer -= sec;
		if (timer < 0)
		{
			recoilTimer+=timer;
			timer = 0;
		}
		if(recoilTimer>recoilDampeningTime) recoilTimer=recoilDampeningTime;
	}
	else
	{
		recoilTimer-=sec;
		if(recoilTimer<0) recoilTimer=0;
	}
	if(fullAuto)
	{
		if(trigger)
		{
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
			vec3 recoilNow=recoil*(1-recoilTimer/recoilDampeningTime)+recoilSpread*rand;
			cam->alpha-=recoilNow.x;
			cam->beta+=recoilNow.y;
			return zp;
		}
	}
	return 0;
}

void Zombie_Gun::stopShooting()
{
	trigger=false;
}
