
#include "Simulation_World.h"

#include <random>

#include <iostream>

//dirty
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"


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


#include "EventDefines.h"
#include "EntityPlayer.h"
#include "Graphics2D.h"
#include "ChunkManager.h"
#include "Zombie_Gun.h"
#include "DebugScreen.h"
#include "EventMapper.h"
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


Simulation_World::Simulation_World(const WallClock& reference_clock, sf::Window * w)
	: IGameMode(reference_clock)
{
	test=0;
	CfgIO cfgio( "./res/config.txt" );
	Cfg cfg = cfgio.get();
//	int physDist=*cfg.getInt("graphics", "physicsDistance");
//	int renderDist=*cfg.getInt("graphics", "renderDistance");
	lodQuality=*cfg.getFlt("graphics", "terrainQuality");
	objects_count=*cfg.getInt("simulation", "objects_count");

	wd=new WorldDefault(16);

	bm=new BenchmarkManager(getIWorld());

	//cm=new ChunkManager(16,physDist*2,renderDist,16,*cfg.getInt("graphics", "chunkLoadRate"));//TODO make chunksPerLockchunk configurable
	td=new TerrainDummy(getIWorld(),getIWorld()->fromMeters(0));

	SimClock::time_point timS = clock.tick();

	eMap->registerAction(
			EVENT_ID_KEY_F3,
			MAPPER_MODE_TOGGLE,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_DEBUG_SCREEN_ACTIVE,
			EVENT_VALUE_KEY_PRESSED);
	eMap->registerAction(
			EVENT_ID_KEY_R,
			MAPPER_MODE_ADD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_RESTART,
			0);
	eMap->registerAction(
			EVENT_ID_KEY_T,
			MAPPER_MODE_ADD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_BENCHMARK,
			0);
	eMap->registerAction(
			EVENT_ID_KEY_E,
			MAPPER_MODE_ADD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_INVENTORY,
			0);
	eMap->registerAction(
			EVENT_ID_KEY_ARROW_UP,
			MAPPER_MODE_ADD,
			-CONDITION_SELECTION_ACTIVE,
			STATUS_ID_SELECTION_UP,
			0);
	eMap->registerAction(
			EVENT_ID_KEY_ARROW_DOWN,
			MAPPER_MODE_ADD,
			-CONDITION_SELECTION_ACTIVE,
			STATUS_ID_SELECTION_DOWN,
			0);
	eMap->registerAction(
			EVENT_ID_KEY_B,
			MAPPER_MODE_TOGGLE,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_DRAW_AABBs,
			EVENT_VALUE_KEY_PRESSED);
	eMap->registerAction(
			EVENT_ID_KEY_SHIFT,
			MAPPER_MODE_HOLD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_GO_UP,
			1);
	eMap->registerAction(
			EVENT_ID_KEY_CTRL,
			MAPPER_MODE_HOLD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_GO_DOWN,
			1);
	eMap->registerAction(
			EVENT_ID_KEY_Z,
			MAPPER_MODE_TOGGLE,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_SLOMO,
			EVENT_VALUE_KEY_PRESSED);
	eMap->registerAction(
			EVENT_ID_KEY_P,
			MAPPER_MODE_TOGGLE,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_PAUSE,
			EVENT_VALUE_KEY_PRESSED);
	eMap->registerAction(
			EVENT_ID_MOUSE_RMB,
			MAPPER_MODE_TOGGLE,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_ZOOM,
			EVENT_VALUE_KEY_PRESSED);
	eMap->registerAction(
			EVENT_ID_MOUSE_WHEEL,
			MAPPER_MODE_ADD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_WEAPON_SWITCH,
			0);
	eMap->registerAction(
			EVENT_ID_MOUSE_LMB,
			MAPPER_MODE_HOLD,
			CONDITION_ALWAYS_TRUE,
			STATUS_ID_TRIGGER,
			EVENT_VALUE_KEY_PRESSED);

	float characterSpeed=7.6f;

	float sensX = *cfg.getFlt("input", "sensitivityX");
	float sensY = *cfg.getFlt("input", "sensitivityY");

	player=new EntityPlayer(timS,{{0,0},{0,0},{0,0}},w,sensX,sensY,characterSpeed);
	adQ=new AdaptiveQuality(32,player->cam->maxView,0.001f*(*cfg.getFlt("graphics","maxRenderTime")));//TODO not hard code

	pmLogic = new PerformanceMeter(PM_LOGIC_CHUNKMOVE+1,1000);
	pmGraphics = new PerformanceMeter(PM_GRAPHICS_FLUSH+1,1000);
	pmLogic->roundtripUpdateIndex = 0;
	pmGraphics->roundtripUpdateIndex = 0;

	dsLogic=new DebugScreen(pmLogic,&g);
	dsGraphics=new DebugScreen(pmGraphics,&g);

	pmLogic->setName(" Total logic time",-1);
	pmLogic->setName("  precalculations",PM_LOGIC_PRECALC);
	pmLogic->setName("        guns tick",PM_LOGIC_GUNTICK);
	pmLogic->setName("            spawn",PM_LOGIC_SPAWN);
	pmLogic->setName("          physics",PM_LOGIC_PHYSICS);
	pmLogic->setName(" chunk generation",PM_LOGIC_CHUNKGEN);
	pmLogic->setName("   chunk movement",PM_LOGIC_CHUNKMOVE);
	pmLogic->setName("            other",PM_LOGIC_OUTSIDE);

	pmGraphics->setName("Total render time",-1);
	pmGraphics->setName("   world contents",PM_GRAPHICS_WORLD);
	pmGraphics->setName("     debug screen",PM_GRAPHICS_DRAWDEBUG);
	pmGraphics->setName(" GPU work + syncs",PM_GRAPHICS_FLUSH);
	pmGraphics->setName("            other",PM_GRAPHICS_OUTSIDE);

	setCam(player->cam);
}

