
#ifndef SRC_ZOMBIE_ENEMY_H_
#define SRC_ZOMBIE_ENEMY_H_

#include "ITexture.h"
#include "Zombie_Physics.h"
#include "ChunkManager.h"
#include "Entity.h"
#include "AnimationCycle.h"
class Zombie_Enemy: public Entity
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

	AnimationCycle bodyAnim;
	AnimationCycle fallAnim;
	AnimationCycle transitionAnim;

	float maxTransition=1;

	float dead;

	Zombie_Enemy(ITexture * texture,spacevec startPos,ChunkManager * cm);
	~Zombie_Enemy();

	virtual void draw(float tickOffset,spacevec observerPos,ChunkManager * cm,DrawServiceProvider * dsp);
	virtual void tick(float seconds,TickServiceProvider * tsp);

	void checkHitboxes(Zombie_Physics * ph,spacevec middleChunk,ChunkManager * cm);
};

#endif /* SRC_ZOMBIE_ENEMY_H_ */
