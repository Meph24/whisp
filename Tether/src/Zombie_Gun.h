#ifndef SRC_ZOMBIE_GUN_H_
#define SRC_ZOMBIE_GUN_H_

#include <SFML/Audio.hpp>

#include "Zombie_Projectile.h"
#include "ICamera3D.h"
#include "ITexture.h"
#include "Zombie_AmmoType.h"

#include "MatrixLib.h"

#include "noise/noise.h"
using namespace noise;

class Zombie_Projectile;
class Zombie_Gun
{
	float rld;
	float timer;
	Zombie_AmmoType * pType;

	module::Perlin nm;
	float noiseTimer=0;
	float recoilTimer=0;
public:
	sf::SoundBuffer sBuf;
	std::string name;
	float pitch;

	int curSound;
	int maxSound;
	sf::Sound ** sounds;
	bool fullAuto;
	bool trigger;

	vec3 recoil;
	vec3 recoilSpread;

	Zombie_Gun(std::string weaponName,float ReloadTime,const std::string& filename,float pitchModifier,Zombie_AmmoType * pType,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread);
	~Zombie_Gun();
	Zombie_Projectile * tryShoot(ICamera3D * cam, ITexture * tex);
	Zombie_Projectile * tick(float sec,ICamera3D * cam, ITexture * tex);
	void stopShooting();
};

#endif /*SRC_ZOMBIE_GUN_H_*/
