#include "Zombie_World.h"
#include "Cfg.h"
//dirty
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
extern Zombie_KeyInput * keyInput;
extern Zombie_MouseInput * mouseInput;

#include "SpeedMod.h"
#include "ZombieTree.h"

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"

#include <iostream>

Zombie_World::Zombie_World(sf::Window * w):
		tm(1,1000,40)//TODO 20/20
{
	test=0;
	Cfg cfg({ "./res/config.txt" });
	int physDist=*cfg.getint("graphics", "physicsDistance");
	int renderDist=*cfg.getint("graphics", "renderDistance");
	chunkLoadRate=*cfg.getint("graphics", "chunkLoadRate");
	lodQuality=*cfg.getfloat("graphics", "terrainQuality");
	std::cout<<"testStart"<<std::endl;
	cm=new ChunkManager(16,physDist*2,renderDist,16,9.81f);//TODO make chunksPerLockchunk configurable
	currentGun=0;
	spawnZombies=true;
	zCount = *cfg.getint("test", "zombies");
	zombieDist = *cfg.getint("test", "zombieDist");
	wCount = 8;
	guns = new Zombie_Gun * [wCount];
	Timestamp timS=tm.getSlaveTimestamp();
	guns[0] = new Zombie_Gun(timS,"Glock 17 9mm",0.2f,"res/gunshot.wav",0.9f,new ItemAmmo(358, 79.5f,0.001628170585565067f,1),false,{2,0.15f,0},{1,0.5f,0});//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns[1] = new Zombie_Gun(timS,"Flamethrower",0.04f,"res/mortar_shoot.wav",1,new ItemAmmo(20, 75,0.005f,1),true,{0.2f,0,0},{0.05f,0.01f,0});//new Zombie_Gun(30000, 800,5.0f);
	guns[2] = new Zombie_Gun(timS,"American 180 .22",0.05f,"res/gunshot.wav",1.2f,new ItemAmmo(440,31.8f,0.0022272754325748604f,1),true,{0.5f,0,0},{0.5f,0.5f,0});//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns[3] = new Zombie_Gun(timS,"Barret M95 .50BMG",1.5f,"res/gunshot.wav",0.6f,new ItemAmmo(900, 3166,0.0004f,1),false,{5,0,0},{2,2,0});
	guns[4] = new Zombie_Gun(timS,"G3A3 .308",0.12f,"res/gunshot.wav",0.75f,new ItemAmmo(800, 200,0.0008f,1),true,{3,0,0},{1.5f,1.5f,0});
	guns[5] = new Zombie_Gun(timS,"Shotgun",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,45.0f,0.0022272754325748604f,9),true,{2.5f,0,0},{1.5f,1.5f,0});
	guns[6] = new Zombie_Gun(timS,"Shotgun with Birdshot",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,0.30f,0.0038f,900),true,{2.5f,0,0},{1.5f,1.5f,0});
	guns[7] = new Zombie_Gun(timS,"Cheat Blaster 180",0.08f,"res/gunshot.wav",0.5f,new ItemAmmo(600,200.30f,0.0018f,180),true,{0.5f,0,0},{0.5f,0.5f,0});

	float characterSpeed=30.6f;//debug speed=30.6; release speed should be 3.6f

	float sensX = *cfg.getfloat("input", "sensitivityX");
	float sensY = *cfg.getfloat("input", "sensitivityY");

	player=new EntityPlayer(timS,{{0,0},{0,0},{0,0}},w,sensX,sensY,characterSpeed);
	player->cam->zoom = 1;//TODO better zoom
	adQ=new AdaptiveQuality(32,player->cam->maxView,0.001f*(*cfg.getfloat("graphics","maxRenderTime")));//TODO not hard code


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
}

