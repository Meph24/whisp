#include "Zombie_Enemy.h"

#include <cstdlib>
#include <GL/glew.h>

#include "Chunk.h"
#include "ChunkManager.h"
#include "Perspective.hpp"
#include "EntityProjectileBulletLike.h"
#include "EntitySound.h"
#include "FloatSeconds.hpp"
#include "Frustum.h"
#include "glmutils.hpp"
#include "InteractFilterAlgoSym.h"
#include "InteractFilterAlgoAsym.h"
#include "ITexture.h"
#include "Projectile.h"
#include "SoundManager.h"
#include "SpeedMod.h"
#include "TickServiceProvider.h"

int Zombie_Enemy::zombieCount=0;

Zombie_Enemy::Zombie_Enemy(const SimClock::time_point& spawn_time,spacevec startPos,TickServiceProvider* tsp)
	: ml()
	, legDmg(0)
	, bodyAnim(1,0)
	, fallAnim(0.25f,0,1)
	, transitionAnim(0.5f,0,1)
{
	IWorld * iw=&tsp->world();
	ITerrain * it=tsp->getITerrain();
	fac=FACTION_ZOMBIES;
	acceptedConversions=FLAG_HIT_TYPE_BULLET_LIKE;
	last_ticked = spawn_time;
	pos=it->clip(startPos,true);
	v=iw->fromMeters({0,0,0});
	facing = std::rand() % 360;
	speed = 2 +(std::rand() % 5000)/1000;
	size = 0.6f + (std::rand() % 1024) / 1024.0f;
	speed *= size;
	remainingHP = 100 * size;
	if (rand() % 640==0)//640 == 0)//64
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
	zombieCount++;
}


