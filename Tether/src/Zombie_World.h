#pragma once

#include <SFML/Window.hpp>

#include "ChunkManager.h"
#include "Perspective.hpp"
#include "Simulation.hpp"
#include "PerformanceMeter.h"
#include "SimulationInputStatusSet.hpp"
#include "Spacevec.h"
#include "TickServiceProvider.h"
#include "WorldTerrainDisplayer.hpp"

class DebugScreen;
class EntityPlayer;
class IWorld;
class TexParamSet;
class WorldDefault;

class Zombie_World: public Simulation
{
	ChunkManager cm_;
	WorldTerrainDisplayer world_;

	struct PrevInputStatus
	{
		SignalCounter restart =  SimulationInputStatusSet().restart;
	};
	map<User*, PrevInputStatus> user_prev_input_status;

	void restart();
	void doPhysics(const SimClock::time_point& t);
	void doLogic(const SimClock::time_point& t);

public:
	Zombie_World(const WallClock& reference_clock, Cfg& cfg);
	~Zombie_World() = default;

	void spawnZombie(const SimClock::time_point& t);
	void init();
	void step();

	virtual IWorld& world();
	virtual ITerrain * getITerrain();
	virtual Entity * getTarget( const Entity * me);

	PerformanceMeter::SingleTimer logicOutside;
	PerformanceMeter::SingleTimer logicGunTick;
	PerformanceMeter::SingleTimer logicSpawn;
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
	bool spawnZombies;
	int zCount;//max number of zombies

	int zombieDist;

	static const float chunk_size;
	static const float character_speed;
};
