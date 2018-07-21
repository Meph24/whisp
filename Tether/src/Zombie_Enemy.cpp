#include "Zombie_Enemy.h"
#include <cstdlib>
#include <GL/glew.h>


Zombie_Enemy::Zombie_Enemy(Timestamp spawnTime,ITexture * texture,spacevec startPos,ChunkManager * chm):
tex(texture),ml(4),cm(chm),legDmg(0),bodyAnim(1,0),fallAnim(0.25f,0,1),transitionAnim(0.5f,0,1)
{
	acceptedConversions=FLAG_HIT_TYPE_BULLET;
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
	pushRadius=0.3f*size;
	pushForce=speed/30;

	bb=AABB(pos);
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
	if(remainingHP<=0) fallAnim.update(tickOffset);


	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=cm->toMeters(interPos);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	glRotatef(facing, 0, 1, 0);
	glRotatef(fallAnim.getCurStep(0)*90, 1, 0, 0);
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
	if(remainingHP<=0) fallAnim.update(seconds);
	if(fallAnim.getCurStep(0)>=1) this->requestDestroy(tsp);
	ChunkManager * cm=tsp->getChunkManager();
	spacelen characterHeightConv=cm->fromMeters(size*2);

	spacevec old=pos;

	pos.x += cm->fromMeters((speed)*cos(facing *TAU/360)*seconds);//Zombie walk "AI"
	pos.z += cm->fromMeters(-(speed)*sin(facing*TAU/360)*seconds);
	pos=cm->clip(pos,true);

	spacevec newVec=pos;
	spacevec moved=(newVec-old);
	bool chunkBorder=(old.y==cm->fromMeters((defaultHeight*1.0f)))^(newVec.y==cm->fromMeters((defaultHeight*1.0f)));
	if(moved.fLengthSq(cm->getChunkSize())>0.0000000001f)
	{
		vec3 norm=cm->toMeters(moved);
		norm.normalize();
		flt speedModA=(vec3(norm.x,0,norm.z).length());
		vec3 flat=vec3(cm->toMeters(moved.x),0,cm->toMeters(moved.z));
		flt h=cm->toMeters(moved.y)/flat.length();
		SpeedMod sm=SpeedMod();
		flt speedModB=sm.slowdownFromTerrain(h);
		if(chunkBorder)
		{
			speedModA=1;
			speedModB=1;
		}
		old+=cm->fromMeters(flat*speedModA*speedModB);
		pos.x=old.x;
		pos.z=old.z;
		pos=cm->clip(pos,true);
		maxTransition=1-(h/1.5f);
		if(maxTransition>1.7f) maxTransition=1.7f;
		if(maxTransition<0) maxTransition=0;

	}

	vec3 relPos=cm->toMeters(pos-tsp->getTarget(this)->pos);
	float wishAngle=atan2(relPos.x, relPos.z);
	wishAngle *= 360 / TAU;
	wishAngle += 90;
	float dif = abs(wishAngle - facing);
	float difplus = abs(wishAngle - (facing+360));
	float difminus = abs(wishAngle - (facing-360));
	if (difplus < dif) facing += 360;
	else if (difminus<dif) facing -= 360;
	facing = facing *(1 - seconds) + seconds*wishAngle;

	spacevec sizeBB;

	if(legDmg>0.25f*totalHP)
	{
		sizeBB.x=characterHeightConv;
	}
	else
	{
		sizeBB.x=characterHeightConv*0.3f;
	}
	sizeBB.y=characterHeightConv*1.5f;
	sizeBB.z=sizeBB.x;


	bb=AABB(pos,sizeBB);
	registerPushCheck((Entity *)this,seconds,tsp);
	if(remainingHP>=0) registerHitCheck((Entity *)this,seconds,tsp);
}

#include "WarnErrReporter.h"
void Zombie_Enemy::checkProjectile(EntityProjectile * projectile,vec3 relPosMeters,ChunkManager* cm)
{
	float before=this->remainingHP;
	DualPointer<Projectile> projConv=DualPointer<Projectile>((Entity *)projectile,(Projectile *)projectile);

	spacevec middleChunk=cm->getMiddleChunk();
	ml.loadIdentity();
	ml.translatef(relPosMeters.x, relPosMeters.y, relPosMeters.z);
	ml.rotatef(facing, 0, 1, 0);
	ml.rotatef(fallAnim.getCurStep(0)*90, 1, 0, 0);
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
	float hitTime;
	loc = 1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 4, projectile);

	ml.popMatrix();


	ml.pushMatrix();	//leg
	loc = -1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 5, projectile);

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

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 0, projectile);


	ml.popMatrix();


	ml.pushMatrix();	//body
	ml.translatef(0, 0.6f, 0);
	ml.scalef(1, 3, 2);

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 1, projectile);

	ml.popMatrix();



	ml.pushMatrix();	//arm
	loc = 1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 16 + 90
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 2, projectile);

	ml.popMatrix();

	ml.pushMatrix();	//arm
	loc = -1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	gotHit(hitTime, 3, projectile);

	ml.popMatrix();

	ml.popMatrix();
	float after=this->remainingHP;
	if(before==after)
	{
		WarnErrReporter::noEffectErr("expected hp change when hit, but no hp change detected");
	}
	std::cout<<"Entity ptr="<<(Entity *)this<<"; remainingHP="<<remainingHP<<std::endl;

}



