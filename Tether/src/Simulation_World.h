#pragma once

#include <memory>

using std::unique_ptr;

#include "TickServiceProvider.h"
#include "DrawServiceProvider.h"

#include "ModelEntity.hpp"

#include "TimestampManager.h"
#include "Spacevec.h"

class AdaptiveQuality;
class PerformanceMeter;
class DebugScreen;
class EntityPlayer;
class Mesh;
class TexParamSet;
class IWorld;

#include <SFML/Window.hpp>


#define PM_GRAPHICS_OUTSIDE 0
#define PM_GRAPHICS_WORLD 1
#define PM_GRAPHICS_DRAWDEBUG 2
#define PM_GRAPHICS_FLUSH 3

#define PM_LOGIC_OUTSIDE 0
#define PM_LOGIC_PRECALC 1
#define PM_LOGIC_GUNTICK 2
#define PM_LOGIC_SPAWN 3
#define PM_LOGIC_PHYSICS 4
#define PM_LOGIC_CHUNKGEN 5
#define PM_LOGIC_CHUNKMOVE 6


class Simulation_World: public TickServiceProvider, DrawServiceProvider
{
	bool spawnZombies;

	int objects_count;



	PerformanceMeter * pmLogic;
	PerformanceMeter * pmGraphics;
	DebugScreen * dsLogic;
	DebugScreen * dsGraphics;

	ChunkManager * cm;

	vector<unique_ptr<Mesh>> meshes;
	vector<unique_ptr<Model>> models;

	ITexture * zombieTex;
	ITexture * grass;
	ITexture * shot;
	ITexture * tree;
	ITexture * leaves;
	TexParamSet * tps;

	TexParamSet * tps2;

	int chunkLoadRate;
	float lodQuality;
	int zombieDist;
	AdaptiveQuality * adQ;

	void render(Timestamp t);
	void doPhysics(Timestamp t);
	void spawnZombie(Timestamp t);
	void restart();
	void drawGameOver();
	void doLogic();
	void doGraphics();

	int test;//TODO remove

public:
	EntityPlayer * player;
	TimestampManager tm;
	Simulation_World(sf::Window * w);
	~Simulation_World();

	//TickServiceProvider
	virtual ICamera3D * getHolderCamera();//can return 0 if currently not held

	virtual ChunkManager * getChunkManager();//TODO remove

	ITerrain * getITerrain();
	IWorld * getIWorld();

	virtual Entity * getTarget(Entity * me);

	void spawn(Entity*, spacevec);
	void spawnGrounded(ModelEntity* ep, spacevec pos);

	void loadStandardTex();

	void trigger(bool pulled);

	void loop();

	//from DrawServiceProvider:
	virtual ITexture * suggestFont();//returns 0 if no suggestion is made
};

