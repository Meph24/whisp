#pragma once

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"
#include "IGameMode.h"

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

#include "SimulationInputStatusSet.hpp"

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

	void render(const SimClock::time_point& t);
	void doPhysics(const SimClock::time_point& t);
	void spawnZombie(const SimClock::time_point& t);

	SignalCounter prev_restart_signal = 0;
	void restart();

	void drawGameOver();
	void doLogic(const SimClock::time_point& t);
	void doGraphics(const SimClock::time_point& t);

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

