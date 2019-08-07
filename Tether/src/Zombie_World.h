#pragma once

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"

#include "TimestampManager.h"

class AdaptiveQuality;
class PerformanceMeter;
class ITexture;
class ChunkManager;
class DebugScreen;
class EntityPlayer;

#include <SFML/Window.hpp>


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

	PerformanceMeter * pmLogic;
	PerformanceMeter * pmGraphics;
	DebugScreen * dsLogic;
	DebugScreen * dsGraphics;



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

	void loop();

	void markRestart();

	//from DrawServiceProvider:
	virtual ITexture * suggestFont();//returns 0 if no suggestion is made
};

#include "PerformanceMeter.h"
#include "ITexture.h"
#include "ChunkManager.h"
#include "DebugScreen.h"
#include "EntityPlayer.h"
#include "AdaptiveQuality.h"

