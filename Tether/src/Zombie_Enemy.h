
#ifndef SRC_ZOMBIE_ENEMY_H_
#define SRC_ZOMBIE_ENEMY_H_

#include "Entity.h"
#include "Pushable.h"
#include "Hittable.h"
#include "HittableBulletLike.h"

class ITexture;
class EntityProjectileBulletLike;

#include "CumulativeMat.hpp"
#include "AnimationCycle.h"

class Zombie_Enemy: public Entity,public Pushable,public Hittable, public HittableBulletLike
{
	bool headshot=true;
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

	CumulativeMat ml;

	float tilted=10;

	void drawHead();
	void drawBody();
	void drawArm(int loc);
	void drawLeg(int loc,float strength);
	void drawTexturedCube(texCooSet textureCoordinates);

	bool gotHit(float time, int part, EntityProjectileBulletLike * projectile);

public:

	static int zombieCount;


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


	//spawns a random zombie at the given location
	Zombie_Enemy(Timestamp spawnTime,ITexture * texture,spacevec startPos,ChunkManager * cm);
	~Zombie_Enemy();

	virtual void draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp);
	virtual void tick(Timestamp t,TickServiceProvider * tsp);

	void checkProjectile(EntityProjectileBulletLike * projectile,TickServiceProvider* tsp);
	float checkBox(DualPointer<Projectile> projectile,CumulativeMat * ml,float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo);//valid hit from 0 to 1, otherwise -1


	virtual void testHit(std::vector<ProjectileCollision> * collisions,hitType type,DualPointer<Projectile> projectile,ChunkManager * cm);

	virtual void push(spacevec amount, TickServiceProvider& tsp);

	virtual HittableBulletLike * asHittableBulletLike();

};

#endif /* SRC_ZOMBIE_ENEMY_H_ */
