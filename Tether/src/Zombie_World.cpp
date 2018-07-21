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
		replaceThisTimestamp(0)
{
	Cfg cfg({ "./res/config.txt" });
	int physDist=*cfg.getint("graphics", "physicsDistance");
	int renderDist=*cfg.getint("graphics", "renderDistance");
	chunkLoadRate=*cfg.getint("graphics", "chunkLoadRate");
	lodQuality=*cfg.getfloat("graphics", "terrainQuality");
	std::cout<<"testStart"<<std::endl;
	cm=new ChunkManager(16,physDist*2,renderDist,9.81f);
	currentGun=0;
	spawnZombies=true;
	zCount = *cfg.getint("test", "zombies");
	zombieDist = *cfg.getint("test", "zombieDist");
	zombies = new Zombie_Enemy *[zCount];
	for (int i = 0; i < zCount; i++)
	{
		zombies[i] = 0;
	}
	pCount = 1024;//TODO
	shots = new EntityProjectile *[pCount];
	for (int i = 0; i < pCount; i++)
	{
		shots[i] = 0;
	}
	wCount = 5;
	guns = new Zombie_Gun * [wCount];

	guns[0] = new Zombie_Gun("Glock 17 9mm",0.2f,"res/gunshot.wav",0.9f,new ItemAmmo(358, 79.5f,0.001628170585565067f),false,{2,0.15f,0},{1,0.5f,0});//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns[1] = new Zombie_Gun("Flamethrower",0.04f,"res/mortar_shoot.wav",1,new ItemAmmo(20, 75,0.005f),true,{0.2f,0,0},{0.05f,0.01f,0});//new Zombie_Gun(30000, 800,5.0f);
	guns[2] = new Zombie_Gun("American 180 .22",0.05f,"res/gunshot.wav",1.2f,new ItemAmmo(440,31.8f,0.0022272754325748604f),true,{0.5f,0,0},{0.5f,0.5f,0});//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns[3] = new Zombie_Gun("Barret M95 .50BMG",1.5f,"res/gunshot.wav",0.6f,new ItemAmmo(900, 3166,0.0004f),false,{5,0,0},{2,2,0});
	guns[4] = new Zombie_Gun("G3A3 .308",0.12f,"res/gunshot.wav",0.75f,new ItemAmmo(800, 200,0.0008f),true,{3,0,0},{1.5f,1.5f,0});

	float characterSpeed=30.6f;//debug speed=30.6; release speed should be 3.6f

	float sensX = *cfg.getfloat("input", "sensitivityX");
	float sensY = *cfg.getfloat("input", "sensitivityY");

	player=new EntityPlayer(replaceThisTimestamp,{{0,0},{0,0},{0,0}},w,sensX,sensY,characterSpeed);
	player->cam->zoom = 1;//TODO better zoom
	adQ=new AdaptiveQuality(32,player->cam->maxView,0.001f*(*cfg.getfloat("graphics","maxRenderTime")));//TODO not hard code


	pm = new PerformanceMeter(12,1000);
	pm->roundtripUpdateIndex = 0;

	//dirty
	keyInput = player->keyInp;
	mouseInput = player->mouseInp;

	physics = new Zombie_Physics(zCount * 2);
	physics->cm=cm;//TODO implement cleaner solution
	physics->pCount = &pCount;
	physics->projectiles = shots;
	ds=new DebugScreen(pm,&g);
	pm->setName("       Total time",-1);
	pm->setName("        flush etc",0);
	pm->setName("        guns tick",1);
	pm->setName("            spawn",2);
	pm->setName("      zombie tick",3);
	pm->setName("       chunk tick",4);
	pm->setName("      bullet tick",5);
	pm->setName("           retick",6);
	pm->setName("           delete",7);
	pm->setName("           render",8);
	pm->setName("      debugScreen",9);
	pm->setName("     chunk update",10);
}

void Zombie_World::restart()
{
	spawnZombies=true;
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			removeZombie(i);
		}
	}
	player->HP = player->maxHP;
	score = 0;
	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
			shots[i]->pos.y = cm->fromMeters(-1000);
			shots[i]->posOld.y =cm->fromMeters(-1000);
		}
	}
}


