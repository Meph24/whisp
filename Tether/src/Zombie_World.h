#pragma once

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"
#include "IGameMode.h"

#include "TimestampManager.h"
#include "Spacevec.h"

class AdaptiveQuality;
class PerformanceMeter;
class DebugScreen;
class EntityPlayer;
class TexParamSet;
class IWorld;
class WorldDefault;

#include <SFML/Window.hpp>


#define PM_GRAPHICS_OUTSIDE 0
#define PM_GRAPHICS_WORLD 1
#define PM_GRAPHICS_DRAWDEBUG 2
#define PM_GRAPHICS_FLUSH 3

#define PM_LOGIC_OUTSIDE 0
#define PM_LOGIC_PRECALC 1
#define PM_LOGIC_GUNTICK 2
#define PM_LOGIC_SPAWN 3
#define PM_LOGIC_TICK 4
#define PM_LOGIC_EVAL 5
#define PM_LOGIC_CHUNKMOVE 6


class Zombie_World: public DrawServiceProvider, public IGameMode
{
	bool spawnZombies;
	int zCount;//max number of zombies

	PerformanceMeter * pmLogic;
	PerformanceMeter * pmGraphics;
	DebugScreen * dsLogic;
	DebugScreen * dsGraphics;

	WorldDefault * wd;
	ChunkManager * cm;

	ITexture * zombieTex;
	ITexture * grass;
	ITexture * shot;
	ITexture * tree;
	ITexture * leaves;
	TexParamSet * tps;

	TexParamSet * tps2;

	float lodQuality;
	int zombieDist;
	AdaptiveQuality * adQ;

	void render(Timestamp t);
	void doPhysics(Timestamp t);
	void spawnZombie(Timestamp t);
	void restart();
	void drawGameOver();
	void doLogic(Timestamp t);
	void doGraphics(Timestamp t);

public:


	Zombie_World(const WallClock& reference_clock, sf::Window * w);
	~Zombie_World();

	//TickServiceProvider
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held

	ITerrain * getITerrain();
	IWorld * getIWorld();

	virtual Entity * getTarget(Entity * me);

	void init();

	void trigger(bool pulled);

	void loop();

	//from DrawServiceProvider:
	virtual ITexture * suggestFont();//returns 0 if no suggestion is made
};

