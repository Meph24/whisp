#pragma once

#include <memory>

using std::unique_ptr;

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"
#include "IGameMode.h"

#include "ModelEntity.hpp"
#include "Spacevec.h"
#include "PerformanceMeter.h"


class AdaptiveQuality;
class DebugScreen;
class EntityPlayer;
class Mesh;
class TexParamSet;
class IWorld;
class TerrainDummy;
class WorldDefault;
class BenchmarkManager;

#include <SFML/Window.hpp>


#include "SimulationInputStatusSet.hpp"

class Simulation_World: public DrawServiceProvider, public IGameMode
{
	int objects_count;

	BenchmarkManager * bm;

	PerformanceMeter * pmLogic;
	PerformanceMeter * pmGraphics;
	DebugScreen * dsLogic;
	DebugScreen * dsGraphics;

	WorldDefault * wd;
//	ChunkManager * cm;
	TerrainDummy * td;

	vector<unique_ptr<Mesh>> meshes;
	vector<unique_ptr<Model>> models;

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

	SignalCounter prev_restart_signal = 0;
	void restart();

	void drawGameOver();
	void doLogic(const SimClock::time_point& t);

	int test;//TODO remove

public:
	PerformanceMeter::SingleTimer logicOutside;
	PerformanceMeter::SingleTimer logicGunTick;
	PerformanceMeter::SingleTimer logicTick;
	PerformanceMeter::SingleTimer logicIntersectEval;
	PerformanceMeter::SingleTimer logicRetick;
	PerformanceMeter::SingleTimer logicTerrain;

	PerformanceMeter::SingleTimer graphicsOutside;
	PerformanceMeter::SingleTimer graphicsWorld;
	PerformanceMeter::SingleTimer graphicsDebug;
	PerformanceMeter::SingleTimer graphicsFlush;

	Simulation_World(const WallClock& reference_clock, sf::Window * w);
	~Simulation_World();

	//TickServiceProvider
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held

	ITerrain * getITerrain();
	IWorld * getIWorld();

	virtual Entity * getTarget(Entity * me);

	void spawn(Entity *, spacevec);

	void init();

	void trigger(bool pulled);

	void loop();
	void doGraphics(const SimClock::time_point& t);

	//from DrawServiceProvider:
	virtual ITexture * suggestFont();//returns 0 if no suggestion is made

};

