#include "Zombie_Enemy.h"
#include <cstdlib>
#include <GL/glew.h>


Zombie_Enemy::Zombie_Enemy(Timestamp spawnTime,ITexture * texture,spacevec startPos,ChunkManager * chm):
tex(texture),ml(4),cm(chm),legDmg(0),bodyAnim(1,0),fallAnim(0.5f,0,1),transitionAnim(0.5f,0,1),dead(0)
{
	lastTick=spawnTime;
	pos=cm->clip(startPos,true);
	v=cm->fromMeters({0,0,0});
	facing = std::rand() % 360;
	speed = 2 +(std::rand() % 5000)/1000;
	size = 0.6f + (std::rand() % 1024) / 1024.0f;
	speed *= size;
	remainingHP = 100 * size;
	if (rand() % 64 == 0)
	{
		speed *= 2;
		size *= 5;
		remainingHP *= 40;
	}
	totalHP=remainingHP;

	bodyAnim=AnimationCycle(3*size/speed,0);
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
	glVertex3f(0.1, 0.2f, 0.1);

	glTexCoord2f(tc.startX + tc.sideWidth+tc.frontWidth, tc.startY);
	glVertex3f(0.1, 0.2f, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(0.1, 0, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth, tc.startY + tc.mainHeight);
	glVertex3f(0.1, 0, 0.1);

	glTexCoord2f(tc.startX+tc.sideWidth, tc.startY);//left //0
	glVertex3f(0.1, 0.2f, 0.1);

	glTexCoord2f(tc.startX, tc.startY);
	glVertex3f(-0.1, 0.2f, 0.1);

	glTexCoord2f(tc.startX, tc.startY + tc.mainHeight);
	glVertex3f(-0.1, 0, 0.1);

	glTexCoord2f(tc.startX + tc.sideWidth, tc.startY + tc.mainHeight);
	glVertex3f(0.1, 0, 0.1);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY);//right
	glVertex3f(-0.1, 0.2f, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY);
	glVertex3f(0.1, 0.2f, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(0.1, 0, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth*2 + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(-0.1, 0, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY);//back
	glVertex3f(-0.1, 0.2f, -0.1);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth * 2, tc.startY);
	glVertex3f(-0.1, 0.2f, 0.1);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth * 2, tc.startY + tc.mainHeight);
	glVertex3f(-0.1, 0, 0.1);

	glTexCoord2f(tc.startX + tc.sideWidth * 2 + tc.frontWidth, tc.startY + tc.mainHeight);
	glVertex3f(-0.1, 0, -0.1);

	glTexCoord2f(tc.topX, tc.topY + tc.sideWidth);//top
	glVertex3f(0.1, 0.2f, 0.1);

	glTexCoord2f(tc.topX+tc.frontWidth, tc.topY + tc.sideWidth);
	glVertex3f(0.1, 0.2f, -0.1);

	glTexCoord2f(tc.topX + tc.frontWidth, tc.topY);
	glVertex3f(-0.1, 0.2f, -0.1);

	glTexCoord2f(tc.topX, tc.topY);
	glVertex3f(-0.1, 0.2f, 0.1);

	glTexCoord2f(tc.bottomX, tc.bottomY + tc.sideWidth);//bottom
	glVertex3f(0.1, 0, 0.1);

	glTexCoord2f(tc.bottomX + tc.frontWidth, tc.bottomY + tc.sideWidth);
	glVertex3f(0.1, 0, -0.1);

	glTexCoord2f(tc.bottomX + tc.frontWidth, tc.bottomY);
	glVertex3f(-0.1, 0, -0.1);

	glTexCoord2f(tc.bottomX, tc.bottomY);
	glVertex3f(-0.1, 0, 0.1);

	glEnd();
}

void Zombie_Enemy::drawHead()
{
	glPushMatrix();

	glTranslatef(0, 1.2f, 0);
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
	glTranslatef(0, 0.6f, 0);
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
	glTranslatef(0, 1.1f, loc *0.3f);
	glRotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 16 + 90
	glTranslatef(0, -0.6f, 0);
	glScalef(1, 3, 1);
	drawTexturedCube(tc);
	glPopMatrix();
}
void Zombie_Enemy::drawLeg(int loc,float strength)
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
	glTranslatef(0, 0.6f, loc * 0.1f);
	glRotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*strength, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 30*strength
	glTranslatef(0, -0.6f, 0);
	glScalef(1, 3, 1);
	drawTexturedCube(tc);
	glPopMatrix();
}
void Zombie_Enemy::draw(Timestamp t,Frustum * viewFrustum,ChunkManager * cm,DrawServiceProvider * dsp)
{
	float tickOffset=t-lastTick;
	if(!exists) return;//TODO this kind of check should be done by the caller beforehand
	if(!viewFrustum->inside(bb)) return;

	bodyAnim.updateTemp(tickOffset);
	if(legDmg>0.25f*totalHP)
		transitionAnim.update(tickOffset);
	if(dead) fallAnim.update(tickOffset);


	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=cm->toMeters(interPos);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	glRotatef(facing, 0, 1, 0);
	glRotatef(dead, 1, 0, 0);
	glScalef(size, size, size);
	tex->bind();
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);


	glPushMatrix();
	float animStrength=1;
	if(legDmg>0.25f*totalHP)
	{
		glTranslatef(-0.25f*transitionAnim.getCurStep(0)*maxTransition,0.1f*transitionAnim.getCurStep(0)*maxTransition,0);
		glRotatef(-90*transitionAnim.getCurStep(0)*maxTransition, 0, 0, 1);
		animStrength=0.15f*transitionAnim.getCurStep(0)*maxTransition;
	}
	drawLeg(1,animStrength);
	drawLeg(-1,animStrength);

	glPopMatrix();

	if(legDmg>0.25f*totalHP)
	{
		glTranslatef(0,-0.4f*transitionAnim.getCurStep(0)*maxTransition,0);
		glRotatef(-35*transitionAnim.getCurStep(0)*maxTransition, 0, 0, 1);
	}
	drawBody();

	drawHead();

	drawArm(1);
	drawArm(-1);


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Zombie_Enemy::tick(Timestamp t,TickServiceProvider * tsp)
{
	float seconds=t-lastTick;
	lastTick=t;
	bodyAnim.update(seconds);
	if(legDmg>0.25f*totalHP)
		transitionAnim.update(seconds);
	if(dead) fallAnim.update(seconds);
	spacelen characterHeightConv=tsp->getChunkManager()->fromMeters(size*2);
	spacevec sizeBB;
	sizeBB.x=characterHeightConv;
	sizeBB.y=characterHeightConv*1.5f;
	sizeBB.z=sizeBB.x;
	bb=AABB(pos,sizeBB);
}

