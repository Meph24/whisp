#include "Zombie_Enemy.h"
#include <cstdlib>
#include <GL/glew.h>


Zombie_Enemy::Zombie_Enemy(ITexture * texture, float startX, float startZ):
tex(texture),ml(4), posX(startX), posZ(startZ),animStep(0),dead(0)//spawns a random zombie at the given location
{
	facing = std::rand() % 360;
	speed = 2000 +std::rand() % 5000;
	size = 0.6f + (std::rand() % 1024) / 1024.0f;
	speed *= size;
	remainingHP = 100 * size;
	if (rand() % 64 == 0)
	{
		speed *= 2;
		size *= 5;
		remainingHP *= 20;
	}
}


Zombie_Enemy::~Zombie_Enemy()
{

}
void Zombie_Enemy::drawTexturedCube(texCooSet tc)
{
	tc.startX *= tc.unitSize;
	tc.startY *= tc.unitSize;
	tc.bottomX *= tc.unitSize;
	tc.bottomY *= tc.unitSize;
	tc.frontWidth *= tc.unitSize;
	tc.mainHeight *= tc.unitSize;
	tc.sideWidth *= tc.unitSize;
	tc.topX *= tc.unitSize;
	tc.topY *= tc.unitSize;

	glBegin(GL_QUADS);

	glTexCoord2f(tc.startX + tc.sideWidth, tc.startY);//front
	glVertex3f(100, 200, 100);

	glTexCoord2f(tc.startX + tc.sideWidth+tc.frontWidth, tc.startY);
	glVertex3f(100, 200, -100);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(100, 0, -100);

	glTexCoord2f(tc.startX + tc.sideWidth, tc.startY + tc.mainHeight);
	glVertex3f(100, 0, 100);

	glTexCoord2f(tc.startX+tc.sideWidth, tc.startY);//left
	glVertex3f(100, 200, 100);

	glTexCoord2f(tc.startX, tc.startY);
	glVertex3f(-100, 200, 100);

	glTexCoord2f(tc.startX, tc.startY + tc.mainHeight);
	glVertex3f(-100, 0, 100);

	glTexCoord2f(tc.startX + tc.sideWidth, tc.startY + tc.mainHeight);
	glVertex3f(100, 0, 100);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY);//right
	glVertex3f(-100, 200, -100);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY);
	glVertex3f(100, 200, -100);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(100, 0, -100);

	glTexCoord2f(tc.startX + tc.sideWidth*2 + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(-100, 0, -100);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY);//back
	glVertex3f(-100, 200, -100);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth * 2, tc.startY);
	glVertex3f(-100, 200, 100);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth * 2, tc.startY + tc.mainHeight);
	glVertex3f(-100, 0, 100);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(-100, 0, -100);

	glTexCoord2f(tc.topX, tc.topY + tc.sideWidth);//top
	glVertex3f(100, 200, 100);

	glTexCoord2f(tc.topX+tc.frontWidth, tc.topY + tc.sideWidth);
	glVertex3f(100, 200, -100);

	glTexCoord2f(tc.topX + tc.frontWidth, tc.topY);
	glVertex3f(-100, 200, -100);

	glTexCoord2f(tc.topX, tc.topY);
	glVertex3f(-100, 200, 100);


	glTexCoord2f(tc.bottomX, tc.bottomY + tc.sideWidth);//bottom
	glVertex3f(100, 0, 100);

	glTexCoord2f(tc.bottomX + tc.frontWidth, tc.bottomY + tc.sideWidth);
	glVertex3f(100, 0, -100);

	glTexCoord2f(tc.bottomX + tc.frontWidth, tc.bottomY);
	glVertex3f(-100, 0, -100);

	glTexCoord2f(tc.bottomX, tc.bottomY);
	glVertex3f(-100, 0, 100);

	glEnd();
}

void Zombie_Enemy::drawHead()
{
	glPushMatrix();

	glTranslatef(0, 1200, 0);
	glScalef(2, 2, 2);
	glRotatef(tilted, 1, 0, 0);


	texCooSet tc;
	tc.unitSize = 1 / 16.0f;
	tc.startX = 0;
	tc.startY = 2;
	tc.mainHeight = 2;
	tc.frontWidth = 2;
	tc.sideWidth = 2;
	tc.topX = 2;
	tc.topY = 0;
	tc.bottomX = 4;
	tc.bottomY = 0;

	drawTexturedCube(tc);
	glPopMatrix();
}