Zombie_Enemy::~Zombie_Enemy()
{
	zombieCount--;
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
#include <iostream>
void Zombie_Enemy::draw(const SimClock::time_point& draw_time ,Frustum * viewFrustum,IWorld& iw,Perspective& perspective)
{
	float tickOffset=(float) FloatSeconds(draw_time-last_ticked);
	if(!viewFrustum->inside(bb,iw))
	{
//		std::cout<<"zombie outside view"<<std::endl;//use this to test Frustum Culling
		return;
	}

	bodyAnim.updateTemp(tickOffset);
	if(legDmg>0.25f*totalHP)
		transitionAnim.updateTemp(tickOffset);
	if(remainingHP<=0) fallAnim.updateTemp(tickOffset);


	spacevec interPos=pos+v*tickOffset-viewFrustum->observerPos;
	vec3 interPosMeters=iw.toMeters(interPos);

	glPushMatrix();
	glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
	glRotatef(facing, 0, 1, 0);
	glRotatef(fallAnim.getCurStep(0)*90, 1, 0, 0);
	glScalef(size, size, size);
	perspective.graphics_ressources.zombieTex->bind();
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

void Zombie_Enemy::tick(const SimClock::time_point& next_tick_time, TickServiceProvider * tsp)
{
	IWorld * iw=&tsp->world();
	ITerrain * it=tsp->getITerrain();

	headshot=true;
	float seconds=(float) FloatSeconds(next_tick_time-last_ticked);
	last_ticked = next_tick_time;
	bodyAnim.update(seconds);
	if(legDmg>0.25f*totalHP)
		transitionAnim.update(seconds);
	if(remainingHP<=0) fallAnim.update(seconds);
	if(fallAnim.getCurStep(0)>=1)
	{
		exists=false;//TODO convert to new entity management
		requestDestroy(&tsp->world());
	}

	spacelen characterHeightConv=iw->fromMeters(size*2);

	spacevec old=pos;
	spacevec prev=pos;

	pos.x += iw->fromMeters((speed)*cos(facing *TAU/360)*seconds);//Zombie walk "AI"
	pos.z += iw->fromMeters(-(speed)*sin(facing*TAU/360)*seconds);
	pos=it->clip(pos,true);

	spacevec newVec=pos;
	spacevec moved=(newVec-old);
	bool chunkBorder=(old.y==iw->fromMeters((defaultHeight*1.0f)))^(newVec.y==iw->fromMeters((defaultHeight*1.0f)));
	if(!moved.equalsZero())
	{
		vec3 norm=iw->toMeters(moved);
		norm=glm::normalize(norm);
		float speedModA=glm::length(vec3(norm.x,0,norm.z));
		vec3 flat=vec3(iw->toMeters(moved.x),0,iw->toMeters(moved.z));
		float h=iw->toMeters(moved.y)/glm::length(flat);
		SpeedMod sm=SpeedMod();
		float speedModB=sm.slowdownFromTerrain(h);
		if(chunkBorder)
		{
			speedModA=1;
			speedModB=1;
		}
		old+=iw->fromMeters(flat*speedModA*speedModB);
		pos.x=old.x;
		pos.z=old.z;
		pos=it->clip(pos,true);
		maxTransition=1-(h/1.5f);
		if(maxTransition>1.7f) maxTransition=1.7f;
		if(maxTransition<0) maxTransition=0;

	}

	Entity* target = tsp->getTarget(this);
	if(!target)
	{
		v.set0();
	}
	else
	{	
		vec3 relPos=iw->toMeters(pos - target->pos);

		vec3 mySpeed=iw->toMeters(v);
		vec3 xprod=glm::cross(mySpeed,relPos);
	//	float mySpdSq=mySpeed.lengthSq();
	//	if(mySpdSq==0) mySpdSq=1;
	//	facing-=seconds*xprod.y/mySpdSq*20;
		if(xprod.y>0)
		{
			facing-=seconds*60;
		}
		else
		{
			facing+=seconds*60;
		}
	}

//	float wishAngle=atan2(relPos.x, relPos.z);
//	wishAngle *= 360 / TAU;
//	wishAngle += 90;
//	float dif = abs(wishAngle - facing);
//	float difplus = abs(wishAngle - (facing+360));
//	float difminus = abs(wishAngle - (facing-360));
//	if (difplus < dif) facing += 360;
//	else if (difminus<dif) facing -= 360;
//	facing = facing *(1 - seconds) + seconds*wishAngle;

	spacevec sizeBB;

	if(legDmg>0.25f*totalHP)
	{
		sizeBB.x=characterHeightConv*0.8f;
	}
	else
	{
		sizeBB.x=characterHeightConv*0.4f;
	}
	sizeBB.y=characterHeightConv*0.9f;
	sizeBB.z=sizeBB.x;
	if(seconds)
		v=(pos-prev)/seconds;
	//std::cout<<pos<<"|"<<prev<<"|"<<seconds<<"|"<<v<<std::endl;
	bb=AABB(pos,sizeBB);
	iw->pushAlgo->doChecks((Pushable *)this,(Entity *)this,seconds,*tsp);
	if(remainingHP>=0) iw->projectileAlgo->doChecks((Hittable *) this,(Entity *)this,seconds,*tsp);
}

#include "WarnErrReporter.h"
void Zombie_Enemy::checkProjectile(EntityProjectileBulletLike * projectile,TickServiceProvider& tsp)
{
	IWorld * iw = &tsp.world();
//	std::cout<<"inside checkProjectile"<<std::endl;
	float before=this->remainingHP;
	DualPointer<Projectile> projConv=DualPointer<Projectile>((Entity *)projectile,(Projectile *)projectile);

	ml.loadIdentity();
	ml = ml * glm::rotateDeg(facing, vec3(0.0f, 1.0f, 0.0f))
		    * glm::rotateDeg(	
							fallAnim.getCurStep(0)*90, 
							glm::vec3(1.0f, 0.0f, 0.0f)
						 )		 
			* glm::scale(vec3(size, size, size));


	ml.push();
	float animStrength=1;
	if(legDmg>0.25f*totalHP)
	{
		ml = glm::translate(ml, 
							vec3( 
								-0.25f*transitionAnim.getCurStep(0)*maxTransition,
								0.1f*transitionAnim.getCurStep(0)*maxTransition,
								0.0f
								));
		ml = glm::rotateDeg(ml, -90*transitionAnim.getCurStep(0)*maxTransition, vec3(0, 0, 1));
		animStrength=0.15f*transitionAnim.getCurStep(0)*maxTransition;
	}

	ml.push();//leg
	int loc;
	float hitTime;
	loc = 1;
	ml = glm::translate(ml, vec3(0, 0.6f, loc * 0.1f));
	ml = glm::rotateDeg(ml, (float)sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, vec3(0, 0, 1));//sin(loc*animStep * 2 *speed/size) * 30*animStrength, 0, 0, 1);
	ml = glm::translate(ml, vec3(0, -0.6f, 0));
	ml = glm::scale(ml, vec3(1, 3, 1));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if(gotHit(hitTime, 4, projectile))
	{
		EntitySound * es=new EntitySound(this,tsp.getSoundManager()->getCrippleSound());
		iw->requestEntitySpawn(es);
	}

	ml.pop();


	ml.push();	//leg
	loc = -1;
	ml = ml
		* glm::translate(vec3(0, 0.6f, loc * 0.1f))
		* glm::rotateDeg((float) sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength, vec3(0, 0, 1))
		* glm::translate(vec3(0, -0.6f, 0))
		* glm::scale(vec3(1, 3, 1));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if(gotHit(hitTime, 5, projectile))
	{
		EntitySound * es=new EntitySound(this,tsp.getSoundManager()->getCrippleSound());
		iw->requestEntitySpawn(es);
	}

	ml.pop();
	ml.pop();


	ml.push();

	if(legDmg>0.25f*totalHP)
	{
		ml = ml
			* glm::translate(vec3(0,-0.4f*transitionAnim.getCurStep(0)*maxTransition,0))
			* glm::rotateDeg((float)-35*transitionAnim.getCurStep(0)*maxTransition, vec3(0, 0, 1));
	}

	ml.push();	//head

	ml = ml 
		* glm::translate(vec3(0, 1.2f, 0))
		* glm::scale(vec3(2, 2, 2))
		* glm::rotateDeg(tilted,vec3( 1, 0, 0));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);

	if(headshot&&(hitTime!=-1))
	{
		EntitySound * es=new EntitySound(this,tsp.getSoundManager()->getHeadshotSound(),0.65f);
		iw->requestEntitySpawn(es);
		headshot=false;
	}
	gotHit(hitTime, 0, projectile);


	ml.pop();


	ml.push();	//body
	ml = ml
		* glm::translate(vec3(0.0f, 0.6f, 0.0f))
		* glm::scale(vec3(1, 3, 2));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	gotHit(hitTime, 1, projectile);

	ml.pop();



	ml.push();	//arm
	loc = 1;

	ml = ml 
		* glm::translate(vec3(0, 1.1f, loc * 0.3f))
		* glm::rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, vec3(0, 0, 1))//sin(loc*animStep * 2 *speed/size) * 16 + 90

		* glm::translate(vec3(0, -0.6f, 0))
		* glm::scale(vec3(1, 3, 1));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	gotHit(hitTime, 2, projectile);

	ml.pop();

	ml.push();	//arm
	loc = -1;

	ml = ml 
		* glm::translate(vec3(0, 1.1f, loc * 0.3f))
		* glm::rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, vec3(0, 0, 1))
		* glm::translate(vec3(0, -0.6f, 0))
		* glm::scale(vec3(1, 3, 1));

	hitTime = checkBox(projConv,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	gotHit(hitTime, 3, projectile);

	ml.pop();

	ml.pop();
	float after=this->remainingHP;
	if(before==after)
	{
		WarnErrReporter::noEffectErr("expected hp change when hit, but no hp change detected");
	}
//	std::cout<<"Entity ptr="<<(Entity *)this<<"; remainingHP="<<remainingHP<<std::endl;

}



