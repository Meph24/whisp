#include "Zombie_World.h"
#include "Cfg.h"
//dirty
#include "Zombie_KeyInput.h"
#include "Zombie_MouseInput.h"
extern Zombie_KeyInput * keyInput;
extern Zombie_MouseInput * mouseInput;

#include <iostream>
Zombie_World::Zombie_World(sf::Window * w) :
playerHP(100)//,

//TEST
//qm(320,256)

{
	zCount = 128;
	zombies = new Zombie_Enemy *[zCount];
	for (int i = 0; i < zCount; i++)
	{
		zombies[i] = 0;
	}
	pCount = zCount * 8;
	shots = new Zombie_Projectile *[pCount];
	for (int i = 0; i < pCount; i++)
	{
		shots[i] = 0;
	}
	wCount = 16;
	guns = new Zombie_Gun * [wCount];
	sounds=new sf::Sound ** [wCount];
	curSound=new int[wCount];
	maxSound=new int[wCount];
	for (int i = 0; i < wCount; i++)
	{
		guns[i] = 0;
		sounds[i]=0;
		curSound[i]=0;
	}
	maxSound[0]=16;
	guns[0] = new Zombie_Gun(120000, 40,0.2f);
	if(!(guns[0]->sBuf.loadFromFile("res/gunshot.wav"))) std::cout<<"could not load gunshot.wav"<<std::endl;
	sounds[0]=new sf::Sound * [maxSound[0]];
	for(int i=0;i<maxSound[0];i++)
	{
		sounds[0][i]=new sf::Sound(guns[0]->sBuf);
	}

	maxSound[1]=1;
	guns[1] = new Zombie_Gun(30000, 800,5.0f);
	if(!(guns[1]->sBuf.loadFromFile("res/mortar_shoot.wav"))) std::cout<<"could not load mortar_shoot.wav"<<std::endl;
	sounds[1]=new sf::Sound * [1];
	sounds[1][0]=new sf::Sound(guns[1]->sBuf);

	cam = new CameraFP();
	cam->posY = 1600;//1.6m
	cam->aspect = (float)(w->getSize().x)/(w->getSize().y);


	std::cout << "aspect: " << cam->aspect << std::endl;
	mouseInp = new Zombie_MouseInput(cam, w);

	Cfg cfg({ "./res/config.txt" });
	mouseInp->sensitivityX = *cfg.getfloat("input", "sensitivityX");
	mouseInp->sensitivityY = *cfg.getfloat("input", "sensitivityY");
	keyInp = new Zombie_KeyInput(mouseInp, cam);
	keyInp->speed = 3600;

	pm = new PerformanceMeter(16);
	pm->roundtripUpdateIndex = 0;

	//dirty
	keyInput = keyInp;
	mouseInput = mouseInp;
	mouseInp->enable();

	physics = new Zombie_Physics(zCount * 2);
	physics->pCount = &pCount;
	physics->projectiles = shots;

}

void Zombie_World::restart()
{
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			removeZombie(i);
		}
	}
	playerHP = 100;
	score = 0;
	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
			shots[i]->pos.y = -1000000;
			shots[i]->posOld.y = -1000000;
		}
	}
}


Zombie_World::~Zombie_World()
{
	delete physics;
	delete zombieTex;
	delete tps;
	delete pm;
	delete keyInp;
	delete mouseInp;
	delete cam;
	delete[] zombies;
	delete[] shots;
}
void Zombie_World::removeZombie(int zid)
{
	Zombie_Enemy * ptr;
	if (ptr=zombies[zid])//= intended
	{
		zombies[zid] = 0;
		delete ptr;
	}
}
void Zombie_World::render(float seconds)
{
	keyInp->applyMovement(seconds);

	cam->applyFresh();

	grass->bind();
	glColor3f(0.2f, 0.6f, 0.1f);

	glPushMatrix();
	glTranslatef((int)(cam->posX / 1000.0f)*1000, 0, (int)(cam->posZ / 1000.0f)*1000);//set middle of ground to where player is while keeping texture grid
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	int groundSize = 128;
	glTexCoord2f(groundSize, groundSize);//ground
	glVertex3f(groundSize * 1000, 0, groundSize * 1000);

	glTexCoord2f(-groundSize, groundSize);
	glVertex3f(groundSize * 1000, 0, -groundSize * 1000);

	glTexCoord2f(-groundSize, -groundSize);
	glVertex3f(-groundSize * 1000, 0, -groundSize * 1000);

	glTexCoord2f(groundSize, -groundSize);
	glVertex3f(-groundSize * 1000, 0, groundSize * 1000);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			zombies[i]->draw(seconds);
		}
	}
	glEnable(GL_TEXTURE_2D);
	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
			shots[i]->draw();
		}
	}
	glDisable(GL_TEXTURE_2D);

	//TEST
	//static int texLoaded=0;
	//if(!texLoaded)
	//{
	//	qm.initTextures();
	//	qm.calc();
	//	qm.update();
	//	texLoaded=1;
	//}
    //
	//qm.draw();


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

	scoreTemp = playerHP;
	for (int i = 0; i < 3; i++)
	{

		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	g->drawString("health:", 7, -0.2f, 0.8f, 0.1f);
	g->drawString(scoreString+5, 3, -0.2f, 0.62f, 0.1f);
	glPopMatrix();
	float crosshairSize = 0.005f;
	int crosshairAmount = 4;

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
	shot = new TextureStatic2D(tps, "./res/fireball.png");
	shot->update();

	g = new Graphics2D(64);

	//TEST
	//zombies[0] = new Zombie_Enemy(zombieTex, 0, -2000);
	//zombies[1] = new Zombie_Enemy(zombieTex, 0, -4000);
}

