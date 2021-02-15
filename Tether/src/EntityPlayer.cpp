#include "EntityPlayer.h"

#include <GL/glew.h>
#include "glmutils.hpp"

#include "CameraTP.h"
#include "ChunkManager.h"
#include "drawUtils.hpp"
#include "EventDefines.h"
#include "EventMapper.hpp"
#include "faction.h"
#include "FloatSeconds.hpp"
#include "Frustum.h"
#include "Graphics2D.h"
#include "HittableBulletLike.h"
#include "ItemDummy.h"
#include "ItemAmmo.h"
#include "InteractFilterAlgoSym.h"
#include "Perspective.hpp"
#include "SpeedMod.h"
#include "TickServiceProvider.h"
#include "TopLevelInventory.h"
#include "Zombie_Gun.h"
#include "sfml_packet_utils.hpp"

using glm::vec3;

EntityPlayer::EntityPlayer(	SimClock::time_point spawn_time,
							spacevec startPos,
							float sensX,
							float sensY,
							float characterSpeed)
	: player_mesh (diamondMesh(3, 0.2, 0.5))
	, player_model( player_mesh )
	, eye(*this)
	, speed(characterSpeed)
{
	surviveClearing=true;
	last_ticked=spawn_time;
	pos=startPos;
	v.set0();
	
	HP=maxHP;

	pushRadius=0.4f;
	pushForce=speed/30;

	bb=AABB(pos);

	unused_inventory_slot = std::make_unique<TopLevelInventory>(this);//comment this line to disable inventory
	if(unused_inventory_slot)
	{
		for(int i=0;i<42;i++)
		{
			inventory()->items.push_back(new ItemDummy("Duftkerze Nummer "+std::to_string(i)));
		}
	}

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

void EntityPlayer::selectWeapon(size_t selection)
{
	Zombie_Gun* selected_gun =  guns[selection%guns.size()].get();
	if( current_gun == selected_gun ) return;

	current_gun->stopShooting();
	current_gun = guns[selection%guns.size()].get();
}

void EntityPlayer::draw(const SimClock::time_point& draw_time,Frustum * viewFrustum,IWorld& iw, Perspective& perspective)
{
	float tickOffset = (float) FloatSeconds(draw_time-last_ticked);

	spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
	vec3 draw_pos = iw.toMeters(interPos) + eye.offsetFromEntity();

	glPushMatrix();

	//apply position
	glTranslatef( draw_pos.x, draw_pos.y, draw_pos.z );

	glRotatef( -eye.rotation.z, 0.0f, 0.0f, 1.0f );
	glRotatef( -eye.rotation.y, 0.0f, 1.0f, 0.0f );
	glRotatef( -eye.rotation.x + 90, 1.0f, 0.0f, 0.0f );

	player_model.drawHere();

	glPopMatrix();

	if(held_item) held_item->draw(draw_time, viewFrustum, iw, perspective);
}

void EntityPlayer::setUser(User* user){ user_ = user; }
const User* EntityPlayer::user() const { return user_; }

void setLookingDirection(const vec3& forward, const vec3& up);
	
TopLevelInventory* EntityPlayer::inventory()
{
	if(!unused_inventory_slot) return (TopLevelInventory*) held_item.get();
	else return (TopLevelInventory*) unused_inventory_slot.get();
}

void EntityPlayer::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * iw=&tsp->world();
	ITerrain * it=tsp->getITerrain();

	if( user() )
	{
		const SimulationInputStatusSet& controlinputs = user()->input_status;

		int i = 0;

		i64 selectAdd = controlinputs.selection_down - prev_input_status.selection_down -i;
		selectAdd -= controlinputs.selection_up - prev_input_status.selection_up;
		if( prev_input_status.selection_up != controlinputs.selection_up )
			prev_input_status.selection_up = controlinputs.selection_up;
		if( prev_input_status.selection_down != controlinputs.selection_down )
			prev_input_status.selection_down = controlinputs.selection_down;
		inventory()->selectRelative(selectAdd);
	

		size_t weapon_selection = controlinputs.weapon_selection.value();
		selectWeapon( weapon_selection );

		if(!inventory()) //=> inventory is currently held
		{
			i64 selectAdd = controlinputs.selection_down - prev_input_status.selection_down;
			selectAdd -= controlinputs.selection_up - prev_input_status.selection_up;
			prev_input_status.selection_up = controlinputs.selection_up;
			prev_input_status.selection_down = controlinputs.selection_down;
			inventory()->selectRelative(selectAdd);
		}
		
		if(prev_input_status.inventory != controlinputs.inventory)
		{
			prev_input_status.inventory = controlinputs.inventory;

			unused_inventory_slot.swap(held_item);

			//this writes to controlinputs, which shall not be permitted in the future ( issue #36 )
			//this line will need to go
			controlinputs.toggleCondition(controlinputs.selection_active);//TODO find out which is inventory and set value ORed accordingly
		}
	}

	if(held_item) held_item->tick(next_tick_begin, tsp);

	float time=(float) FloatSeconds(next_tick_begin - last_ticked);
	last_ticked = next_tick_begin;
	hitmark -= time * 10;
	if (hitmark < 0) hitmark = 0;
	HP += maxHP*time / 200;
	if (HP > maxHP) HP = maxHP;

	if( user() )
	{
		const SimulationInputStatusSet& controlinputs = user()->input_status;
		const unique_ptr<CameraTP>& cam = user()->perspective->camera;
		spacevec oldPos=pos;

		vec3 wantedV = controlinputs.walk;
		wantedV.z*=-1; //invert because someone thought it would be nice forward meaning negative
		if(wantedV != vec3(0.0f))
		{
			wantedV = cam->getNormal(wantedV);
		}
		pos+=iw->fromMeters(wantedV * speed )*time;

		if(controlinputs.turn != prev_input_status.turn)
		{
			eye.rotation.x = controlinputs.turn.x;
			eye.rotation.y = controlinputs.turn.y;
			eye.rotation.z = controlinputs.turn.z;
			prev_input_status.turn = controlinputs.turn;
		}

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

		if (controlinputs.trigger)
			current_gun->tryShoot(next_tick_begin, this, *iw);
		else
			current_gun->stopShooting();

		if(time>0.0000000001f)
			v=(pos-oldPos)/time;
	}

	spacevec size;
	size.x=iw->fromMeters( eye.offsetFromEntity().y*0.5f );
	size.y=iw->fromMeters( eye.offsetFromEntity().y*1.5f );
	size.z=size.x;
	bb=AABB(pos,size,v*time);
	iw->pushAlgo->doChecks((Pushable *)this,(Entity *)this,time,*tsp);
}