void Zombie_World::restart()
{
	spawnZombies=true;
	player->HP = player->maxHP;
	score = 0;
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

	for(int i=0;i<wCount;i++)
	{
		delete guns[i];
	}
	delete guns;

	delete adQ;
	delete player;
}
void Zombie_World::render(Timestamp t)
{
	player->applyPerspective(t,true,cm);
	spacevec relPos=cm->getMiddleChunk();
	float renderTime=(pmGraphics->getTime(PM_GRAPHICS_WORLD)+pmGraphics->getTime(PM_GRAPHICS_FLUSH))/1000000.0f;
	float quality=adQ->getQuality(renderTime);
	Frustum * viewFrustum=player->newGetViewFrustum(cm,quality);

	grass->bind();
	glColor3f(0.2f, 0.6f, 0.1f);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	cm->render(lodQuality,viewFrustum,relPos);
	glDisable(GL_TEXTURE_2D);

	cm->draw(t,viewFrustum,cm,this);
	glPopMatrix();


	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glColor3f(0, 1, 0);
	glPushMatrix();
	glScalef(1, 1, -1.01f);
	char scoreString[8];
	int scoreTemp = score;
	for (int i = 0; i < 8; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10)+'0';
		scoreTemp /= 10;
	}
	g->drawString("score:", 6, -0.8f, 0.8f, 0.1f);
	g->drawString(scoreString, 8, -0.8f, 0.62f, 0.1f);

	scoreTemp = player->HP;
	for (int i = 0; i < 3; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	g->drawString("health:", 7, -0.2f, 0.8f, 0.1f);
	g->drawString(scoreString+5, 3, -0.2f, 0.62f, 0.1f);

	glColor3f(1, 1, 0);
	g->drawString("Weapon:", 7, 0.6f, -0.66f, 0.1f);
	g->drawString(guns[currentGun]->name.c_str(), guns[currentGun]->name.length(), 0.6f, -0.82f, 0.1f);
	glColor3f(0, 1, 0);
	glPopMatrix();
	float crosshairSize = 0.005f;
	int crosshairAmount = 4;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1, player->hitmark, 0);

	glPushMatrix();
	for (int i = 0; i < crosshairAmount; i++)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, crosshairSize, -1.01f);
		glVertex3f(-crosshairSize, crosshairSize * 6, -1.01f);
		glVertex3f(crosshairSize, crosshairSize * 6, -1.01f);
		glEnd();
		glRotatef(360.0f / crosshairAmount, 0, 0, 1);
	}
	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
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

	g = new Graphics2D(64);

	Zombie_Tree * tr=new Zombie_Tree(cm->fromMeters(vec3(5,0,5)),tree, leaves);
	cm->requestEntitySpawn(tr);
}
#include "WarnErrReporter.h"
void Zombie_World::doPhysics(Timestamp t)
{
	initNextTick();
	player->tick(t,this);

	cm->tick(t,this);

	doReticks();

	cm->applyEntityChunkChanges(this);
}

void Zombie_World::loop()
{
	if (reset)
	{
		reset = false;
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
		guns[currentGun]->stopShooting();
		return;
	}
	guns[currentGun]->tryShoot(tm.getSlaveTimestamp(),player->cam,player,shot,cm);
}

void Zombie_World::switchWeapon(int dir)
{
	guns[currentGun]->stopShooting();
	currentGun=(currentGun+dir+wCount*1024)%wCount;//TODO
	std::cout<<"gun switched to"<<currentGun<<std::endl;
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
		cm->requestEntitySpawn(new Zombie_Enemy(t,zombieTex,  player->pos+cm->fromMeters(vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)),cm));
		float r3 = (rand()%7)+13;
		float r4 = ((rand()%32768)/32768.0f)*4 + 5;
		cm->requestEntitySpawn(new Zombie_Tree(player->pos+cm->fromMeters(vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)),1.2, r3, r4, tree, leaves));
		std::cout<<"zombie spawned, new zombie count:"<<Zombie_Enemy::zombieCount<<std::endl;
	}
}

Entity* Zombie_World::getTarget(Entity* me)
{
	return (Entity *)player;
}

void Zombie_World::drawGameOver()
{
	glPushMatrix();
	glColor3f(1, 0, 0);
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glScalef(1, 1, -1.01f);
	g->drawString("GAME OVER", 9, -0.8f, -0.2f, 0.4f);
	glColor3f(0, 1, 0);
	g->drawString("R=restart", 9, -0.8f, -0.6f, 0.3f);

	char scoreString[8];

	int scoreTemp = score;
	for (int i = 0; i < 8; i++)
	{

		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	g->drawString("score:", 6, -0.8f, 0.8f, 0.1f);
	g->drawString(scoreString, 8, -0.8f, 0.62f, 0.1f);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void Zombie_World::doLogic()
{
	pmLogic->registerTime(PM_LOGIC_OUTSIDE);
	Timestamp t=tm.masterUpdate();
	pmLogic->registerTime(PM_LOGIC_PRECALC);
	guns[currentGun]->tick(t,player->cam,player,shot,cm);
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
		if(debugScreen)
		{
			glPushMatrix();
			glLoadIdentity();
			glScalef(1, 1, -1.01f);
			glColor3f(1, 0, 0);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			vec3 ppos=cm->toMeters(player->pos);
			int offset=dsGraphics->draw(ppos.x,ppos.y,ppos.z,0);
			dsLogic->draw(player->pos.x.intpart,player->pos.y.intpart,player->pos.z.intpart,offset);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			glPopMatrix();
		}
		pmGraphics->registerTime(PM_GRAPHICS_DRAWDEBUG);
	}

	sf::Time waitt = sf::microseconds(1000);//TODO enable/disable depending on framerate
	sf::sleep(waitt);
	glFlush();
	pmGraphics->registerTime(PM_GRAPHICS_FLUSH);
}

void Zombie_World::markRestart()
{
	reset = true;
}

ICamera3D* Zombie_World::getHolderCamera()
{
	return player->cam;
}

ChunkManager* Zombie_World::getChunkManager()
{
	return cm;
}

