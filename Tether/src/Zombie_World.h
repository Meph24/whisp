#pragma once

#include <SFML/Window.hpp>

#include "ChunkManager.h"
#include "Perspective.hpp"
#include "IGameMode.h"
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

class Zombie_World: public IGameMode
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

private:
	bool spawnZombies;
	int zCount;//max number of zombies

	int zombieDist;

	static const float chunk_size;
	static const float character_speed;
};
