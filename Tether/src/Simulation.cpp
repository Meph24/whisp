#include "Simulation.hpp"

#include <memory>
#include <mutex>
#include <utility>

#include "Spacevec.h"
#include "User.hpp"
#include "EntityPlayer.h"
#include "CameraTP.h"

using std::lock_guard;
using std::unique_ptr;
using std::pair;


Simulation::Simulation(const WallClock& reference_clock, Cfg& cfg,unique_ptr<IWorld> iW,sf::Packet * p)
	: clock(reference_clock, /* rate */ 0.0f, 200ms, 20ms,p)
	, cfg(cfg)
	, iw(std::move(iW))//iw(iW)
	, pmLogic(1s, 8s)
{
	std::cout<<"sim"<<std::endl;
	if(!iw)
	{
		iw.reset(new WorldDefault(16));
	}
	std::cout<<"sim2"<<std::endl;
}

Simulation::Simulation(const WallClock& reference_clock, Cfg& cfg,sf::Packet * p)
	: Simulation(reference_clock,cfg,unique_ptr<IWorld>(),p )
{}

unique_ptr<Perspective> Simulation::getPerspective( LocalUser* user )
{
	if(!user) return nullptr;
	if(!user->simulation) return nullptr;

	{
		lock_guard lg_ (players_lock);	
		if( players.find(user) == players.end() ) return nullptr;
	}

	return std::make_unique<Perspective>( user->window, *user);
}

void Simulation::onRegisterUser( SimulationUser* ){}

EntityPlayer* Simulation::registerUser(SimulationUser* user, spacevec spawn_pos)
{ 
	lock_guard lg_ (players_lock);	
	if( players.find(user) != players.end() ) return nullptr;

	unique_ptr<EntityPlayer> newentityplayer = std::make_unique<EntityPlayer>( 
		/* spawn time */		clock.now(),
		/* spawn position */	spawn_pos,
		/* character speed */ 	(! *cfg.getInt("test","debug_movement") ) ? 7.6f : 30.6f );

	iw->requestEntitySpawn(newentityplayer.get(), true);
	players[user] = newentityplayer.get();
	players[user]->setUser(user);
	newentityplayer.release();
	onRegisterUser(user);
	return players[user];
}

EntityPlayer* Simulation::userAvatar(SimulationUser* user) 
{ 
	lock_guard lg_ (players_lock);	
	auto f = players.find(user); 
	return (f == players.end())? nullptr : f->second;
}

void Simulation::kickUser( SimulationUser* to_kick_user )
{ 
	lock_guard lg_ (players_lock);	
	players.erase(to_kick_user);
}

void Simulation::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	iw->getOwnedSyncables(collectHere);
}

void Simulation::drawOtherStuff(const SimClock::time_point& draw_time,Frustum* viewFrustum, IWorld& iw, Perspective& perspective)
{} //default: nothing to draw

IWorld& Simulation::world()
{
	return *iw;
}

void Simulation::serialize(sf::Packet& p, bool complete)
{
	clock.serialize(p,complete);
}

void Simulation::deserialize(sf::Packet& p, SyncableManager& sm)
{
	clock.deserialize(p,sm);
}
