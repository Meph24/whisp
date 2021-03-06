
#include "Simulation_World.h"

#include <mutex>
#include <random>

#include <iostream>

#include "ZombieTree.h"
#include "TextureStatic2D.h"
#include "Zombie_Enemy.h"
#include "PerformanceMeter.h"

#include "Cfg.hpp"
#include "CfgIO.hpp"
#include "Mesh.hpp"
#include "diamondMesh.hpp"
#include "torusMesh.hpp"
#include "ModelEntity.hpp"
#include "MeshIO.hpp"
#include "TransModelEntity.hpp"

#include "EntityPlayer.h"
#include "Graphics2D.h"
#include "ChunkManager.h"
#include "Zombie_Gun.h"
#include "DebugScreen.h"
#include "CameraTP.h"
#include "AdaptiveQuality.h"
#include "EntityProjectileBulletLike.h"
#include "Frustum.h"
#include "WarnErrReporter.h"
#include "TerrainDummy.h"
#include "WorldDefault.h"
#include "BenchmarkManager.h"

#include "randomModel.hpp"
#include <glm/gtc/random.hpp>

#include "FloatSeconds.hpp"
#include "GridEntity.hpp"
#include "OxelEntity.hpp"

#include "EventDefines.h"

#include "InteractFilterAlgoAsym.h"
#include "InteractFilterAlgoSym.h"

#include <utility>

using std::lock_guard;

Simulation_World::Simulation_World(const WallClock& reference_clock, Cfg& cfg)
	: Simulation(reference_clock, cfg)
	, bm_(iw.get())
	, terrain_(iw.get(), iw->fromMeters(0))
{
	//	int physDist=*cfg.getInt("graphics", "physicsDistance");
	//	int renderDist=*cfg.getInt("graphics", "renderDistance");
	objects_count=*cfg.getInt("simulation", "objects_count");

	logicOutside=pmLogic.createTimestep			("            other");
	logicOutside.setAsRoundtripMarker			(" Total logic time");
	logicGunTick=pmLogic.createTimestep			("        guns tick");
	logicTick=pmLogic.createTimestep			("      entity tick");
	logicIntersectEval=pmLogic.createTimestep	("   intersect eval");
	logicRetick=pmLogic.createTimestep			("     re+post tick");
}

void Simulation_World::restart(){}