Zombie_World::~Zombie_World()
{
	//missing deletes (one-time tier 1 code, so who cares)
	delete ds;
	delete physics;
	delete zombieTex;
	delete tps;
	delete pm;
	delete adQ;
	delete player;
	delete[] zombies;
	delete[] shots;
}
void Zombie_World::removeZombie(int zid)
{
	Zombie_Enemy * ptr=zombies[zid];
	if (ptr)
	{
		zombies[zid] = 0;
		delete ptr;
	}
}
void Zombie_World::render(float seconds,Timestamp t)
{

	player->applyPerspective(true,cm);
	spacevec relPos=cm->getMiddleChunk();
	float renderTime=(pm->getTime(8)+pm->getTime(0))/1000000.0f;
	float quality=adQ->getQuality(renderTime);
	Frustum * viewFrustum=player->newGetViewFrustum(cm,quality);

	grass->bind();
	glColor3f(0.2f, 0.6f, 0.1f);

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);

	cm->render(lodQuality,viewFrustum,relPos);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	spacevec treePos=cm->clip(cm->fromMeters(vec3(5,0,5)),true);
	spacevec relTree=treePos-relPos;
	Zombie_Tree tr=Zombie_Tree(cm->toMeters(relTree),tree, leaves);
	tr.draw();

	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			zombies[i]->draw(t,viewFrustum,cm,this);//TODO
		}
	}
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
			shots[i]->draw(t,viewFrustum,cm,this);//TODO
		}
	}
	glDisable(GL_TEXTURE_2D);


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
	int crosshairAmount = 4;//TODO 4

	glDisable(GL_TEXTURE_2D);
	glColor3f(1, hitmark, 0);

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
	EntityProjectile::setTexture(shot);
	leaves = new TextureStatic2D(tps2, "./res/leaves.png");
	leaves->update();

	g = new Graphics2D(64);

}
#include "WarnErrReporter.h"
void Zombie_World::doPhysics(float sec,Timestamp t)
{
	initNextTick();
//	spacevec mid=cm->getMiddleChunk();
	player->tick(t,this);
	hitmark -= sec * 10;
	if (hitmark < 0) hitmark = 0;

	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			zombies[i]->tick(t,this);
		}
	}
	pm->registerTime(timestep++);

//	for (int i = 0; i < zCount; i++)
//	{
//		if (zombies[i])
//		{
//			cm->registerCollisionCheck(DualPointer<Pushable>(zombies[i],zombies[i]),sec,this);
//		}
//	}
//	std::cout<<"collision counter: "<<AABB::intersectionCounter<<std::endl;
//	std::cout<<"check counter: "<<AABB::checkCounter<<std::endl;
//	cm->registerCollisionCheck(DualPointer<Pushable>(player,player),sec,this);
//	pm->registerTime(timestep++);
//#pragma omp parallel for schedule(dynamic, 1)
//	for (int i = 0; i < zCount; i++)
//	{
//		if (zombies[i])
//		{
//			if (zombies[i]->remainingHP>0)
//			{
//				float hp = zombies[i]->remainingHP;
//				zombies[i]->checkHitboxes(physics,mid,cm);
//				if (hp - (zombies[i]->remainingHP))
//				{
//					hitmark = 1;
//				}
//				int scoreplus= hp-(zombies[i]->remainingHP);
//				score +=scoreplus;
//			}
//		}
//	}
//	pm->registerTime(timestep++);



	cm->tick(t,this);
	pm->registerTime(timestep++);

	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
//			entityIndex=i;
			shots[i]->tick(t,this);
		}
	}
	pm->registerTime(timestep++);
	doReticks();
	pm->registerTime(timestep++);
	int size=toDelete.size();
	for(int i=0;i<size;i++)
	{
		int count=0;
		for(int j=0;j<size;j++)
		{
			if(toDelete[i]==toDelete[j]) count++;
		}
		if(count>1) WarnErrReporter::doubleErr("found more than one request to delete specific entity");
		destroy(toDelete[i]);
	}
	toDelete.clear();
}

void Zombie_World::loop()
{
	if (reset)
	{
		reset = false;
		restart();
	}
	if (player->HP < 0)
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
	else
	{
		timestep = 0;
		pm->registerTime(timestep++);
		float sec = pm->roundtriptime / 1000000.0f;
		sec *= timeFactor;
		if (sec < 0) sec = 0;
		else if (sec>MAX_TICK_TIME) sec = MAX_TICK_TIME;
		//TODO replace with TimestampManager
		int addTime=(int)(sec*1000000);
		replaceThisTimestamp.time+=addTime;


		cm->setMid(player->pos);


		int index = -1;
		for (int i = 0; i < pCount; i++)
		{
			if (!shots[i])
			{
				index = i;
				break;
			}
		}
		if(index!=-1)
		{
			shots[index] = guns[currentGun]->tick(replaceThisTimestamp,sec,player->cam,player,shot,cm);
		}
		pm->registerTime(timestep++);
		for(int i=0;i<40;i++)
			if (sec*4> (rand() % 32768) / 32768.0f) spawnZombie();//TODO change *2
		pm->registerTime(timestep++);
		doPhysics(sec,replaceThisTimestamp);
		pm->registerTime(timestep++);
		render(sec,replaceThisTimestamp);
		pm->registerTime(timestep++);
		if(debugScreen)
		{
			glPushMatrix();
			glLoadIdentity();
			glScalef(1, 1, -1.01f);
			glColor3f(1, 0, 0);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			vec3 ppos=cm->toMeters(player->pos);
			ds->draw(ppos.x,ppos.y,ppos.z);
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_DEPTH_TEST);
			glPopMatrix();
		}
		pm->registerTime(timestep++);
		cm->generateMissing(chunkLoadRate);
		pm->registerTime(timestep++);
	}
}



