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

class Zombie_World
{
	//TEST
	//QuatMandel qm;

	int zCount;//max number of zombies
	int pCount;
	int wCount;
	Zombie_Enemy ** zombies;
	Zombie_Projectile ** shots;
	ICamera3D * cam;
	Zombie_KeyInput * keyInp;
	Zombie_MouseInput * mouseInp;
	PerformanceMeter * pm;
	Graphics2D * g;

	Zombie_Gun ** guns;
	int * curSound;
	int * maxSound;
	sf::Sound *** sounds;
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
	float timeFactor = 1;
	Zombie_World(sf::Window * w);
	~Zombie_World();

	void loadStandardTex();

	void trigger(bool pulled);
	void trigger2(bool pulled);

	void loop();

	void markRestart();
};