//TODO remove comment: old system
void Zombie_Enemy::checkHitboxes(Zombie_Physics * ph,spacevec middleChunk,ChunkManager * cm)
{
//	spacevec relPos=pos-middleChunk;
//	vec3 relPosMeters=cm->toMeters(relPos);
//
//	for (int i = 0; i < ph->pCount[0]; i++)
//	{
//		if (ph->projectiles[i])
//		{
//			if(ph->projectiles[i]->bb.doesIntersect(bb))
//			{
//				checkProjectile(ph->projectiles[i],relPosMeters,cm);
//			}
//		}
//	}
}

float Zombie_Enemy::checkBox(DualPointer<Projectile> projectile,MatrixLib2* ml, float xFrom, float xTo, float yFrom,float yTo, float zFrom, float zTo, spacevec relPos)
{
	float ret=-1;
	ml->pushMatrix();
	ml->translatef(xFrom, yFrom, zFrom);
	ml->scalef(xTo - xFrom, yTo - yFrom, zTo - zFrom);
	mat4 matOut;
	ml->invertMatrix(ml->curMatrix, matOut.mat);

	vec3 p1 = matOut* (cm->toMeters(projectile.pIF->posOld-relPos));
	vec3 p2 = matOut*(cm->toMeters(projectile.e->pos-relPos));
	//projectile now relative to cube, where cube is at 0-1 on all 3 axis
	float vx = p2.x - p1.x;//TODO check for 0
	float vy = p2.y - p1.y;
	float vz = p2.z - p1.z;
	float x0 = -p1.x / vx;
	float x1 = (1 - p1.x) / vx;
	if (x1 < x0)
	{
		float temp = x0;
		x0 = x1;
		x1 = temp;
	}
	float y0 = -p1.y / vy;
	float y1 = (1 - p1.y) / vy;
	if (y1 < y0)
	{
		float temp = y0;
		y0 = y1;
		y1 = temp;
	}
	float z0 = -p1.z / vz;
	float z1 = (1 - p1.z) / vz;
	if (z1 < z0)
	{
		float temp = z0;
		z0 = z1;
		z1 = temp;
	}
	float minC = x0 > y0 ? x0 : y0;
	minC = minC > z0 ? minC : z0;// max(x0, y0, z0);
	float maxC = x1 < y1 ? x1 : y1;
	maxC = maxC < z1 ? maxC : z1;// min(x1, y1, z1);
	if ((minC<1)&&(maxC>0)&&(minC < maxC))
	{
		ret = minC;
	}
	ml->popMatrix();
	return ret;
}

void Zombie_Enemy::gotHit(float time, int part, EntityProjectile * projectile)
{
	if(time==-1) return;//no hit
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
	legDmg+=legDmgMult*projectile->fromItem->damagePerJoule;//TODO replace with better logic once guns 2.0 work
	remainingHP -= projectile->fromItem->damagePerJoule*dmgMult;

	projectile->pos.y.intpart -= 1000;
	projectile->posOld.y.intpart -= 1000;
	if(legDmg>0.25f*totalHP)
	{
		speed/=2.5f;
	}
}

//TODO remove comment: new system
void Zombie_Enemy::testHit(std::vector<ProjectileCollision> * collisions,DualPointer<Projectile> projectile,ChunkManager * cm)
{
	spacevec middleChunk=cm->getMiddleChunk();
	spacevec relPos=pos-middleChunk;
	vec3 relPosMeters=cm->toMeters(relPos);
	ml.loadIdentity();
	ml.translatef(relPosMeters.x, relPosMeters.y, relPosMeters.z);
	ml.rotatef(facing, 0, 1, 0);
	ml.rotatef(fallAnim.getCurStep(0)*90, 1, 0, 0);
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
	float hitTime;
	loc = 1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}

	ml.popMatrix();


	ml.pushMatrix();	//leg
	loc = -1;
	ml.translatef(0, 0.6f, loc * 0.1f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}

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

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}


	ml.popMatrix();


	ml.pushMatrix();	//body
	ml.translatef(0, 0.6f, 0);
	ml.scalef(1, 3, 2);

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}

	ml.popMatrix();



	ml.pushMatrix();	//arm
	loc = 1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);//sin(loc*animStep * 2 *speed/size) * 16 + 90
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}

	ml.popMatrix();

	ml.pushMatrix();	//arm
	loc = -1;

	ml.translatef(0, 1.1f, loc * 0.3f);
	ml.rotatef(sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, 0, 0, 1);
	ml.translatef(0, -0.6f, 0);
	ml.scalef(1, 3, 1);

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,middleChunk);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,cm->activeChunk));
	}

	ml.popMatrix();

	ml.popMatrix();
}

void Zombie_Enemy::push(spacevec amount)
{
	pos+=amount;//TODO?
}

//spacevec Zombie_Enemy::getPos()
//{
//	return pos;
//}
//#include "Zombie_Physics.h"
//#include "EntityPlayer.h"
//void Zombie_Enemy::onAABBintersect(Entity* other,float time,TickServiceProvider * tsp)
//{
//	Pushable * push=other->toPushable();//dynamic_cast<Pushable *>(other);
//	if(push)
//	{
////		pushEntities(this,push,time,tsp->getChunkManager());
//	}
//}
//
//Pushable* Zombie_Enemy::toPushable()
//{
//	return this;
//}
