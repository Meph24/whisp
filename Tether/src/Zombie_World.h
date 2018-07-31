#pragma once
#include "Zombie_Enemy.h"
#include "CameraFP.h"
#include <SFML/Window.hpp>
#include "PerformanceMeter.h"
#include "TextureStatic2D.h"
#include "EntityPlayer.h"
#include "Graphics2D.h"
#include "Zombie_Gun.h"
#include "AdaptiveQuality.h"
#include "TimestampManager.h"

#include "Pushable.h"
#include "DualPointer.h"

#include "PerformanceMeter.h"
#include "DebugScreen.h"
#include "ChunkManager.h"
#include "TickServiceProvider.h"

#define PM_GRAPHICS_OUTSIDE 0
#define PM_GRAPHICS_WORLD 1
#define PM_GRAPHICS_DRAWDEBUG 2
#define PM_GRAPHICS_FLUSH 3


#define PM_LOGIC_OUTSIDE 0
#define PM_LOGIC_PRECALC 1
#define PM_LOGIC_GUNTICK 2
#define PM_LOGIC_SPAWN 3
#define PM_LOGIC_PHYSICS 4
#define PM_LOGIC_CHUNKGEN 5
#define PM_LOGIC_CHUNKMOVE 6


class Zombie_World: public TickServiceProvider, DrawServiceProvider
{
	bool spawnZombies;
	int zCount;//max number of zombies
	int wCount;

	PerformanceMeter * pmLogic;
	PerformanceMeter * pmGraphics;
	DebugScreen * dsLogic;
	DebugScreen * dsGraphics;
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

	void render(Timestamp t);
	void doPhysics(Timestamp t);
	void spawnZombie(Timestamp t);
	void restart();
	void drawGameOver();
	void doLogic();
	void doGraphics();

	bool reset = false;
	int test;//TODO remove

public:
	EntityPlayer * player;
	bool debugScreen=false;
	TimestampManager tm;
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

