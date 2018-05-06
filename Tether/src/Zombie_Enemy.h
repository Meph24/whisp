
#ifndef SRC_ZOMBIE_ENEMY_H_
#define SRC_ZOMBIE_ENEMY_H_

#include "ITexture.h"
#include "Zombie_Physics.h"
#include "ChunkManager.h"
#include "Entity.h"
#include "Pushable.h"
#include "AnimationCycle.h"
class Zombie_Enemy: public Entity, Pushable
{

	struct texCooSet{
		float unitSize;
		float startX, startY;
		float mainHeight;
		float frontWidth;
		float sideWidth;
		float topX, topY;
		float bottomX, bottomY;
	};

	ITexture * tex;

	MatrixLib2 ml;

	float tilted=10;

	void drawHead();
	void drawBody();
	void drawArm(int loc);
	void drawLeg(int loc,float strength);
	void drawTexturedCube(texCooSet textureCoordinates);

	void gotHit(Zombie_Physics::hit hit, int part, EntityProjectile ** shots);

public:
	ChunkManager * cm;
	float facing;
	float speed;
	float size;

	float totalHP;
	float remainingHP;
	float legDmg;
	float maxHP;

	AnimationCycle bodyAnim;
	AnimationCycle fallAnim;
	AnimationCycle transitionAnim;

	float maxTransition=1;

	float dead;

	//spawns a random zombie at the given location
	Zombie_Enemy(Timestamp spawnTime,ITexture * texture,spacevec startPos,ChunkManager * cm);
	~Zombie_Enemy();

	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);
	virtual void tick(Timestamp t,TickServiceProvider * tsp);

	void checkHitboxes(Zombie_Physics * ph,spacevec middleChunk,ChunkManager * cm);


	virtual void push(spacevec amount);
	virtual spacevec getPos();
	virtual void onAABBintersect(Entity * other,float time,TickServiceProvider * tsp);
};

#endif /* SRC_ZOMBIE_ENEMY_H_ */
