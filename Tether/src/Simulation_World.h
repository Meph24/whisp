#pragma once

#include <memory>
#include "IGameMode.h"
#include "PerformanceMeter.h"
#include "SimulationInputStatusSet.hpp"
#include "WorldDefault.h"
#include "BenchmarkManager.h"
#include "Model.hpp"
#include "Mesh.hpp"
#include "TerrainDummy.h"

using std::unique_ptr;

class BenchmarkManager;
class EntityPlayer;
class IWorld;
class Mesh;
class TerrainDummy;
class WorldDefault;

class Simulation_World: public IGameMode
{
	WorldDefault world_;
	BenchmarkManager bm_;
	TerrainDummy terrain_;

	vector<unique_ptr<Mesh>> meshes;
	vector<unique_ptr<Model>> models;

	struct PrevInputStatus
	{
		SignalCounter benchmark = SimulationInputStatusSet().benchmark;
		SignalCounter restart = SimulationInputStatusSet().restart;
	};
	map<User*, PrevInputStatus> user_prev_input_status;

	void restart();

	void doPhysics(const SimClock::time_point& t);
	void doLogic(const SimClock::time_point& t);

public:
	Simulation_World(const WallClock& reference_clock, Cfg& cfg);

	void spawn(Entity *, spacevec);
	void init();
	void step();

	virtual IWorld& world();
	virtual ITerrain* getITerrain();
	virtual Entity* getTarget(const Entity* enemy);
	
	PerformanceMeter::SingleTimer logicOutside;
	PerformanceMeter::SingleTimer logicGunTick;
	PerformanceMeter::SingleTimer logicTick;
	PerformanceMeter::SingleTimer logicIntersectEval;
	PerformanceMeter::SingleTimer logicRetick;
	PerformanceMeter::SingleTimer logicTerrain;

private:
	int objects_count;
};

