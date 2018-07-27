#pragma once
#include "Zombie_Enemy.h"
#include "CameraFP.h"
#include <SFML/Window.hpp>
#include "PerformanceMeter.h"
#include "TextureStatic2D.h"
#include "EntityProjectile.h"
#include "EntityPlayer.h"
#include "Graphics2D.h"
#include "Zombie_Gun.h"
#include "AdaptiveQuality.h"

#include "Pushable.h"
#include "DualPointer.h"

#include "PerformanceMeter.h"
#include "DebugScreen.h"
#include "ChunkManager.h"
#include "TickServiceProvider.h"
class Zombie_World: public TickServiceProvider, DrawServiceProvider
{
	Timestamp replaceThisTimestamp;//TODO replace with TimestampManager
	bool spawnZombies;
	int timestep=0;
	int zCount;//max number of zombies
	int wCount;

	PerformanceMeter * pm;
	DebugScreen * ds;
	Graphics2D * g;

	Zombie_Gun ** guns;
	int currentGun;

	int score=0;
	float hitmark = 0;
	


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
	AdaptiveQuality * adQ;

	void render(float sec,Timestamp t);
	void doPhysics(float sec,Timestamp t);
	void spawnZombie();
	void restart();

	bool reset = false;

public:
	EntityPlayer * player;
	bool debugScreen=false;
	float timeFactor = 1;
	Zombie_World(sf::Window * w);
	~Zombie_World();


	//TickServiceProvider
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held
	virtual ChunkManager * getChunkManager();

	virtual Entity * getTarget(Entity * me);


	void loadStandardTex();

	void trigger(bool pulled);

	void switchWeapon(int dir);

	void loop();

	void markRestart();
};

