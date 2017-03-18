#include "Zombie_Gun.h"


Zombie_Gun::Zombie_Gun(float spd, float dmg, float reloadTime):
projectileSpeed(spd), baseDmg(dmg), rld(reloadTime),timer(0)
{

}


Zombie_Gun::~Zombie_Gun()
{
}

#include <iostream>

bool Zombie_Gun::tryShoot()
{
	std::cout<<timer<<std::endl;
	if (timer <= 0)
	{
		timer = rld;
		return true;
	}
	else return false;
}

void Zombie_Gun::tick(float sec)
{
	timer -= sec;
	if (timer < 0) timer = 0;
}

