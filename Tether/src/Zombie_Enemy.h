
#ifndef SRC_ZOMBIE_ENEMY_H_
#define SRC_ZOMBIE_ENEMY_H_

#include "ITexture.h"
#include "Zombie_Physics.h"
#include "ChunkManager.h"
class Zombie_Enemy
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

	void gotHit(Zombie_Physics::hit hit, int part, Zombie_Projectile ** shots);

public:
	float posX,posY, posZ;
	ChunkManager * cm;
	float facing;
	float speed;
	float size;

	float totalHP;
	float remainingHP;
	float legDmg;

	float animStep;
	float transition=0;
	float maxTransition=1;

	float dead;

	Zombie_Enemy(ITexture * texture,float startX,float startZ,ChunkManager * cm);
	~Zombie_Enemy();

	void draw(float seconds);

	void checkHitboxes(Zombie_Physics * ph);
};

#endif /* SRC_ZOMBIE_ENEMY_H_ */