void Zombie_World::trigger(bool pulled)
{
	if (!pulled)
	{
		guns[currentGun]->stopShooting();
		return;
	}
	int index = -1;
	for (int i = 0; i < pCount; i++)
	{
		if (!shots[i])
		{
			index = i;
			break;
		}
	}
	if(index==-1) return;
	shots[index] = guns[currentGun]->tryShoot(replaceThisTimestamp,player->cam,player,shot,cm);
}

void Zombie_World::switchWeapon(int dir)
{
	guns[currentGun]->stopShooting();
	currentGun=(currentGun+dir+wCount*1024)%wCount;//TODO
	std::cout<<"gun switched to"<<currentGun<<std::endl;
}

void Zombie_World::spawnZombie()
{
	if(!spawnZombies) return;
	int index = -1;
	int z=0;
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i] == 0)
		{
			index = i;
			//break;
		}
		else z++;


	}
	if (index == -1) return;
	float r1 = (rand()%1024);///500.0f;//TODO change
	float maxDistMultiplier=1.2f;
	float r2 = (((rand()%32768)/32768.0f)*(maxDistMultiplier-1)+ 1)*zombieDist;
	zombies[index] = new Zombie_Enemy(replaceThisTimestamp,zombieTex, player->pos+cm->fromMeters(vec3(sin(r1)*r2,0, cos(r1)*r2)),cm);//TODO
	for(int i=1;i<32;i++)
	{
		int z=0;
			for (int i = 0; i < zCount; i++)
			{
				if (zombies[i] == 0)
				{
					index = i;
					//break;
				}
				else z++;


			}
			std::cout<<"zombie spawned, new zombie count:"<<z<<std::endl;
			//if(z==zCount) spawnZombies=false;
			if (index == -1) return;
		zombies[index] = new Zombie_Enemy(replaceThisTimestamp,zombieTex,  player->pos+cm->fromMeters(vec3(sin(r1)*r2+sin(i)*5,0,5*cos(i)+cos(r1)*r2)),cm);//TODO
	}
}

Entity* Zombie_World::getTarget(Entity* me)
{
	return (Entity *)player;
}

void Zombie_World::destroy(Entity* e)
{
	bool found=false;
	for (int i = 0; i < pCount; i++)
	{
		if ((Entity *)shots[i] == e)
		{
			delete shots[i];
			shots[i] = 0;
			found=true;
			break;
		}
	}
	if(!found)
	for (int i = 0; i < zCount; i++)
	{
		if ((Entity *)zombies[i] == e)
		{
			delete zombies[i];
			zombies[i] = 0;
			found=true;
			break;
		}
	}
	if(!found)
		std::cout<<"requestDestroy got called with invalid entity pointer:"<<e<<std::endl;
}

void Zombie_World::markRestart()
{
	reset = true;
}

void Zombie_World::spawnEntity(Entity* e)
{
	//TODO
	std::cout<<"not implemented yet!!!"<<std::endl;
}

ICamera3D* Zombie_World::getHolderCamera()
{
	return player->cam;
}

ChunkManager* Zombie_World::getChunkManager()
{
	return cm;
}

void Zombie_World::requestDestroy(Entity* e)
{
	toDelete.push_back(e);
//	if(((Entity *)shots[entityIndex])==e)
//	{
//		delete shots[entityIndex];
//		shots[entityIndex] = 0;
//	}
//	else
//	{
//		bool found=false;
//		for (int i = 0; i < pCount; i++)
//		{
//			if ((Entity *)shots[i] == e)
//			{
//				delete shots[i];
//				shots[i] = 0;
//				found=true;
//				break;
//			}
//		}
//		if(!found)
//		for (int i = 0; i < zCount; i++)
//		{
//			if ((Entity *)zombies[i] == e)
//			{
//				delete zombies[i];
//				zombies[i] = 0;
//				found=true;
//				break;
//			}
//		}
//		if(found)
//			std::cout<<"requestDestroy got called with other entity pointer, was found later"<<std::endl;
//		else
//			std::cout<<"requestDestroy got called with invalid entity pointer:"<<std::endl;
//	}
}
