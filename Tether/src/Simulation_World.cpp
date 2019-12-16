
#include "Simulation_World.h"

#include "randomModel.hpp"

using randommodel::randomModel;


#include <iostream>

using std::cout;

//dirty
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"

extern Zombie_KeyInput* keyInput;
extern Zombie_MouseInput* mouseInput;

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

#include "randomModel.hpp"


Simulation_World::Simulation_World(sf::Window * w)
{
	test=0;
	CfgIO cfgio( "./res/config.txt" );
	Cfg cfg = cfgio.get();
	int physDist=*cfg.getInt("graphics", "physicsDistance");
	int renderDist=*cfg.getInt("graphics", "renderDistance");
	chunkLoadRate=*cfg.getInt("graphics", "chunkLoadRate");
	lodQuality=*cfg.getFlt("graphics", "terrainQuality");
	objects_count=*cfg.getInt("simulation", "objects_count");

	cm = new ChunkManager(16,physDist*2,renderDist,16);//TODO make chunksPerLockchunk configurable

	Timestamp timS=tm.getSlaveTimestamp();

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

	float characterSpeed=30.6f;//debug speed=30.6; release speed should be 3.6f

	float sensX = *cfg.getFlt("input", "sensitivityX");
	float sensY = *cfg.getFlt("input", "sensitivityY");

	player=new EntityPlayer(timS,{{0,0},{0,0},{0,0}},w,sensX,sensY,characterSpeed);
	player->cam->zoom = 1;//TODO better zoom
	adQ=new AdaptiveQuality(32,player->cam->maxView,0.001f*(*cfg.getFlt("graphics","maxRenderTime")));//TODO not hard code

	pmLogic = new PerformanceMeter(PM_LOGIC_CHUNKMOVE+1,1000);
	pmGraphics = new PerformanceMeter(PM_GRAPHICS_FLUSH+1,1000);
	pmLogic->roundtripUpdateIndex = 0;
	pmGraphics->roundtripUpdateIndex = 0;

	//dirty
	keyInput = player->keyInp;
	mouseInput = player->mouseInp;

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
	spawnZombies=false;
	player->HP = player->maxHP;
	player->score = 0;
	cm->clearEntities();
	//TODO clean chunks
}


