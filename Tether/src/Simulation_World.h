#pragma once

#include <memory>

#include "BenchmarkManager.h"
#include "Model.hpp"
#include "Mesh.hpp"
#include "PerformanceMeter.h"
#include "Simulation.hpp"
#include "SimulationInputStatusSet.hpp"
#include "TerrainDummy.h"
#include "WorldDefault.h"

using std::unique_ptr;

class BenchmarkManager;
class EntityPlayer;
class IWorld;
class Mesh;
class TerrainDummy;
class WorldDefault;

class Simulation_World: public Simulation
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


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();

private:
	int objects_count;
};

