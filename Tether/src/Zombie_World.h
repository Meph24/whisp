#pragma once
#include "Zombie_Enemy.h"
#include "CameraFP.h"
#include <SFML/Window.hpp>
#include "Zombie_MouseInput.h"
#include "Zombie_KeyInput.h"
#include "PerformanceMeter.h"
#include "TextureStatic2D.h"
#include "Zombie_Physics.h"
#include "Zombie_Projectile.h"
#include "Graphics2D.h"
#include "Zombie_Gun.h"

#define MAX_TICK_TIME 0.02f

//TEST
#include "QuatMandel.h"
#include "PerformanceMeter.h"
#include "DebugScreen.h"

class Zombie_World
{
	//TEST
	//QuatMandel qm;

	bool spawnZombies;
	int timestep=0;
	int zCount;//max number of zombies
	int pCount;
	int wCount;
	Zombie_Enemy ** zombies;
	Zombie_Projectile ** shots;
	Zombie_KeyInput * keyInp;
	PerformanceMeter * pm;
	DebugScreen * ds;
	Graphics2D * g;

	Zombie_Gun ** guns;
	int currentGun;

	int score=0;
	float hitmark = 0;
	
	float playerHP;


	Zombie_Physics * physics;

	ITexture * zombieTex;
	ITexture * grass;
	ITexture * shot;
	TexParamSet * tps;

	void removeZombie(int zid);
	void render(float seconds);
	void doPhysics(float seconds);
	void spawnZombie();
	void restart();

	bool reset = false;
public:
	Zombie_MouseInput * mouseInp;
	ICamera3D * cam;
	bool debugScreen=false;
	float timeFactor = 1;
	Zombie_World(sf::Window * w);
	~Zombie_World();

	void loadStandardTex();

	void trigger(bool pulled);
	//void trigger2(bool pulled);
	void switchWeapon(int dir);

	void loop();

	void markRestart();
};

