#ifndef SRC_ZOMBIE_GUN_H_
#define SRC_ZOMBIE_GUN_H_

class ItemAmmo;
class ICamera3D;
class EntityPlayer;
class ITexture;
class IWorld;

#include "SimClock.hpp"
#include "RecoilReturning.h"

#include "noise/noise.h"
#include <SFML/Audio.hpp>

#include "glmutils.hpp"
#include <glm/glm.hpp>

using glm::vec3;

using namespace noise;

class Zombie_Gun
{
	SimClock::time_point last_time;
	float rld;
	float timer;
	ItemAmmo * pType;

	module::Perlin nm;
	float noiseTimer=0;
	RecoilReturning recoilM;

	ITexture* bullet_texture;

	static const float recoilDampeningTime;
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

	Zombie_Gun(const SimClock::time_point& init_time, std::string weaponName,float ReloadTime,const std::string& filename,float pitchModifier,ItemAmmo * pType,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread);
	~Zombie_Gun();
	void tryShoot(const SimClock::time_point& call_time,EntityPlayer * player, IWorld& iw);
	void tick(const SimClock::time_point& call_time, EntityPlayer* player, IWorld& iw);//TODO tsp
	void stopShooting();
};

#endif /*SRC_ZOMBIE_GUN_H_*/
