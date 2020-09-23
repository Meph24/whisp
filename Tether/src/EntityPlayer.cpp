/*
 * EntityPlayer.cpp
 *
 *  Created on:	Apr 16, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#include <GL/glew.h>
#include "EntityPlayer.h"

#include "glmutils.hpp"
using glm::vec3;

#include "faction.h"
#include "Frustum.h"
#include "SpeedMod.h"
#include "TopLevelInventory.h"
#include "ItemDummy.h"
#include "CameraTP.h"
#include "HittableBulletLike.h"
#include "TickServiceProvider.h"
#include "ChunkManager.h"
#include "EventMapper.hpp"
#include "Frustum.h"
#include "DrawServiceProvider.h"
#include "Zombie_Gun.h"
#include "Graphics2D.h"
#include "Zombie_MouseInput.h"
#include "ItemAmmo.h"
#include "InteractFilterAlgoSym.h"
#include "FloatSeconds.hpp"

EntityPlayer::EntityPlayer(	SimClock::time_point spawn_time,
							spacevec startPos,
							sf::Window * w,
							float sensX,
							float sensY,
							float characterSpeed)
	: prev_inventory_signal(SimulationInputStatusSet().inventory) 
	, speed(characterSpeed)
	, heldItem(0)
	, inventory(0)
{
	surviveClearing=true;
	last_ticked=spawn_time;
	pos=startPos;
	v.set0();
	cam=new CameraTP();
	cam->alpha=0.0001f;
	cam->beta=0.0001f;
	cam->gamma=0.0001f;
	cam->height=w->getSize().y;
	cam->width=w->getSize().x;
	cam->maxView=1024*8;
	setTP(false);
	cam->zoom=defaultZoom;
	mouseInp = new Zombie_MouseInput(this, w,sensX,sensY);
	mouseInp->enable();
	HP=maxHP;

	pushRadius=0.4f;
	pushForce=speed/30;

	bb=AABB(pos);

	TopLevelInventory * tli=new TopLevelInventory(this);
	for(int i=0;i<42;i++)
	{
		tli->items.push_back(new ItemDummy("Duftkerze Nummer "+std::to_string(i)));
	}
	inventory=tli;//TODO reenable this line to enable inventory

	guns.emplace_back(new Zombie_Gun(spawn_time,"Glock 17 9mm",0.2f,"res/gunshot.wav",0.9f,new ItemAmmo(358, 79.5f,0.001628170585565067f,1),false,{2,0.15f,0},{1,0.5f,0}));//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns.emplace_back(new Zombie_Gun(spawn_time,"Flamethrower",0.04f,"res/mortar_shoot.wav",1,new ItemAmmo(20, 75,0.005f,1),true,{0.2f,0,0},{0.05f,0.01f,0}));//new Zombie_Gun(30000, 800,5.0f);
	guns.emplace_back(new Zombie_Gun(spawn_time,"American 180 .22",0.05f,"res/gunshot.wav",1.2f,new ItemAmmo(440,31.8f,0.0022272754325748604f,1),true,{0.5f,0,0},{0.5f,0.5f,0}));//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns.emplace_back(new Zombie_Gun(spawn_time,"Barret M95 .50BMG",1.5f,"res/gunshot.wav",0.6f,new ItemAmmo(900, 3166,0.0004f,1),false,{5,0,0},{2,2,0}));
	guns.emplace_back(new Zombie_Gun(spawn_time,"G3A3 .308",0.12f,"res/gunshot.wav",0.75f,new ItemAmmo(800, 200,0.0008f,1),true,{3,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(spawn_time,"Shotgun",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,45.0f,0.0022272754325748604f,9),true,{2.5f,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(spawn_time,"Shotgun with Birdshot",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,0.30f,0.0038f,900),true,{2.5f,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(spawn_time,"Cheat Blaster 180",0.08f,"res/gunshot.wav",0.5f,new ItemAmmo(600,200.30f,0.0018f,180),true,{0.5f,0,0},{0.5f,0.5f,0}));

	current_gun = guns[0].get();
}

EntityPlayer::~EntityPlayer()
{
	delete cam;
	delete mouseInp;
	if(heldItem) delete heldItem;
	if(inventory) delete inventory;
}



void EntityPlayer::selectWeapon(size_t selection)
{
	Zombie_Gun* selected_gun =  guns[selection%guns.size()].get();
	if( current_gun == selected_gun ) return;

	current_gun->stopShooting();
	current_gun = guns[selection%guns.size()].get();
}


void EntityPlayer::draw(const SimClock::time_point& t,Frustum * viewFrustum,IWorld& iw, DrawServiceProvider* dsp)
{
	//float tickOffset=t-lastTick;
	if(isPerspective || cam->dist>minTPdist)
	{
		//TODO draw yourself
		isPerspective=false;
	}

	glEnable(GL_TEXTURE_2D);
	dsp->transformViewToGUI();
	glColor3f(0, 1, 0);
	glPushMatrix();
	char scoreString[8];
	int scoreTemp = score;
	for (int i = 0; i < 8; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10)+'0';
		scoreTemp /= 10;
	}
	dsp->g->drawString("score:", -0.8f, 0.8f, 0.1f);
	dsp->g->drawString(scoreString, -0.8f, 0.62f, 0.1f, 8);

	scoreTemp = HP;
	for (int i = 0; i < 3; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	dsp->g->drawString("health:", -0.2f, 0.8f, 0.1f);
	dsp->g->drawString(scoreString+5, -0.2f, 0.62f, 0.1f, 3);

	glColor3f(1, 1, 0);
	dsp->g->drawString("Weapon:", 0.6f, -0.66f, 0.1f);
	dsp->g->drawString(current_gun->name, 0.6f, -0.82f, 0.1f);
	glColor3f(0, 1, 0);
	glPopMatrix();
	float crosshairSize = 0.005f;
	int crosshairAmount = 4;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1, hitmark, 0);

	glPushMatrix();
	for (int i = 0; i < crosshairAmount; i++)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, crosshairSize, 1);
		glVertex3f(-crosshairSize, crosshairSize * 6, 1);
		glVertex3f(crosshairSize, crosshairSize * 6, 1);
		glEnd();
		glRotatef(360.0f / crosshairAmount, 0, 0, 1);
	}
	glPopMatrix();
	dsp->revertView();

	if(heldItem) heldItem->draw(t,viewFrustum,iw,dsp);
}


void EntityPlayer::setTP(bool on)
{
	if(on)
	{
		if(cam->dist<minTPdist)
		{
			cam->dist=minTPdist;
		}
		cam->minView=0.5f;
	}
	else
	{
		cam->dist=0;
		cam->minView=0.125f;
	}
}

void EntityPlayer::changeTPdist(float amount)
{
	cam->dist+=amount;
	setTP(cam->dist>=minTPdist);
	if(cam->dist>maxTPdist) cam->dist=maxTPdist;
}

spacevec EntityPlayer::getCamPos()
{
	return pos+characterEyeOffset;
}
#include "EventDefines.h"
Frustum * EntityPlayer::newFrustumApplyPerspective(SimClock::time_point t,bool fresh,TickServiceProvider * tsp,float viewDistRestriction)
{
	SimulationInputStatusSet& controlinputs = *(tsp->input_status);
	float zoomMult=8;
	bool zoomed=controlinputs.zoom;
	float zoomFactor=zoomed?zoomMult:1;
	mouseInp->setSensitivityMultiplier(1.0f/zoomFactor);
	cam->zoom=defaultZoom/zoomFactor;
	IWorld * iw=tsp->getIWorld();
	float time=(float)FloatSeconds(t-last_ticked);
	spacevec curPos=pos+v*time;
	cam->posX=iw->toMeters(characterEyeOffset.x);
	cam->posY=iw->toMeters(characterEyeOffset.y);
	cam->posZ=iw->toMeters(characterEyeOffset.z);
	if(fresh) cam->applyFresh();
	else cam->apply();
	vec3 fwd=cam->getForwardVector();
	sf::Listener::setDirection(fwd.x,fwd.y,fwd.z);
	vec3 upv=cam->getUpVector();
	sf::Listener::setUpVector(upv.x,upv.y,upv.z);
	sf::Listener::setPosition(cam->posX,cam->posY,cam->posZ);
	isPerspective=true;

	Frustum * ret=new Frustum();
	ret->observerPos=curPos;
	bool lookingUp=cam->alpha<0;
	if(lookingUp)
	{
		ret->planes[0]=cam->getUpperPlane();
		ret->planes[3]=cam->getLowerPlane();
	}
	else
	{
		ret->planes[0]=cam->getLowerPlane();
		ret->planes[3]=cam->getUpperPlane();
	}
	ret->planes[1]=cam->getLeftPlane();
	ret->planes[2]=cam->getRightPlane();
	ret->planes[4]=cam->getFarPlane(viewDistRestriction);
	if(FRUSTUM_PLANE_COUNT==6)
	{
		ret->planes[5]=cam->getNearPlane();
	}
	return ret;
}
#include "EventDefines.h"
void EntityPlayer::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * iw=tsp->getIWorld();
	ITerrain * it=tsp->getITerrain();

	SimulationInputStatusSet& controlinputs = *tsp->input_status;
	selectWeapon(controlinputs.weapon_selection);

	if(prev_inventory_signal != controlinputs.inventory)
	{
		prev_inventory_signal = controlinputs.inventory;

		Item * temp=heldItem;//put inventory in hand or put it back
		heldItem=inventory;
		inventory=temp;

		//this writes to controlinputs, which shall not be permitted in the future ( issue #36 )
		//this line will need to go
		controlinputs.toggleCondition(controlinputs.selection_active);//TODO find out which is inventory and set value ORed accordingly
	}

	if(heldItem) heldItem->tick(next_tick_begin, tsp);

	characterEyeOffset=iw->toUnitLength(it->getGravity(pos))*(-characterEyeHeight);
	//std::cout<<"pos"<<pos<<std::endl;
	//std::cout<<"gravity"<<it->getGravity(pos)<<std::endl;
	//std::cout<<"characterEyeHeight"<<characterEyeHeight<<std::endl;
	//std::cout<<"iw->toUnitLength(it->getGravity(pos))"<<iw->toUnitLength(it->getGravity(pos))<<std::endl;
	float time=(float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;
	hitmark -= time * 10;
	if (hitmark < 0) hitmark = 0;
	HP += maxHP*time / 200;
	if (HP > maxHP) HP = maxHP;

	spacevec oldPos=pos;

	vec3 wantedV = controlinputs.walk;
	wantedV.z*=-1; //invert because someone thought it would be nice forward meaning negative
	if(wantedV != vec3(0.0f))
	{
		wantedV = cam->getNormal(wantedV);
	}
	pos+=iw->fromMeters(wantedV * speed )*time;

	if(controlinputs.clip)
	{
		pos=it->clip(pos,true);
		spacevec newPos=pos;
		vec3 moved=iw->toMeters(newPos-oldPos);
		if(glm::sqlen(moved) > 0.0000000001f)
		{
			vec3 norm=glm::normalize(moved);
			float speedModA=(glm::length(vec3(norm.x,0,norm.z)));
			vec3 flat=vec3(moved.x,0,moved.z);
			float h=moved.y/glm::length(flat);
			SpeedMod sm=SpeedMod();
			float speedModB=sm.slowdownFromTerrain(h);
			pos=it->clip(oldPos+iw->fromMeters(flat*speedModA*speedModB),true);
		}
	}
	
	if(time>0.0000000001f)
		v=(pos-oldPos)/time;

	spacevec size;
	size.x=iw->fromMeters(characterEyeHeight*0.5f);
	size.y=iw->fromMeters(characterEyeHeight*1.5f);
	size.z=size.x;
	bb=AABB(pos,size,v*time);
	iw->pushAlgo->doChecks((Pushable *)this,(Entity *)this,time,*tsp);
}

void EntityPlayer::push(spacevec amount, TickServiceProvider& tsp)
{
	pos+=amount;
	std::cout<<"amount"<<amount<<" | "<<tsp.getIWorld()->toMeters(amount);
//	HP -= 15625*glm::sqlen(tsp.getChunkManager()->toMeters(amount));
}
#include <iostream>
void EntityPlayer::hitCallback(float dmg, bool kill, bool projDestroyed,HittableBulletLike* victim)
{
	if(!victim) std::cout<<"victim 0"<<std::endl;
	else if(dmg>0 && victim->fac==FACTION_ZOMBIES)
	{
		hitmark=1;
	}
}

void EntityPlayer::trigger(bool pulled, SimClock::time_point now,ITexture * tex,IWorld& iw)
{
	if (!pulled)
	{
		current_gun->stopShooting();
		return;
	}
	current_gun->tryShoot(now,cam,this,tex,iw);
}