void Simulation_World::init()
{
	{
		Mesh m (diamondMesh(7, 0.3f, 2.0f));
		models.emplace_back(new Model (m) );
		
	}
	{
		MeshIO meshio("./res/cross.mesh");
		Mesh m (meshio.get());
		models.emplace_back(new Model( m ) );
	}

	{
		Mesh m (torusMesh(1.0f, 9, 0.3f, 5));
		models.emplace_back(new Model( m ));
		
	}
	{
		MeshIO meshio("./res/prism.mesh");
		Mesh m (meshio.get());
		models.emplace_back(new Model( m ));
	}
/*
 	for ( int i = 0; i < 5; ++i )
	{
		//create 6 more models by random
		models.emplace_back(new Model(Mesh ( randommodel::randomMesh(20, 3.0f))));
		
	}
*/
	float fi = 0.0;
	for(auto& m_uptr : models)
	{
		ModelEntity* me = new ModelEntity(*m_uptr);

		spawn(me, iw->fromMeters(vec3(20.0f, 4.0f, fi*2.5f)));
		fi += 1.0f;
	}
	for ( int i = 0; i < objects_count; ++i )
	{
		TransModelEntity* me = new TransModelEntity(*(models[rand() % models.size()]));
		me->v = iw->fromMeters(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f)
									)
							  );

		spawn
		(	
			me,
			iw->fromMeters	(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f))
							)
		);
	}
	
	//moving diamond
	{
		TransModelEntity* me = new TransModelEntity(*(models[0]));
		me->v = iw->fromMeters(	vec3( 0.3f, 0.0f, 0.0f));
		spawn
		(	me,
			iw->fromMeters	(vec3(2.0f, 9.0f, 1.0f ))
		);
	}
	
	//moving cross
	{
		TransModelEntity* me = new TransModelEntity(*(models[1]));
		//TODO because of some fucking reason the cross vanishes
		// when the velocity vector gets a negative x
		// so i guess its not moving
		// the test still works with only 1 moving part so duh, but this needs to be fixed
		me->v = iw->fromMeters(	vec3( 0.0f, 0.0f, 0.0f));
		spawn
		(	
			me,
			iw->fromMeters	( vec3( 10.0f, 9.0f, 1.0f) )
		);
	}

	//still diamond
	{

		TransModelEntity* me = new TransModelEntity(*(models[0]));
		spawn
		(	me,
			iw->fromMeters	(vec3(2.0f, 3.0f, 1.0f ))
		);
	}
	
	//still cross
	{

		TransModelEntity* me = new TransModelEntity(*(models[1]));
		spawn
		(	me,
			iw->fromMeters	(	vec3( 10.0f, 3.0f, 1.0f))

		);
	}

	//donut
	{
		TransModelEntity* me = new TransModelEntity(*(models[2]));
		me->rot = vec3(90.0f, 0.0f, 0.0f);
		spawn
		(	me,
			iw->fromMeters	(vec3(12.0f, 5.0f, 12.0f ))
		);
	}

	//diamond moving through donut
	{
		TransModelEntity* me = new TransModelEntity(*(models[0]));
		me->v = iw->fromMeters(	vec3( 0.0f, -0.2f, 0.0f));
		spawn
		(	me,
			iw->fromMeters	(	vec3( 12.0f, 10.0f, 12.0f))

		);
	}
	
	//rotating cross
	{
		TransModelEntity* me = new TransModelEntity(*(models[1]));
		me->drot=vec3(0.0f, 10.0f, 0.0f);
		spawn
		(	me,
			iw->fromMeters	(	vec3( 0.0f, 3.0f, 12.0f))

		);
	}

	//orrery
	{
		vec3 pos(15.0f, 15.0f, 15.0f);
		float factor = 2.0f;
		array<TransModelEntity*, 3> mes;

		for(auto& me : mes)
		{
			me = new TransModelEntity(*(models[2]));
			me->drot = glm::linearRand
					(	vec3(-360.0f, -360.0f, -360.0f), 
						vec3(360.0f, 360.0f, 360.0f));
			me->scale = vec3(factor, factor, factor);
			factor/=2.5f;
			spawn(me, iw->fromMeters( pos ));
		}
	}

	//cogged crosses
	{
		TransModelEntity* me0 = new TransModelEntity(*(models[1]));
		TransModelEntity* me1 = new TransModelEntity(*(models[1]));

		vec3 pos0(16.0f, 3.0f, 0.0f);

		//the cross mesh is 1.0 units wide and high
		//its model per default is upright, with the arms pointing in y and z directions
		// so only moving it in x direction for 0.9 will cause collision on arms
		vec3 pos1 = pos0; pos1.x -= 0.8;

		//offset one by rotation	
		me1->rot = vec3(0.0f, 0.0f, 45.0f);

		//and spin them both at the same speed in respective directions
		//clockwise and counter clockwise
		float vel = randommodel::randomFloat(-360.0f, 360.0f);
		vec3 rotational_velocity0(0.0f, 0.0f, vel);
		vec3 rotational_velocity1(0.0f, 0.0f, -1*vel);
		me0->drot = rotational_velocity0;
		me1->drot = rotational_velocity1;
		spawn(me0, iw->fromMeters(pos0));
		spawn(me1, iw->fromMeters(pos1));
	}
	//cogged crosses intersecting
	{
		TransModelEntity* me0 = new TransModelEntity(*(models[1]));
		TransModelEntity* me1 = new TransModelEntity(*(models[1]));

		vec3 pos0(16.0f, 5.0f, 0.0f);

		//the cross mesh is 1.0 units wide and high
		//its model per default is upright, with the arms pointing in y and z directions
		// so only moving it in x direction for 0.9 will cause collision on arms
		vec3 pos1 = pos0; pos1.x -= 0.8;

		me0->rot=vec3(0.0f, 0.0f, 45.0f);

		//we this time spin only one of the crosses
		float vel = randommodel::randomFloat(-360.0f, 360.0f);
		vec3 rotational_velocity0(0.0f, 0.0f, vel);
		me0->drot = rotational_velocity0;
		spawn(me0, iw->fromMeters(pos0));
		spawn(me1, iw->fromMeters(pos1));
	}
	//prisms
	{
		vec3 moving_offset(5.0f, 0.0f, 0.0f);
		vec3 moving_down(0.0f, -0.75f, 0.0f);
		vec3 rotation_increments(0.0f, 0.0f, 103.0f);

		vec3 start(1.0f, 1.0f, 1.0f);
		vec3 experiments_offset(0.0f, 0.0f, 3.0f);

		for ( int i = 1; i <= 128; i+=8 )
		{
			TransModelEntity* prism_static = new TransModelEntity(*(models[3]));
			TransModelEntity* prism_moving = new TransModelEntity(*(models[3]));

			prism_moving->v = iw->fromMeters(vec3(-0.5f, 0.0f, 0.0f) * (float)i);
			prism_static->rot = vec3(0.0f, 90.0f, 0.0f);
			prism_moving->drot = rotation_increments * (float)i;

			spawn(prism_static, iw->fromMeters(start + (float)i * experiments_offset));
			spawn(prism_moving, iw->fromMeters(start + (float)i * experiments_offset + moving_offset*(float)i + moving_down));
		}
	}
	//grid
	{
		Grid<32> grid;
		grid(0,0,0).existence = true;
		grid(1,0,0).existence = true;
		grid(-1,0,0).existence = true;
		grid(0,1,0).existence = true;
		grid(0,-1,0).existence = true;
		grid(0,0,1).existence = true;
		grid(0,0,-1).existence = true;
		GridEntity* ge = new GridEntity( grid );

		spawn(ge, iw->fromMeters( vec3(20.0f, 20.0f, 20.0f) ));
	}
	//oxel
	{
		OxelTree t;

		OxelTree::Creator c ( t );
		for (size_t i = 0; i < 5; i++)
		{
			c.child(3).value().material = &Material::list.dirt;
			c.child(5).value().material = &Material::list.rock;
			c.child(6).value().material = &Material::list.rock;

			c = c.child(0);
		}
		OxelEntity* oxele = new OxelEntity(t);
		spawn( oxele, iw->fromMeters( vec3(-10.0f, 0.0f, 0.0f) ) );
	}
}