void EntityPlayer::push(spacevec amount, TickServiceProvider& tsp)
{
	pos+=amount;
}

void EntityPlayer::hitCallback(float dmg, bool kill, bool projDestroyed,HittableBulletLike* victim)
{
	if(!victim) std::cout<<"victim 0"<<std::endl;
	else if(dmg>0 && victim->fac==FACTION_ZOMBIES)
	{
		hitmark=1;
		score+=dmg+kill*100;
	}
}

void EntityPlayer::serialize(sf::Packet& p, bool complete)
{
	p<<score;
	//TODO eye properly: start
	p<<eye.rotation;
	//TODO eye properly: end
	p<<HP;
	p<<hitmark;
	//TODO held stuff properly: start
	bool inventoryOpen=(bool)held_item;
	p<<inventoryOpen;
	i8 gunHeld=-1;
	size_t size=guns.size();
	for(size_t i=0;i<size;i++)
	{
		if(guns[i].get()==current_gun)
		{
			gunHeld=(i8) i;
			break;
		}
	}
	p<<gunHeld;
	//TODO held stuff properly: end
	p<<pos;
	p<<v;
	p<<last_ticked;

	if(complete)
	{

	}
}

void EntityPlayer::deserialize(sf::Packet& p, SyncableManager& sm)
{
	p>>score;
	//TODO eye properly: start
	p>>eye.rotation;
	//TODO eye properly: end
	p>>HP;
	p>>hitmark;
	//TODO held stuff properly: start
	bool inventoryShouldBeOpen;
	bool inventoryIsOpen=(bool)held_item;
	p>>inventoryShouldBeOpen;
	if(inventoryIsOpen!=inventoryShouldBeOpen)
	{
		unused_inventory_slot.swap(held_item);
	}
	i8 gunHeld;
	p>>gunHeld;
	if(gunHeld<0 || ((size_t)gunHeld)>=guns.size())
	{
		current_gun=0;
	}
	else
	{
		current_gun=guns[gunHeld].get();
	}
	//TODO held stuff properly: end
	p>>pos;
	p>>v;
	p>>last_ticked;
}