void Zombie_Enemy::checkHitboxes(Zombie_Physics * ph,spacevec middleChunk,ChunkManager * cm)
{
	spacevec relPos=pos-middleChunk;
	vec3 relPosMeters=cm->toMeters(relPos);
	bool proj=false;
	for (int i = 0; i < ph->pCount[0]; i++)
	{
		if (ph->projectiles[i])
		{
			if(!ph->projectiles[i]->bb.doesIntersect(bb)) continue;
			proj=true;
		}
	}
	if(!proj) return;
	std::cout<<"phase 2"<<std::endl;
	ml.loadIdentity();
	ml.translatef(relPosMeters.x, relPosMeters.y, relPosMeters.z);
	ml.rotatef(facing, 0, 1, 0);
	ml.scalef(size, size, size);




	ml.pushMatrix();
	float animStrength=1;
	if(legDmg>0.25f*totalHP)
	{
		ml.translatef(-0.25f*transitionAnim.getCurStep(0)*maxTransition,0.1f*transitionAnim.getCurStep(0)*maxTransition,0);
		ml.rotatef(-90*transitionAnim.getCurStep(0)*maxTransition, 0, 0, 1);
		animStrength=0.15f*transitionAnim.getCurStep(0)*maxTransition;
	}

	ml.pushMatrix();//leg
	int loc;
	Zombie_Physics::hit h;
	loc = 1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if (h.projectileIndex != -1) gotHit(h, 4, ph->projectiles);

	ml.popMatrix();


	ml.pushMatrix();	//leg
	loc = -1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if (h.projectileIndex != -1) gotHit(h, 5, ph->projectiles);

	ml.popMatrix();
	ml.popMatrix();


	ml.pushMatrix();

	if(legDmg>0.25f*totalHP)
	{
		ml.translatef(0,-0.4f*transitionAnim.getCurStep(0)*maxTransition,0);
		ml.rotatef(-35*transitionAnim.getCurStep(0)*maxTransition, 0, 0, 1);
	}

	ml.pushMatrix();	//head

	ml.translatef(0, 1.2f, 0);
	ml.scalef(2, 2, 2);
	ml.rotatef(tilted, 1, 0, 0);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);

	if (h.projectileIndex != -1) gotHit(h, 0,ph->projectiles);


	ml.popMatrix();


	ml.pushMatrix();	//body
	ml.translatef(0, 0.6f, 0);
	ml.scalef(1, 3, 2);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if (h.projectileIndex != -1) gotHit(h, 1, ph->projectiles);

	ml.popMatrix();



	ml.pushMatrix();	//arm
	loc = 1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 16 + 90
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if (h.projectileIndex != -1) gotHit(h, 2, ph->projectiles);

	ml.popMatrix();

	ml.pushMatrix();	//arm
	loc = -1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	h = ph->testHitbox(&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if (h.projectileIndex != -1) gotHit(h, 3, ph->projectiles);

	ml.popMatrix();

	ml.popMatrix();


}

void Zombie_Enemy::gotHit(Zombie_Physics::hit hit, int part,EntityProjectile ** shots)
{
	if (hit.projectileIndex == -1) return;
	float dmgMult = 0;
	//float armDmgMult = 0;
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
		//armDmgMult = 1;
		break;
	case 3:
		dmgMult = 0.45f;
		//armDmgMult = 1;
		break;
	case 4:
		dmgMult = 0.55f;
		legDmgMult = 0.55f;
		break;
	case 5:
		dmgMult = 0.55f;
		legDmgMult = 0.55f;
		break;
	}
	legDmg+=legDmgMult*shots[hit.projectileIndex]->fromItem->damagePerJoule;//TODO replace with better logic once guns 2.0 work
	remainingHP -= shots[hit.projectileIndex]->fromItem->damagePerJoule*dmgMult;
	if(remainingHP<0)
	{
		remainingHP=-totalHP*dmgMult;//TODO
	}
	shots[hit.projectileIndex]->pos.y.intpart -= 1000;
	shots[hit.projectileIndex]->posOld.y.intpart -= 1000;
	if((legDmg>0.25f*totalHP)&&(tilted!=-10))
	{
		tilted=-10;
		speed/=2.5f;
	}
}