void Simulation_World::restart()
{	
	player->HP = player->maxHP;
	player->score = 0;
	getIWorld()->clearEntities();
	//TODO clean chunks
}


Simulation_World::~Simulation_World()
{
	//missing deletes (one-time tier 1 ode, so who cares)
//	delete cm;
	delete bm;
	delete wd;

	delete dsLogic;
	delete dsGraphics;
	delete pmLogic;
	delete pmGraphics;
	delete g;

	delete zombieTex;
	delete grass;
	delete shot;
	delete tree;
	delete leaves;
	delete tps;
	delete tps2;

	delete adQ;
	delete player;
}

void Simulation_World::render(const SimClock::time_point& render_time)
{
	IWorld& iw=*getIWorld();

	callbackList.clear();
	float renderTime=(pmGraphics->getTime(PM_GRAPHICS_WORLD)+pmGraphics->getTime(PM_GRAPHICS_FLUSH))/1000000.0f;
	float quality=adQ->getQuality(renderTime);
	Frustum * viewFrustum=player->newFrustumApplyPerspective(render_time,true,this,quality);

	grass->bind();
	//cm->render(lodQuality,viewFrustum);//TODO integrate into draw()?!

	iw.draw(render_time,viewFrustum,iw,this);
	player->draw(render_time,viewFrustum,iw,this);//TODO  this is the job of an instance of IWorld
	doTransparentCallbacks(render_time,viewFrustum,iw);//TODO bugs here

	delete viewFrustum;
}

void Simulation_World::init()
{
	IWorld * iw=getIWorld();

	tps = new TexParamSet();
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAX_ANISOTROPY_EXT,16);//TODO compatibility check
	tps->addF(GL_TEXTURE_LOD_BIAS,0);
	tps->enableMipmap();
	zombieTex = new TextureStatic2D(tps, "./res/zombie.png");
	zombieTex->update();
	grass = new TextureStatic2D(tps, "./res/grass_top.png");
	grass->update();
	tree = new TextureStatic2D(tps, "./res/log_oak.png");
	tree->update();


	tps2 = new TexParamSet();
	tps2->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps2->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps2->addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tps2->addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	shot = new TextureStatic2D(tps2, "./res/fireball.png");
	shot->update();
	EntityProjectileBulletLike::setTexture(shot);
	leaves = new TextureStatic2D(tps2, "./res/leaves.png");
	leaves->update();

	g = new Graphics2D(64,getAspectRatio());

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

