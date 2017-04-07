#ifndef SRC_ZOMBIE_PROJECTILE_H_
#define SRC_ZOMBIE_PROJECTILE_H_

#include"ICamera3D.h"
#include "Zombie_Gun.h"
#include "ITexture.h"
#include "vec3.h"
class Zombie_Gun;
class Zombie_Projectile
{
	
public:
	vec3 pos;
	vec3 v;
	vec3 posOld;

	Zombie_Gun * firedFrom;
	ITexture * tex;

	Zombie_Projectile(ICamera3D * cam, Zombie_Gun * gun, ITexture * tex);
	~Zombie_Projectile();

	bool move(float seconds);//true= destroy
	void draw();
};
#endif /*SRC_ZOMBIE_PROJECTILE_H_*/
