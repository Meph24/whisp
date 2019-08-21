
#include "Zombie_World.h"


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

#include <iostream>

Zombie_World::Zombie_World(sf::Window * w):
		tm(1,1000,40)//TODO 20/20
{
	test=0;
	CfgIO cfgio( "./res/config.txt" );
	Cfg cfg = cfgio.get();
	int physDist=*cfg.getInt("graphics", "physicsDistance");
	int renderDist=*cfg.getInt("graphics", "renderDistance");
	chunkLoadRate=*cfg.getInt("graphics", "chunkLoadRate");
	lodQuality=*cfg.getFlt("graphics", "terrainQuality");
	std::cout<<"testStart"<<std::endl;
	cm=new ChunkManager(16,physDist*2,renderDist,16,9.81f);//TODO make chunksPerLockchunk configurable
	spawnZombies=true;
	zCount = *cfg.getInt("test", "zombies");
	zombieDist = *cfg.getInt("test", "zombieDist");
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

	std::cout<<"initFin"<<std::endl;
}

void Zombie_World::restart()
{
	spawnZombies=true;
	player->HP = player->maxHP;
	player->score = 0;
	cm->clearEntities();
	//TODO clean chunks
}


Zombie_World::~Zombie_World()
{
	//missing deletes (one-time tier 1 code, so who cares)
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
void Zombie_World::render(Timestamp t)
{
	callbackList.clear();
	player->applyPerspective(t,true,cm);
	spacevec relPos=cm->getMiddleChunk();
	float renderTime=(pmGraphics->getTime(PM_GRAPHICS_WORLD)+pmGraphics->getTime(PM_GRAPHICS_FLUSH))/1000000.0f;
	float quality=adQ->getQuality(renderTime);
	Frustum * viewFrustum=player->newGetViewFrustum(cm,quality);

	grass->bind();
	cm->render(lodQuality,viewFrustum,relPos);//TODO integrate into draw()?!

	cm->draw(t,viewFrustum,cm,this);
	player->draw(t,viewFrustum,cm,this);//TODO  this is the job of the chunk manager
	doTransparentCallbacks(t,viewFrustum,cm);//TODO bugs here

	delete viewFrustum;
}

void Zombie_World::loadStandardTex()
{
	tps = new TexParamSet(2, 2);
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);

	zombieTex = new TextureStatic2D(tps, "./res/zombie.png");
	zombieTex->update();
	grass = new TextureStatic2D(tps, "./res/grass_top.png");
	grass->update();
	tree = new TextureStatic2D(tps, "./res/log_oak.png");
	tree->update();


	tps2 = new TexParamSet(2, 2);
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

	Zombie_Tree * tr=new Zombie_Tree(cm->fromMeters(vec3(5,0,5)),tree, leaves);
	cm->requestEntitySpawn(tr);

	diamond_mesh = new Mesh(diamondMesh(9, 0.3f, 2.0f));
	MeshIO meshio("./res/cross.mesh");
	cross_mesh = new Mesh(meshio.get());
	ModelEntity* diamond = new ModelEntity
		(
			Model(diamond_mesh, 0.8f)
		);
	spawnGrounded
		(
			diamond,
			cm->fromMeters(vec3(3, 0, 3))
		);
	ModelEntity* cross = new ModelEntity
		(
			Model(cross_mesh, 2.0f)
		);
	spawnGrounded
		(
			cross,
			cm->fromMeters(vec3(1, 0, 1))
		);
}


void Zombie_World::doPhysics(Timestamp t)
{
	initNextTick();
	player->tick(t,this);//TODO insert into chunk manager

	cm->tick(t,this);

	doReticks();

	cm->applyEntityChunkChanges(this);
}

void Zombie_World::loop()
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



void Zombie_World::trigger(bool pulled)
{
	if (!pulled)
	{
		player->guns[player->currentGun]->stopShooting();
		return;
	}
	player->guns[player->currentGun]->tryShoot(tm.getSlaveTimestamp(),player->cam,player,shot,cm);
}
void Zombie_World::spawnZombie(Timestamp t)
{
	if(!spawnZombies) return;

	if (Zombie_Enemy::zombieCount>=zCount) return;
	float r1 = (rand()%1024);///500.0f;//TODO change
	float maxDistMultiplier=1.2f;
	float r2 = (((rand()%32768)/32768.0f)*(maxDistMultiplier-1)+ 1)*zombieDist;
	cm->requestEntitySpawn(new Zombie_Enemy(t,zombieTex, player->pos+cm->fromMeters(vec3(sin(r1)*r2,0, cos(r1)*r2)),cm));
	std::cout<<"zombie spawned, new zombie count:"<<Zombie_Enemy::zombieCount<<std::endl;
	for(int i=1;i<32;i++)
	{
		if (Zombie_Enemy::zombieCount>=zCount) return;
		cm->requestEntitySpawn
			(
				new Zombie_Enemy
				(
					t,
					zombieTex,
					player->pos+cm->fromMeters(
							vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)
											  ),
					cm
				)
			);
		
		//TODO better spawn algorithm: https://www.youtube.com/watch?v=7WcmyxyFO7o


		//currently unused
		//float r3 = (rand()%7)+13;
		//float r4 = ((rand()%32768)/32768.0f)*4 + 5;

//		cm->requestEntitySpawn(new Zombie_Tree(player->pos+cm->fromMeters(vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)),1.2, r3, r4, tree, leaves));
		std::cout<<"zombie spawned, new zombie count:"<<Zombie_Enemy::zombieCount<<std::endl;
	}
}

Entity* Zombie_World::getTarget(Entity* me)
{
	return (Entity *)player;
}

void Zombie_World::spawn(Entity* ep, spacevec pos)
{
	ep->pos = pos;
	cm->requestEntitySpawn(ep);
}

void Zombie_World::spawnGrounded(ModelEntity* ep, spacevec pos)
{
	spacevec thispos = cm->clip(pos, true);
	cout << "Clipped pos : " << thispos << '\n';
	spacelen ground_distance = cm->fromMeters(ep->groundedDistance());
	cout << "Ground distance " << ground_distance << '\n';
	thispos.y -= ground_distance;
	cout << "Spawn Grounded on location " << thispos << '\n';
	spawn(ep, thispos);
}

void Zombie_World::drawGameOver()//TODO find new home
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

void Zombie_World::doLogic()
{
	pmLogic->registerTime(PM_LOGIC_OUTSIDE);
	Timestamp t=tm.masterUpdate();
	pmLogic->registerTime(PM_LOGIC_PRECALC);
	player->guns[player->currentGun]->tick(t,player->cam,player,shot,cm);
	pmLogic->registerTime(PM_LOGIC_GUNTICK);
	for(int i=0;i<40;i++)
		if (0.03f> (rand() % 32768) / 32768.0f) spawnZombie(t);//TODO replace by better spawn mechanic
	pmLogic->registerTime(PM_LOGIC_SPAWN);
	doPhysics(t);
	pmLogic->registerTime(PM_LOGIC_PHYSICS);
	cm->generateMissing(chunkLoadRate);
	pmLogic->registerTime(PM_LOGIC_CHUNKGEN);
	cm->setMid(player->pos,this);
	pmLogic->registerTime(PM_LOGIC_CHUNKMOVE);
}

void Zombie_World::doGraphics()
{
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

ICamera3D* Zombie_World::getHolderCamera()
{
	return player->cam;
}

ChunkManager* Zombie_World::getChunkManager()
{
	return cm;
}

ITexture* Zombie_World::suggestFont()
{
	return g->font;
}
