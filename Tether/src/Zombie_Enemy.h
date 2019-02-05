
#ifndef SRC_ZOMBIE_ENEMY_H_
#define SRC_ZOMBIE_ENEMY_H_

#include "SpeedMod.h"
#include "ITexture.h"
#include "ChunkManager.h"
#include "Entity.h"
#include "Pushable.h"
#include "AnimationCycle.h"
#include "Hittable.h"
#include "EntityProjectile.h"

class Zombie_Enemy: public Entity,public Pushable,public Hittable
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

	MatrixLib2 ml;

	float tilted=10;

	void drawHead();
	void drawBody();
	void drawArm(int loc);
	void drawLeg(int loc,float strength);
	void drawTexturedCube(texCooSet textureCoordinates);

	bool gotHit(float time, int part, EntityProjectile * projectile);

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

	void checkProjectile(EntityProjectile * projectile,vec3 relPosMeters,TickServiceProvider* tsp);
	float checkBox(DualPointer<Projectile> projectile,MatrixLib2 * ml,float xFrom, float xTo, float yFrom, float yTo, float zFrom, float zTo,spacevec relPos);//valid hit from 0 to 1, otherwise -1


	virtual void testHit(std::vector<ProjectileCollision> * collisions,hitType type,DualPointer<Projectile> projectile,ChunkManager * cm);

	virtual void push(spacevec amount);
//	virtual spacevec getPos();
//	virtual void onAABBintersect(Entity * other,float time,TickServiceProvider * tsp);
//
//	virtual Pushable * toPushable();
};

#endif /* SRC_ZOMBIE_ENEMY_H_ */
