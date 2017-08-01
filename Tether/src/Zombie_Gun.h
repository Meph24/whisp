#ifndef SRC_ZOMBIE_GUN_H_
#define SRC_ZOMBIE_GUN_H_

#include <SFML/Audio.hpp>

#include "Zombie_Projectile.h"
#include "ICamera3D.h"
#include "ITexture.h"
class Zombie_Projectile;
class Zombie_Gun
{
	float rld;
	float timer;
public:
	sf::SoundBuffer sBuf;
	float baseDmg;
	float projectileSpeed;
	std::string name;
	float pitch;

	int curSound;
	int maxSound;
	sf::Sound ** sounds;
	Zombie_Gun(std::string weaponName,float ProjectileSpeed, float BaseDmg,float ReloadTime,const std::string& filename,float pitchModifier);
	~Zombie_Gun();
	Zombie_Projectile * tryShoot(ICamera3D * cam, ITexture * tex);
	void tick(float sec);
};

#endif /*SRC_ZOMBIE_GUN_H_*/
