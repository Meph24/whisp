#ifndef SRC_ZOMBIE_GUN_H_
#define SRC_ZOMBIE_GUN_H_

class ItemAmmo;
class ICamera3D;
class EntityPlayer;
class ITexture;
class ChunkManager;

#include "Timestamp.h"
#include "RecoilReturning.h"

#include "noise/noise.h"
#include <SFML/Audio.hpp>

#include <glm/glm.hpp>

using glm::vec3;

using namespace noise;

class Zombie_Gun
{
	Timestamp lastTimestamp;
	float rld;
	float timer;
	ItemAmmo * pType;

	module::Perlin nm;
	float noiseTimer=0;
	RecoilReturning recoilM;
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

	Zombie_Gun(Timestamp initTimestamp,std::string weaponName,float ReloadTime,const std::string& filename,float pitchModifier,ItemAmmo * pType,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread);
	~Zombie_Gun();
	void tryShoot(Timestamp callTimestamp,ICamera3D * cam,EntityPlayer * player, ITexture * tex,ChunkManager * cm);
	void tick(Timestamp callTimestamp,ICamera3D * cam,EntityPlayer * player, ITexture * tex,ChunkManager * cm);//TODO tsp
	void stopShooting();
};

#endif /*SRC_ZOMBIE_GUN_H_*/