void Simulation_World::doPhysics(const SimClock::time_point& next_tick_begin)
{
	bm_.tick(next_tick_begin,this);
	initNextTick();
	iw->preTick(*this);
	{
		lock_guard lg_ (players_lock);	
		for(auto& p : players)
		{
			p.second->tick(next_tick_begin,this);//TODO insert into IWorld
		}
	}
	iw->tick(next_tick_begin,this);
	logicTick.registerTime();

	iw->finishTick(*this);
	logicIntersectEval.registerTime();

	doReticks();
	iw->postTick(*this);
	logicRetick.registerTime();

	bm_.notifyTickEnded();
}

void Simulation_World::step()
{
	lock_guard lg_ (players_lock);	
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

		vector<SimulationUser*> changed_restart;
		vector<SimulationUser*> changed_benchmark;
		for(auto& p : players)
		{
			if(p.first->input_status.restart != user_prev_input_status[p.first].restart)
				changed_restart.push_back(p.first);
			if(p.first->input_status.benchmark != user_prev_input_status[p.first].benchmark)
				changed_benchmark.push_back(p.first);

		}
		if( !changed_restart.empty() && 
			changed_restart.size() == players.size() )
		{
			for(SimulationUser* user : changed_restart) 
				user_prev_input_status[user].restart = user->input_status.restart;
			restart();
		}
		if( !changed_benchmark.empty() &&
			changed_benchmark.size() == players.size() )
		{ 
			for(SimulationUser* user : changed_benchmark)
				user_prev_input_status[user].benchmark = user->input_status.benchmark;
			bm_.requestBenchmark();
		}
	}

	if(std::none_of(players.begin(), players.end(), 
		[](auto& p)->bool 
		{ return p.second->HP < 0; }))
	{
		doLogic(clock.tick());
	}
}

void Simulation_World::doLogic(const SimClock::time_point& next_tick_begin)
{
	if( *cfg.getInt("test","verbose") )
	{ 	iw->verbose=true;	}
	else iw->verbose = false;

	iw->projectileAlgo->verbose = iw->verbose;
	iw->collideAlgo->verbose = iw->verbose;
	iw->pushAlgo->verbose = iw->verbose;

	logicOutside.registerTime();

	for(auto& p : players)
		p.second->current_gun->tick(next_tick_begin, p.second, world());

	logicGunTick.registerTime();

	doPhysics(next_tick_begin);
}

void Simulation_World::spawn(Entity* e, spacevec pos)
{
	e->pos=pos;
	e->last_ticked = clock.now();
	e->bb = AABB(e->pos);
	e->onSpawn( this );
	iw->requestEntitySpawn(e);
}

ITerrain* Simulation_World::getITerrain() { return &terrain_; }
Entity* Simulation_World::getTarget(const Entity* enemy)
{
	return nullptr;
}

void Simulation_World::getReferencedSyncables(std::vector<Syncable*> collectHere)
{}

u32 Simulation_World::getClassID()
{
	return CLASS_ID_Simulation_World;
}
