#include "Simulation.hpp"

#include <memory>
#include <utility>

#include "Spacevec.h"
#include "User.hpp"
#include "EntityPlayer.h"
#include "CameraTP.h"

using std::unique_ptr;
using std::pair;

Simulation::Simulation(const WallClock& reference_clock, Cfg& cfg,unique_ptr<IWorld> iW)
	: clock(reference_clock, /* rate */ 0.0f, 200ms, 20ms)
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

Simulation::Simulation(const WallClock& reference_clock, Cfg& cfg)
	: Simulation(reference_clock,cfg,unique_ptr<IWorld>() )
{}

unique_ptr<Perspective> Simulation::getPerspective( LocalUser* user )
{
	if(!user) return nullptr;
	if(!user->simulation) return nullptr;
	if( players.find(user) == players.end() ) return nullptr;

	return std::make_unique<Perspective>( user->window, *user);
}

void Simulation::onRegisterUser( SimulationUser* ){}

EntityPlayer* Simulation::registerUser(SimulationUser* user)
{ 
	if( players.find(user) != players.end() ) return nullptr;

	unique_ptr<EntityPlayer> newentityplayer = std::make_unique<EntityPlayer>( 
		/* spawn time */		clock.now(),
		/* spawn position */	spacevec(),
		/* character speed */ 	(! *cfg.getInt("test","debug_movement") ) ? 7.6f : 30.6f );

	iw->requestEntitySpawn(newentityplayer.get());
	players[user] = newentityplayer.get();
	players[user]->setUser(user);
	newentityplayer.release();
	onRegisterUser(user);
	return players[user];
}

EntityPlayer* Simulation::userAvatar(SimulationUser* user) 
{ 
	auto f = players.find(user); 
	return (f == players.end())? nullptr : f->second;
}

void Simulation::kickUser( SimulationUser* to_kick_user ){ players.erase(to_kick_user);}

void Simulation::getOwnedSyncables(std::vector<Syncable*> collectHere)
{
	for(auto& p : players)
		collectHere.push_back(p.second);
	iw->getOwnedSyncables(collectHere);
}
void Simulation::drawOtherStuff(const SimClock::time_point& draw_time,Frustum* viewFrustum, IWorld& iw, Perspective& perspective)
{} //default: nothing to draw

IWorld& Simulation::world()
{
	return *iw;
}