Simulation_World::~Simulation_World()
{
	//missing deletes (one-time tier 1 ode, so who cares)
	delete cm;

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

void Simulation_World::render(Timestamp t)
{
	callbackList.clear();
	float renderTime=(pmGraphics->getTime(PM_GRAPHICS_WORLD)+pmGraphics->getTime(PM_GRAPHICS_FLUSH))/1000000.0f;
	float quality=adQ->getQuality(renderTime);
	Frustum * viewFrustum=player->newFrustumApplyPerspective(t,true,this,quality);

	grass->bind();
	cm->render(lodQuality,viewFrustum);//TODO integrate into draw()?!

	cm->draw(t,viewFrustum,cm,this);
	player->draw(t,viewFrustum,cm,this);//TODO  this is the job of the chunk manager
	doTransparentCallbacks(t,viewFrustum,cm);//TODO bugs here

	delete viewFrustum;
}

void Simulation_World::loadStandardTex()
{
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

//	Zombie_Tree * tr=new Zombie_Tree(cm->fromMeters(vec3(5,0,5)),tree, leaves);
//	cm->requestEntitySpawn(tr);

	meshes.emplace_back( new Mesh(diamondMesh(7, 0.3f, 2.0f)) );
	MeshIO meshio("./res/cross.mesh");
	meshes.emplace_back( new Mesh(meshio.get()) );
	meshes.emplace_back( new Mesh(torusMesh(1.5f, 9, 0.3f, 5)) );

	for ( int i = 0; i < 5; ++i )
	{
		//create 6 more meshes by random
		meshes.emplace_back( new Mesh ( randommodel::randomMesh(20, 3.0f)));
	}


	for ( int i = 0; i < objects_count; ++i )
	{
		Model rand_model = randomModel(meshes);
		models.emplace_back( new Model(rand_model) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		me->v = cm->fromMeters(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f)
									)
							  );
		spawn
		(	me,
			cm->fromMeters	(	vec3(	randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f),
										randommodel::randomFloat(-30.0f, 30.0f))
							)

		);

	}
	
	//moving diamond
	{
		models.emplace_back( new Model(meshes[0].get()) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		me->v = cm->fromMeters(	vec3( 0.6f, 0.0f, 0.0f));
		spawn
		(	me,
			cm->fromMeters	(vec3(1.0f, 10.0f, 0.0f ))
		);
	}
	
	//moving cross
	{
		models.emplace_back( new Model(meshes[1].get()) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		me->v = cm->fromMeters(	vec3( -0.6f, 0.0f, 0.0f));
		spawn
		(	me,
			cm->fromMeters	(	vec3( 12.0f, 10.0f, 0.0f))

		);
	}
	//still diamond
	{
		models.emplace_back( new Model(meshes[0].get()) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		spawn
		(	me,
			cm->fromMeters	(vec3(1.0f, 3.0f, 0.0f ))
		);
	}
	
	//still cross
	{
		models.emplace_back( new Model(meshes[1].get()) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		spawn
		(	me,
			cm->fromMeters	(	vec3( 12.0f, 3.0f, 0.0f))

		);
	}

	//donut
	//TODO turn the donut
	{
		models.emplace_back( new Model(meshes[2].get()) );

		ModelEntity* me = new ModelEntity(*(models.back()));
		spawn
		(	me,
			cm->fromMeters	(vec3(12.0f, 5.0f, 12.0f ))
		);
	}

	//diamond moving through donut
	{
		models.emplace_back( new Model(meshes[0].get()) );
	//TODO move the diamond 
		ModelEntity* me = new ModelEntity(*(models.back()));
		spawn
		(	me,
			cm->fromMeters	(	vec3( 12.0f, 10.0f, 12.0f))

		);
	}


}

void Simulation_World::doPhysics(Timestamp t)
{
	initNextTick();

	cm->preTick(this);

	player->tick(t,this);//TODO insert into IWorld

	cm->tick(t,this);

	doReticks();

	cm->applyEntityChunkChanges(this);
}

void Simulation_World::loop()
{
	if (eMap->getStatusAndReset(STATUS_ID_RESTART))
	{
		restart();
	}
	if (!(player->HP < 0))
	{
		//test=(test+1)%4;
		//if(!test)
			doLogic();
	}
	doGraphics();
}

void Simulation_World::trigger(bool pulled)
{
	if (!pulled)
	{
		player->guns[player->currentGun]->stopShooting();
		return;
	}
	player->guns[player->currentGun]->tryShoot(tm.getSlaveTimestamp(),player->cam,player,shot,cm);
}

Entity* Simulation_World::getTarget(Entity* me)
{
	return (Entity *)player;
}

void Simulation_World::spawn(Entity* ep, spacevec pos)
{
	ep->pos = pos;
	cm->requestEntitySpawn(ep);
}

void Simulation_World::spawnGrounded(ModelEntity* ep, spacevec pos)
{
	spacevec thispos = cm->clip(pos, true);
	cout << "Clipped pos : " << thispos << '\n';
	spacelen ground_distance = cm->fromMeters(ep->groundedDistance());
	cout << "Ground distance " << ground_distance << '\n';
	thispos.y -= ground_distance;
	cout << "Spawn Grounded on location " << thispos << '\n';
	spawn(ep, thispos);
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

void Simulation_World::doLogic()
{
	pmLogic->registerTime(PM_LOGIC_OUTSIDE);
	Timestamp t=tm.masterUpdate();
	pmLogic->registerTime(PM_LOGIC_PRECALC);
	player->guns[player->currentGun]->tick(t,player->cam,player,shot,cm);
	pmLogic->registerTime(PM_LOGIC_GUNTICK);
	pmLogic->registerTime(PM_LOGIC_SPAWN);
	doPhysics(t);
	pmLogic->registerTime(PM_LOGIC_PHYSICS);
	cm->generateMissing(chunkLoadRate);
	pmLogic->registerTime(PM_LOGIC_CHUNKGEN);
	cm->setMid(player->pos,this);
	pmLogic->registerTime(PM_LOGIC_CHUNKMOVE);
}

void Simulation_World::doGraphics()
{
	cm->drawAABBs=eMap->getStatus(STATUS_ID_DRAW_AABBs)==1;
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	if (player->HP < 0)
	{
		drawGameOver();
	}
	else
	{
		Timestamp t=tm.getSlaveTimestamp();
		pmGraphics->registerTime(PM_GRAPHICS_OUTSIDE);
		render(t);
		pmGraphics->registerTime(PM_GRAPHICS_WORLD);
		if(eMap->getStatus(STATUS_ID_DEBUG_SCREEN_ACTIVE))
		{
			transformViewToGUI(1);
			glColor3f(1, 0, 1);
			glEnable(GL_TEXTURE_2D);
			vec3 ppos=cm->toMeters(player->pos);
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

ChunkManager* Simulation_World::getChunkManager()
{
	return cm;
}

IWorld* Simulation_World::getIWorld()
{
	return (IWorld *)cm;
}
ITerrain* Simulation_World::getITerrain()
{
	return (ITerrain *)cm;
}

ITexture* Simulation_World::suggestFont()
{
	return g->font;
}