/*	
 *	for ( int i = 0; i < 5; ++i )
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
		ModelEntity* me = new ModelEntity(*(models[rand() % models.size()]));
		me->v = iw->fromMeters(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f)
									)
							  );

		spawn
		(	me,
			iw->fromMeters	(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f))
							)

		);

	}
	
	//moving diamond
	{
		ModelEntity* me = new ModelEntity(*(models[0]));
		me->v = iw->fromMeters(	vec3( 0.3f, 0.0f, 0.0f));
		spawn
		(	me,
			iw->fromMeters	(vec3(2.0f, 9.0f, 1.0f ))
		);
	}
	
	//moving cross
	{
		ModelEntity* me = new ModelEntity(*(models[1]));
		//TODO because of some fucking reason the cross vanishes
		// when the velocity vector gets a negative x
		// so i guess its not moving
		// the test still works with only 1 moving part so duh, but this needs to be fixed
		me->v = iw->fromMeters(	vec3( 0.0f, 0.0f, 0.0f));
		spawn
		(	me,
			iw->fromMeters	(	vec3( 10.0f, 9.0f, 1.0f) )

		);
	}
	//still diamond
	{

		ModelEntity* me = new ModelEntity(*(models[0]));
		spawn
		(	me,
			iw->fromMeters	(vec3(2.0f, 3.0f, 1.0f ))
		);
	}
	
	//still cross
	{

		ModelEntity* me = new ModelEntity(*(models[1]));
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
		ModelEntity* me = new ModelEntity(*(models[0]));
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
			factor/=2.0f;
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


}

void Simulation_World::doPhysics(const SimClock::time_point& next_tick_begin)
{
	IWorld * iw=getIWorld();

	bm->tick(next_tick_begin,this);

	initNextTick();

	iw->preTick(*this);

	player->tick(next_tick_begin,this);//TODO insert into IWorld

	iw->tick(next_tick_begin,this);

	iw->finishTick(*this);

	doReticks();

	iw->postTick(*this);

	bm->notifyTickEnded();
}

void Simulation_World::loop()
{
	if(eMap->getStatus(STATUS_ID_PAUSE))
	{	
		clock.setNextTargetRate(0.0);
	}
	else if(eMap->getStatus(STATUS_ID_SLOMO)) 
	{
		clock.setNextTargetRate(0.1);
	}
	else
	{
		clock.setNextTargetRate(1.0);
	}
	if (eMap->getStatusAndReset(STATUS_ID_RESTART))
	{
		restart();
	}
	
	SimClock::time_point new_tick_begin = clock.tick();

	if (!(player->HP < 0))
	{
		//test=(test+1)%4;
		//if(!test)
			doLogic(new_tick_begin);
	}
	doGraphics(new_tick_begin);
}

void Simulation_World::trigger(bool pulled)
{
	player->trigger(pulled, clock.now() ,shot,*getIWorld());
}

Entity* Simulation_World::getTarget(Entity* me)
{
	return (Entity *)player;
}

void Simulation_World::drawGameOver()//TODO find new home
{
	transformViewToGUI();
	glColor3f(1, 0, 0);
	g->drawString("GAME OVER", -0.8f, -0.2f, 0.4f);
	glColor3f(0, 1, 0);
	g->drawString("R=restart", -0.8f, -0.6f, 0.3f);

	char scoreString[8];

	int scoreTemp = player->score;
	for (int i = 0; i < 8; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	g->drawString("score:", -0.8f, 0.8f, 0.1f);
	g->drawString(scoreString, -0.8f, 0.62f, 0.1f, 8);
	revertView();
}

void Simulation_World::doLogic(const SimClock::time_point& next_tick_begin)
{
	IWorld * iw=getIWorld();
	pmLogic->registerTime(PM_LOGIC_OUTSIDE);
	//Timestamp t=tm.masterUpdate();

	static SimClock::time_point last_call;

	pmLogic->registerTime(PM_LOGIC_PRECALC);
	player->guns[player->currentGun]->tick(next_tick_begin, player->cam,player,shot,*getIWorld());
	pmLogic->registerTime(PM_LOGIC_GUNTICK);
	pmLogic->registerTime(PM_LOGIC_SPAWN);
	doPhysics(next_tick_begin);
	pmLogic->registerTime(PM_LOGIC_PHYSICS);
	getITerrain()->postTickTerrainCalcs(this,player->pos);
	pmLogic->registerTime(PM_LOGIC_CHUNKGEN);
	float time_since_last_call = (float) FloatSeconds ( next_tick_begin - last_call );
	td->height+=iw->fromMeters(eMap->getStatus(STATUS_ID_GO_UP)*time_since_last_call*player->speed);
	td->height-=iw->fromMeters(eMap->getStatus(STATUS_ID_GO_DOWN)*time_since_last_call*player->speed);
	pmLogic->registerTime(PM_LOGIC_CHUNKMOVE);//TODO fix perf measurements

	last_call = next_tick_begin;
}

void Simulation_World::doGraphics(const SimClock::time_point& t)
{
	IWorld * iw=getIWorld();

	drawAABBs=eMap->getStatus(STATUS_ID_DRAW_AABBs)==1;
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	if (player->HP < 0)
	{
		drawGameOver();
	}
	else
	{
		//Timestamp t=tm.getSlaveTimestamp();
		pmGraphics->registerTime(PM_GRAPHICS_OUTSIDE);
		render(t);
		pmGraphics->registerTime(PM_GRAPHICS_WORLD);
		if(eMap->getStatus(STATUS_ID_DEBUG_SCREEN_ACTIVE))
		{
			transformViewToGUI(1);
			glColor3f(1, 0, 1);
			glEnable(GL_TEXTURE_2D);
			vec3 ppos=iw->toMeters(player->pos);
			int offset=dsGraphics->draw(ppos.x,ppos.y,ppos.z,0);
			dsLogic->draw(player->pos.x.intpart,player->pos.y.intpart,player->pos.z.intpart,offset);
			glDisable(GL_TEXTURE_2D);
			revertView();
		}
		pmGraphics->registerTime(PM_GRAPHICS_DRAWDEBUG);
	}

	sf::Time waitt = sf::microseconds(1000);//TODO enable/disable depending on framerate
	sf::sleep(waitt);
	glFlush();
	pmGraphics->registerTime(PM_GRAPHICS_FLUSH);
}

ICamera3D* Simulation_World::getHolderCamera()
{
	return player->cam;
}

IWorld* Simulation_World::getIWorld()
{
	return (IWorld *)wd;//cm;
}
ITerrain* Simulation_World::getITerrain()
{
	return (ITerrain *)td;//(ITerrain *)cm;
}

void Simulation_World::spawn(Entity* e, spacevec pos)
{
	e->pos=pos;
	e->last_ticked = clock.now();
	getIWorld()->requestEntitySpawn(e);
}

ITexture* Simulation_World::suggestFont()
{
	return g->font;
}
