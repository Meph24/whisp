#pragma once
#include "Zombie_Enemy.h"
#include "CameraFP.h"
#include <SFML/Window.hpp>
#include "Zombie_MouseInput.h"
#include "Zombie_KeyInput.h"
#include "PerformanceMeter.h"
#include "TextureStatic2D.h"
#include "Zombie_Physics.h"
#include "EntityProjectile.h"
#include "EntityPlayer.h"
#include "Graphics2D.h"
#include "Zombie_Gun.h"
#include "Zombie_AmmoType.h"

//TEST
#include "QuatMandel.h"

#include "PerformanceMeter.h"
#include "DebugScreen.h"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
class Zombie_World: public TickServiceProvider, DrawServiceProvider
{
	//TEST
	//QuatMandel qm;

	bool spawnZombies;
	bool flatEarth;
	int timestep=0;
	int zCount;//max number of zombies
	int pCount;
	int wCount;
	float characterHeight=1.6f;
	Zombie_Enemy ** zombies;
	//Zombie_Projectile ** shots;
	EntityProjectile ** shots;
	Zombie_KeyInput * keyInp;
	PerformanceMeter * pm;
	DebugScreen * ds;
	Graphics2D * g;

	Zombie_Gun ** guns;
	int currentGun;

	int score=0;
	float hitmark = 0;
	
	float playerHP;

	EntityPlayer player;

	Zombie_Physics * physics;
	ChunkManager * cm;

	ITexture * zombieTex;
	ITexture * grass;
	ITexture * shot;
	ITexture * tree;
	ITexture * leaves;
	TexParamSet * tps;

	TexParamSet * tps2;

	int chunkLoadRate;
	float lodQuality;
	int zombieDist;

	void removeZombie(int zid);
	void render(float seconds);
	void doPhysics(float seconds);
	void spawnZombie();
	void restart();

	bool reset = false;

	int entityIndex=0;//TODO
public:
	Zombie_MouseInput * mouseInp;
	ICamera3D * cam;
	bool debugScreen=false;
	float timeFactor = 1;
	Zombie_World(sf::Window * w);
	~Zombie_World();



	//TickServiceProvider
	virtual void spawnEntity(Entity * e);//spawns Entity into World
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held
	virtual ChunkManager * getChunkManager();
	virtual void requestDestroy(Entity * e);//do not call yourself, call Entity.requestDestroy(tsp) instead!




	void loadStandardTex();

	void trigger(bool pulled);

	void switchWeapon(int dir);

	void loop();

	void markRestart();
};

