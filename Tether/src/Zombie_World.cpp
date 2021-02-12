#include "Zombie_World.h"

#include "AdaptiveQuality.h"
#include "CameraTP.h"
#include "Cfg.hpp"
#include "CfgIO.hpp"
#include "ChunkManager.h"
#include "DebugScreen.h"
#include "diamondMesh.hpp"
#include "EntityProjectileBulletLike.h"
#include "EntityPlayer.h"
#include "EventDefines.h"
#include "Frustum.h"
#include "Graphics2D.h"
#include "InteractFilterAlgoAsym.h"
#include "InteractFilterAlgoSym.h"
#include "ModelEntity.hpp"
#include "Mesh.hpp"
#include "MeshIO.hpp"
#include "PerformanceMeter.h"
#include "TextureStatic2D.h"
#include "WarnErrReporter.h"
#include "Zombie_Enemy.h"
#include "Zombie_Gun.h"
#include "ZombieTree.h"

#include <iostream>
using std::cout;

const float Zombie_World::chunk_size = 16;

Zombie_World::Zombie_World(const WallClock& reference_clock, Cfg& cfg)
	: Simulation(reference_clock, cfg)
	, cm_(	cfg,
			chunk_size,
			*cfg.getInt("graphics", "physicsDistance") * 2,
			*cfg.getInt("graphics", "renderDistance"),
			16,
			*cfg.getInt("graphics", "chunkLoadRate")
			)
	, world_(chunk_size, cm_)
			
	, spawnZombies(true)
	, zCount( *cfg.getInt("test", "zombies") )
	, zombieDist( *cfg.getInt("test", "zombieDist") )
{
	classID=CLASS_ID_Zombie_World;
	logicOutside 		= pmLogic.createTimestep	("            other");
	logicOutside.setAsRoundtripMarker				(" Total logic time");
	logicGunTick	 	= pmLogic.createTimestep	("        guns tick");
	logicSpawn 			= pmLogic.createTimestep	("            spawn");
	logicTick 			= pmLogic.createTimestep	("      entity tick");
	logicIntersectEval 	= pmLogic.createTimestep	("   intersect eval");
	logicRetick 		= pmLogic.createTimestep	("     re+post tick");
	logicTerrain 		= pmLogic.createTimestep	("    terrain calcs");
}

void Zombie_World::restart()
{
	spawnZombies=true;
	for(auto& p : players)
	{
		p.second->HP = p.second->maxHP;
		p.second->score = 0;
	}
	world_.clearEntities();
}

void Zombie_World::init()
{
	Zombie_Tree * tr = new Zombie_Tree(world_.fromMeters(vec3(5,0,5)));
	world_.requestEntitySpawn(tr);
}

void Zombie_World::doPhysics(const SimClock::time_point& next_tick_begin)
{
	IWorld * iw = &world();

	initNextTick();
	iw->preTick(*this);
	for(auto& p : players)
		p.second->tick(next_tick_begin,this);//TODO insert into IWorld

	iw->tick(next_tick_begin,this);
	logicTick.registerTime();

	iw->finishTick(*this);
	logicIntersectEval.registerTime();

	doReticks();
	iw->postTick(*this);
	logicRetick.registerTime();
}


void Zombie_World::step()
{
	if(players.empty())
		clock.setNextTargetRate(1.0);
	else
	{

		if(std::all_of(players.begin(), players.end(), 
			[](auto& p)->bool 
			{ return p.first->input_status.pause; }))
		{	
			clock.setNextTargetRate(0.0);
		}
		else if(std::all_of(players.begin(), players.end(), 
			[](auto& p)->bool 
			{ return p.first->input_status.slomo; }))
		{
			clock.setNextTargetRate(0.1);
		}
		else
		{
			clock.setNextTargetRate(1.0);
		}

		vector<User*> changed_restart;
		for(auto& p : players)
		{
			if(p.first->input_status.restart != user_prev_input_status[p.first].restart)
				changed_restart.push_back(p.first);

		}
		if( !changed_restart.empty() && 
			changed_restart.size() == players.size() )
		{
			for(User* user : changed_restart) 
				user_prev_input_status[user].restart = user->input_status.restart;
			restart();
		}
	}

	if(std::none_of(players.begin(), players.end(), 
		[](auto& p)->bool 
		{ return p.second->HP < 0; }))
	{
		doLogic(clock.tick());
	}
}

void Zombie_World::spawnZombie(const SimClock::time_point& t)
{
	IWorld * iw=&world();

	if(!spawnZombies) return;
	if (Zombie_Enemy::zombieCount>=zCount) return;
	if(players.empty()) return;

	float r1 = (rand()%1024);///500.0f;//TODO change
	float maxDistMultiplier=1.2f;
	float r2 = (((rand()%32768)/32768.0f)*(maxDistMultiplier-1)+ 1)*zombieDist;
	iw->requestEntitySpawn(new Zombie_Enemy(	t, 
												players.begin()->second->pos + iw->fromMeters(vec3(sin(r1)*r2,0, cos(r1)*r2)),
												this
												));
	if( *cfg.getInt("test","verbose") ) std::cout<<"zombie spawned, new zombie count:"<<Zombie_Enemy::zombieCount<<std::endl;
	for(int i=1;i<32;i++)
	{
		if (Zombie_Enemy::zombieCount>=zCount) return;
		iw->requestEntitySpawn
			(
				new Zombie_Enemy
				(
					t,
					players.begin()->second->pos+iw->fromMeters(vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)),
					this
				)
			);
		
		//TODO better spawn algorithm: https://www.youtube.com/watch?v=7WcmyxyFO7o


		//currently unused
		//float r3 = (rand()%7)+13;
		//float r4 = ((rand()%32768)/32768.0f)*4 + 5;

		if( *cfg.getInt("test","verbose") ) std::cout<<"zombie spawned, new zombie count:"<<Zombie_Enemy::zombieCount<<std::endl;
	}
}

void Zombie_World::doLogic(const SimClock::time_point& next_tick_begin)
{
	IWorld * iw=&world();
	if( *cfg.getInt("test","verbose") )
	{ 	iw->verbose=true;	}
	else iw->verbose = false;

	iw->projectileAlgo->verbose = iw->verbose;
	iw->collideAlgo->verbose = iw->verbose;
	iw->pushAlgo->verbose = iw->verbose;

	logicOutside.registerTime();

	for(auto& p : players)
		p.second->current_gun->tick(next_tick_begin, p.second.get(), world());

	logicGunTick.registerTime();


	for(int i=0;i<40;i++)
		if (0.03f> (rand() % 32768) / 32768.0f) spawnZombie( next_tick_begin );//TODO replace by better spawn mechanic

	logicSpawn.registerTime();

	doPhysics( next_tick_begin );

	if(!players.empty()) getITerrain()->postTickTerrainCalcs(this, players.begin()->second->pos);
	logicTerrain.registerTime();
}

IWorld& Zombie_World::world() { return world_; }
ITerrain* Zombie_World::getITerrain() { return &cm_; }
Entity* Zombie_World::getTarget(const Entity* me)
{
	if(players.empty()) return nullptr;
	return players.begin()->second.get();
}

void Zombie_World::serialize(sf::Packet& p, bool complete)
{
}

void Zombie_World::deserialize(sf::Packet& p, SyncableManager& sm)
{
}

void Zombie_World::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	for(auto& p : players)
		collectHere.push_back(p.second.get());
	for(auto e : world_.managedEntities)
		collectHere.push_back(e);
}

void Zombie_World::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
}
