#include "Zombie_Gun.h"


Zombie_Gun::Zombie_Gun(std::string weaponName, float reloadTime,const std::string& filename,float pitchModifier,ItemAmmo * type,bool fullAutomatic,vec3 Recoil,vec3 RecoilSpread):
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
#include "MatrixLib2.h"
#define recoilDampeningTime 0.5f
EntityProjectile * Zombie_Gun::tryShoot(ICamera3D * cam, ITexture * tex,ChunkManager * cm)
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

	vec3 v;
	float velZ = -pType->explEnergy;
	float velY = 0;
	float velX = 0;
	float rand3=(rand()%1024)/512.0f-1.0f;//TODO remove
	float rand4=(rand()%1024)/512.0f-1.0f;//TODO remove
	MatrixLib2 ml(2);
	ml.loadIdentity();

	ml.rotatef(-cam->beta, 0, 1, 0);
	ml.rotatef(-cam->alpha, 1, 0, 0);
	ml.rotatef(-cam->gamma, 0, 0, 1);

	float maxSpreadX=1/60.0f;//TODO remove, test only
	float maxSpreadY=1/60.0f;//TODO remove, test only
	float greater=maxSpreadX>maxSpreadY?maxSpreadX:maxSpreadY;
	if(greater!=0)
		ml.scalef(maxSpreadX/greater,maxSpreadY/greater,1);//allow horizontal spread to be different to vertical
	ml.rotatef(rand4*90, 0, 0, 1);//produces elliptical spread
	ml.rotatef(rand3*greater, 1, 0, 0);//distance from center of aim is chosen by rand3

	v.x = ml.curMatrix[0] * velX + ml.curMatrix[4] * velY + ml.curMatrix[8] * velZ;
	v.y = ml.curMatrix[1] * velX + ml.curMatrix[5] * velY + ml.curMatrix[9] * velZ;
	v.z = ml.curMatrix[2] * velX + ml.curMatrix[6] * velY + ml.curMatrix[10] * velZ;

	EntityProjectile * zp= new EntityProjectile((ItemAmmo*)pType->newClone(),cm->fromMeters(vec3(cam->posX,cam->posY,cam->posZ)),cm->fromMeters(v));//cam, tex,pType);

	noiseTimer+=timer*3.2f;
	vec3 rand={(float)nm.GetValue(noiseTimer,0,0),(float)nm.GetValue(noiseTimer,0,55),0};
	vec3 recoilRand=recoilSpread*rand;
	recoilM.registerRecoil(recoil,recoilRand,{0,0,0});
	return zp;
}

EntityProjectile * Zombie_Gun::tick(float sec,ICamera3D * cam, ITexture * tex,ChunkManager * cm)
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
			EntityProjectile * zp=tryShoot(cam,tex, cm);
			return zp;
		}
	}
	return 0;
}

void Zombie_Gun::stopShooting()
{
	trigger=false;
}