EntityPlayer::EntityPlayer(sf::Packet p, TickServiceProvider* tsp,SyncableManager& sm)
: player_mesh (diamondMesh(3, 0.2, 0.5))
, player_model( player_mesh )
, eye(*this)
{
	deserialize(p,sm);
	surviveClearing=true;
	HP=maxHP;

	pushRadius=0.4f;
	pushForce=speed/30;

	bb=AABB(pos);

	unused_inventory_slot = std::make_unique<TopLevelInventory>(this);//comment this line to disable inventory
	if(unused_inventory_slot)
	{
		for(int i=0;i<42;i++)
		{
			inventory()->items.push_back(new ItemDummy("Duftkerze Nummer "+std::to_string(i)));
		}
	}

	guns.emplace_back(new Zombie_Gun(last_ticked,"Glock 17 9mm",0.2f,"res/gunshot.wav",0.9f,new ItemAmmo(358, 79.5f,0.001628170585565067f,1),false,{2,0.15f,0},{1,0.5f,0}));//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns.emplace_back(new Zombie_Gun(last_ticked,"Flamethrower",0.04f,"res/mortar_shoot.wav",1,new ItemAmmo(20, 75,0.005f,1),true,{0.2f,0,0},{0.05f,0.01f,0}));//new Zombie_Gun(30000, 800,5.0f);
	guns.emplace_back(new Zombie_Gun(last_ticked,"American 180 .22",0.05f,"res/gunshot.wav",1.2f,new ItemAmmo(440,31.8f,0.0022272754325748604f,1),true,{0.5f,0,0},{0.5f,0.5f,0}));//new Zombie_Gun(300000, 40,0.18f);//new Zombie_Gun(120000, 40,0.2f);//TODO change
	guns.emplace_back(new Zombie_Gun(last_ticked,"Barret M95 .50BMG",1.5f,"res/gunshot.wav",0.6f,new ItemAmmo(900, 3166,0.0004f,1),false,{5,0,0},{2,2,0}));
	guns.emplace_back(new Zombie_Gun(last_ticked,"G3A3 .308",0.12f,"res/gunshot.wav",0.75f,new ItemAmmo(800, 200,0.0008f,1),true,{3,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(last_ticked,"Shotgun",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,45.0f,0.0022272754325748604f,9),true,{2.5f,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(last_ticked,"Shotgun with Birdshot",0.2f,"res/gunshot.wav",0.5f,new ItemAmmo(400,0.30f,0.0038f,900),true,{2.5f,0,0},{1.5f,1.5f,0}));
	guns.emplace_back(new Zombie_Gun(last_ticked,"Cheat Blaster 180",0.08f,"res/gunshot.wav",0.5f,new ItemAmmo(600,200.30f,0.0018f,180),true,{0.5f,0,0},{0.5f,0.5f,0}));

	current_gun = guns[0].get();
}

void EntityPlayer::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
}

void EntityPlayer::getReferencedSyncables(std::vector<Syncable*> collectHere)
{
}

u32 EntityPlayer::getClassID()
{
	return CLASS_ID_EntityPlayer;
}