float Zombie_Enemy::checkBox(DualPointer<Projectile> projectile,CumulativeMat* ml, float xFrom, float xTo, float yFrom,float yTo, float zFrom, float zTo,TickServiceProvider& tsp)
{
	IWorld * iw=&tsp.world();
	float ret=-1;
	ml->push();
	*ml = *ml 
		* translate(vec3(xFrom, yFrom, zFrom))
		* scale(vec3(xTo - xFrom, yTo - yFrom, zTo - zFrom));

	mat4 matOut = glm::inverse( (mat4)*ml );
	vec3 p1 = matOut* (iw->toMeters(projectile.pIF->posOld-pos));
	vec3 p2 = matOut*(iw->toMeters(projectile.e->pos-pos));
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
	ml->pop();
	return ret;
}

bool Zombie_Enemy::gotHit(float time, int part, EntityProjectileBulletLike * projectile)
{
	bool before=legDmg>0.25f*totalHP;
	if(time==-1) return false;//no hit
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
//	std::cout<<"hit on body part "<<part<<std::endl;
	legDmg+=legDmgMult*projectile->typeB.dmgPerJd0;//TODO replace with better logic once guns 2.0 work
	remainingHP -= projectile->typeB.dmgPerJd0*dmgMult;

	projectile->pos.y.intpart -= 1000;
	projectile->posOld.y.intpart -= 1000;
	if((!before)&&(legDmg>0.25f*totalHP))
	{
		speed/=2.5f;
		return true;
	}
	return false;
}

