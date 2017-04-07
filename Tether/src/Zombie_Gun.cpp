#include "Zombie_Gun.h"


Zombie_Gun::Zombie_Gun(float spd, float dmg, float reloadTime,const std::string& filename):
rld(reloadTime), timer(0),sBuf(), baseDmg(dmg),projectileSpeed(spd)
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
}

#include <iostream>

Zombie_Projectile * Zombie_Gun::tryShoot(ICamera3D * cam, ITexture * tex)
{
	//std::cout<<timer<<std::endl;

	if (timer <= 0)
	{
		timer = rld;
	}
	else return 0;

	sounds[curSound]->play();
	sounds[curSound]->setPitch((rand()%16)/256.0f+1);
	curSound=(curSound+1)%maxSound;

	Zombie_Projectile * zp= new Zombie_Projectile(cam, this, tex);
	return zp;
}

void Zombie_Gun::tick(float sec)
{
	timer -= sec;
	if (timer < 0) timer = 0;
}