void Zombie_Enemy::drawBody()
{
	texCooSet tc;
	tc.unitSize = 1 / 16.0f;
	tc.startX = 4;
	tc.startY = 5;
	tc.mainHeight = 3;
	tc.frontWidth = 2;
	tc.sideWidth = 1;
	tc.topX = 5;
	tc.topY = 4;
	tc.bottomX = 7;
	tc.bottomY = 4;


	glPushMatrix();
	glTranslatef(0, 600, 0);
	glScalef(1, 3, 2);
	drawTexturedCube(tc);
	glPopMatrix();
}
#include <cmath>
void Zombie_Enemy::drawArm(int loc)
{
	texCooSet tc;
	tc.unitSize = 1 / 16.0f;
	tc.startX = 10;
	tc.startY = 5;
	tc.mainHeight = 3;
	tc.frontWidth = 1;
	tc.sideWidth = 1;
	tc.topX = 11;
	tc.topY = 4;
	tc.bottomX = 12;
	tc.bottomY = 4;


	glPushMatrix();
	glTranslatef(0, 1100, loc * 300);
	glRotatef(sin(loc*animStep * 2 / 1000.0f*speed/size) * 16 + 90, 0, 0, 1);
	glTranslatef(0, -600, 0);
	glScalef(1, 3, 1);
	drawTexturedCube(tc);
	glPopMatrix();
}
void Zombie_Enemy::drawLeg(int loc)
{
	texCooSet tc;
	tc.unitSize = 1 / 16.0f;
	tc.startX = 0;
	tc.startY = 5;
	tc.mainHeight = 3;
	tc.frontWidth = 1;
	tc.sideWidth = 1;
	tc.topX = 1;
	tc.topY = 4;
	tc.bottomX = 2;
	tc.bottomY = 4;


	glPushMatrix();
	glTranslatef(0, 600, loc * 100);
	glRotatef(sin(loc*animStep * 2 / 1000.0f*speed/size) * 30, 0, 0, 1);
	glTranslatef(0, -600, 0);
	glScalef(1, 3, 1);
	drawTexturedCube(tc);
	glPopMatrix();
}
void Zombie_Enemy::draw(float seconds)
{
	animStep += seconds;
	glPushMatrix();
	glTranslatef(posX, 0, posZ);
	glRotatef(facing, 0, 1, 0);
	glRotatef(dead, 1, 0, 0);
	glScalef(size, size, size);
	tex->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);

	//tilted = 10;
	drawHead();

	drawBody();

	drawArm(1);
	drawArm(-1);
	drawLeg(1);
	drawLeg(-1);


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Zombie_Enemy::checkHitboxes(Zombie_Physics * ph)
{

	bool proj=false;
	for (int i = 0; i < ph->pCount[0]; i++)
	{
		if (ph->projectiles[i])
		{

			vec3 max=ph->projectiles[i]->posOld;
			vec3 min=max;
			vec3 next=ph->projectiles[i]->posOld;
			if(min.x>next.x) min.x=next.x;
			else if(max.x<next.x) max.x=next.x;
			if(min.y>next.y) min.y=next.y;
			else if(max.y<next.y) max.y=next.y;
			if(min.z>next.z) min.z=next.z;
			else if(max.z<next.z) max.z=next.z;
			flt maxSize=4000*size;
			max+={maxSize,maxSize,maxSize};
			min-={maxSize,maxSize,maxSize};
			if(min.x>posX) continue;
			if(min.y>0) continue;
			if(min.z>posZ) continue;//lol optimization at tier 1
			if(max.x<posX) continue;
			if(max.y<0) continue;
			if(max.z<posZ) continue;
			proj=true;
		}
	}
	if(!proj) return;
	ml.loadIdentity();
	ml.translatef(posX, 0, posZ);
	ml.rotatef(facing, 0, 1, 0);
	ml.scalef(size, size, size);





	ml.pushMatrix();

	ml.translatef(0, 1200, 0);
	ml.scalef(2, 2, 2);
	ml.rotatef(tilted, 1, 0, 0);

	Zombie_Physics::hit h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);

	if (h.projectileIndex != -1) gotHit(h, 0,ph->projectiles);


	ml.popMatrix();


	ml.pushMatrix();
	ml.translatef(0, 600, 0);
	ml.scalef(1, 3, 2);

	h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);
	if (h.projectileIndex != -1) gotHit(h, 1, ph->projectiles);

	ml.popMatrix();



	ml.pushMatrix();
	int loc = 1;

	ml.translatef(0, 1100, loc * 300);
	ml.rotatef(sin(loc*animStep * 2 / 1000.0f*speed) * 16 + 90, 0, 0, 1);
	ml.translatef(0, -600, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);
	if (h.projectileIndex != -1) gotHit(h, 2, ph->projectiles);

	ml.popMatrix();

	ml.pushMatrix();
	loc = -1;

	ml.translatef(0, 1100, loc * 300);
	ml.rotatef(sin(loc*animStep * 2 / 1000.0f*speed) * 16 + 90, 0, 0, 1);
	ml.translatef(0, -600, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);
	if (h.projectileIndex != -1) gotHit(h, 3, ph->projectiles);

	ml.popMatrix();


	ml.pushMatrix();
	loc = 1;
	ml.translatef(0, 600, loc * 100);
	ml.rotatef(sin(loc*animStep * 2 / 1000.0f*speed) * 30, 0, 0, 1);
	ml.translatef(0, -600, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);
	if (h.projectileIndex != -1) gotHit(h, 4, ph->projectiles);

	ml.popMatrix();


	ml.pushMatrix();
	loc = -1;
	ml.translatef(0, 600, loc * 100);
	ml.rotatef(sin(loc*animStep * 2 / 1000.0f*speed) * 30, 0, 0, 1);
	ml.translatef(0, -600, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-100, 100, 0, 200, -100, 100);
	if (h.projectileIndex != -1) gotHit(h, 5, ph->projectiles);

	ml.popMatrix();
}

void Zombie_Enemy::gotHit(Zombie_Physics::hit hit, int part,Zombie_Projectile ** shots)
{
	if (hit.projectileIndex == -1) return;
	float dmgMult = 0;
	float armDmgMult = 0;
	float legDmgMult = 0;
	switch (part)
	{
	case 0:
		dmgMult=4;
		break;
	case 1:
		dmgMult = 1;
		break;
	case 2:
		dmgMult = 0.45f;
		armDmgMult = 1;
		break;
	case 3:
		dmgMult = 0.45f;
		armDmgMult = 1;
		break;
	case 4:
		dmgMult = 0.55f;
		legDmgMult = 1;
		break;
	case 5:
		dmgMult = 0.55f;
		legDmgMult = 1;
		break;
	}
	remainingHP -= shots[hit.projectileIndex]->firedFrom->baseDmg*dmgMult;
	shots[hit.projectileIndex]->pos.y -= 100000;
	shots[hit.projectileIndex]->posOld.y -= 100000;
}