void Zombie_Enemy::testHit(std::vector<ProjectileCollision> * collisions,hitType type,DualPointer<Projectile> projectile,TickServiceProvider& tsp)
{
	ml.loadIdentity();
	ml = ml 
		* rotateDeg(facing,vec3( 0, 1, 0))
		* rotateDeg(fallAnim.getCurStep(0)*90,vec3( 1, 0, 0))
		* scale(vec3(size, size, size));


	ml.push();
	float animStrength=1;
	if(legDmg>0.25f*totalHP)
	{
		ml = ml 
			* translate(vec3(-0.25f*transitionAnim.getCurStep(0)*maxTransition,0.1f*transitionAnim.getCurStep(0)*maxTransition,0))
			* rotateDeg(-90*transitionAnim.getCurStep(0)*maxTransition, vec3(0, 0, 1));
		animStrength=0.15f*transitionAnim.getCurStep(0)*maxTransition;
	}

	ml.push();//leg
	int loc;
	float hitTime;
	loc = 1;
	ml = ml 
		* translate(vec3(0, 0.6f, loc * 0.1f))
		* rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength,vec3( 0, 0, 1))//sin(loc*animStep * 2 *speed/size) * 30*animStrength, 0, 0, 1);
		* translate(vec3(0, -0.6f, 0))
		* scale(vec3(1, 3, 1));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}

	ml.pop();


	ml.push();	//leg
	loc = -1;
	ml = ml 
		* translate(vec3(0, 0.6f, loc * 0.1f))
		* rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25*loc))*30*animStrength,vec3( 0, 0, 1))
		* translate(vec3(0, -0.6f, 0))
		* scale(vec3(1, 3, 1));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}

	ml.pop();
	ml.pop();


	ml.push();

	if(legDmg>0.25f*totalHP)
	{
		ml = ml 
			* translate(vec3(0,-0.4f*transitionAnim.getCurStep(0)*maxTransition,0))
			* rotateDeg((float)-35*transitionAnim.getCurStep(0)*maxTransition,vec3( 0, 0, 1));
	}

	ml.push();	//head

	ml = ml	
		* translate(vec3(0, 1.2f, 0))
		* scale(vec3(2, 2, 2))
		* rotateDeg(tilted, vec3(1, 0, 0));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}


	ml.pop();


	ml.push();	//body
	ml = ml 
		* translate(vec3(0, 0.6f, 0))
		* scale(vec3(1, 3, 2));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}

	ml.pop();

	ml.push();	//arm
	loc = 1;

	ml = ml 
		* translate(vec3(0, 1.1f, loc * 0.3f))
		* rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, vec3(0, 0, 1))//sin(loc*animStep * 2 *speed/size) * 16 + 90
		* translate(vec3(0, -0.6f, 0))
		* scale(vec3(1, 3, 1));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}

	ml.pop();

	ml.push();	//arm
	loc = -1;

	ml = ml 
		* translate(vec3(0, 1.1f, loc * 0.3f))
		* rotateDeg((float)sin(bodyAnim.getCurStepTau(0.25+0.25*loc))*16+90, vec3(0, 0, 1))
		* translate(vec3(0, -0.6f, 0))
		* scale(vec3(1, 3, 1));

	hitTime = checkBox(projectile,&ml,-0.1f, 0.1f, 0, 0.2f, -0.1f, 0.1f,tsp);
	if((hitTime>=0)&&(hitTime<=1))
	{
		collisions->push_back(ProjectileCollision(hitTime,(Entity *)this,(Hittable *) this,type));//,cm->activeChunk));
	}

	ml.pop();

	ml.pop();
}

void Zombie_Enemy::push(spacevec amount, TickServiceProvider& tsp)
{
	pos+=amount;
}


HittableBulletLike* Zombie_Enemy::asHittableBulletLike()
{
	return this;
}