void Zombie_World::doPhysics(float sec)
{
	playerHP += sec / 2;
	if (playerHP > 100) playerHP = 100;
	hitmark -= sec * 10;
	if (hitmark < 0) hitmark = 0;
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			zombies[i]->posX += (zombies[i]->speed)*cos(zombies[i]->facing *TAU/360)*sec;//Zombie walk "AI"
			zombies[i]->posZ += -(zombies[i]->speed)*sin(zombies[i]->facing*TAU/360)*sec;
			float wishAngle=atan2(zombies[i]->posX - cam->posX, zombies[i]->posZ - cam->posZ);
			wishAngle *= 360 / TAU;
			wishAngle += 90;
			float dif = abs(wishAngle - zombies[i]->facing);
			float difplus = abs(wishAngle - (zombies[i]->facing+360));
			float difminus = abs(wishAngle - (zombies[i]->facing-360));
			if (difplus < dif) zombies[i]->facing += 360;
			else if (difminus<dif) zombies[i]->facing -= 360;
			zombies[i]->facing = zombies[i]->facing *(1 - sec) + sec*wishAngle;
			physics->registerObject(i, zombies[i]->speed/30, zombies[i]->posX, zombies[i]->posZ,300*zombies[i]->size);
		}
	}
	physics->registerObject(zCount, keyInp->speed/30, cam->posX, cam->posZ, 400);
	physics->doPushPhysics();
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i])
		{
			if ((zombies[i]->dead) != 0)
			{
				(zombies[i]->dead) += sec * 240;
				if ((zombies[i]->dead)>90) removeZombie(i);
			}
			else
			{
				float hp = zombies[i]->remainingHP;
				zombies[i]->checkHitboxes(physics);
				if (hp - (zombies[i]->remainingHP))
				{
					hitmark = 1;
				}
				score += hp-(zombies[i]->remainingHP) ;
				Zombie_Physics::motion m = physics->getMotion(i, sec);
				zombies[i]->posX += m.x;
				zombies[i]->posZ += m.z;
				if ((zombies[i]->remainingHP) < 0) (zombies[i]->dead) += sec * 240;
			}
		}
	}
	Zombie_Physics::motion m = physics->getMotion(zCount, sec);
	cam->posX += m.x;
	cam->posZ += m.z;
	playerHP -= (m.x*m.x + m.z*m.z) / 64.0f;


	for (int i = 0; i < pCount; i++)
	{
		if (shots[i])
		{
			if (shots[i]->move(sec))
			{
				delete shots[i];
				shots[i] = 0;
			}
		}
	}


}

void Zombie_World::loop()
{
	if (reset)
	{
		reset = false;
		restart();
	}
	if (playerHP < 0)
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
		//score = 52;
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
		int timestep = 0;
		pm->registerTime(timestep++);
		float sec = pm->roundtriptime / 1000000.0f;
		sec *= timeFactor;
		if (sec < 0) sec = 0;
		else if (sec>MAX_TICK_TIME) sec = MAX_TICK_TIME;
		guns[0]->tick(sec);
		guns[1]->tick(sec);
		if (sec*2 > (rand() % 32768) / 32768.0f) spawnZombie();
		pm->registerTime(timestep++);
		doPhysics(sec);
		pm->registerTime(timestep++);
		render(sec);
		pm->registerTime(timestep++);
	}
}



void Zombie_World::trigger(bool pulled)
{
	if (!pulled) return;
	if (guns[0]->tryShoot())
	{
		int index = 0;
		for (int i = 0; i < pCount; i++)
		{
			if (!shots[i])
			{
				index = i;
				break;
			}
		}
		shots[index] = new Zombie_Projectile(cam, guns[0], shot);
		sounds[0][curSound[0]]->play();
		sounds[0][curSound[0]]->setPitch((rand()%16)/256.0f+1);
		curSound[0]=(curSound[0]+1)%maxSound[0];
	}
}
void Zombie_World::trigger2(bool pulled)
{
	if (!pulled) return;
	if (guns[1]->tryShoot())
	{
		int index = 0;
		for (int i = 0; i < pCount; i++)
		{
			if (!shots[i])
			{
				index = i;
				break;
			}
		}
		shots[index] = new Zombie_Projectile(cam, guns[1], shot);
		sounds[1][curSound[1]]->play();
		sounds[1][curSound[1]]->setPitch((rand()%16)/256.0f+1);
		curSound[1]=(curSound[1]+1)%maxSound[1];
	}
}


void Zombie_World::spawnZombie()
{
	int index = -1;
	for (int i = 0; i < zCount; i++)
	{
		if (zombies[i] == 0)
		{
			index = i;
			break;
		}
	}
	if (index == -1) return;
	float r1 = rand();
	float r2 = (rand()%32768)*8.0f + 20000;
	zombies[index] = new Zombie_Enemy(zombieTex, sin(r1)*r2+cam->posX, cos(r1)*r2+cam->posZ);
}


void Zombie_World::markRestart()
{
	reset = true;
}
