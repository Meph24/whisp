#include "Simulation.hpp"

#include <memory>
#include <utility>

#include "Spacevec.h"
#include "User.hpp"
#include "EntityPlayer.h"
#include "CameraTP.h"

using std::unique_ptr;
using std::pair;

Simulation::Simulation(const WallClock& reference_clock, Cfg& cfg)
	: clock(reference_clock, /* rate */ 0.0f, 200ms, 20ms)
	, cfg(cfg)
	, pmLogic(1s, 8s)
{}

unique_ptr<Perspective> Simulation::getPerspective( LocalUser* user )
{
	if( players.find(user) == players.end() ) return nullptr;

	return std::make_unique<Perspective>( user->window, players[user].get(), this );
}

void Simulation::onRegisterUser( SimulationUser* ){}

void Simulation::registerUser(SimulationUser* user)
{ 
	if( players.find(user) != players.end() ) return;
	//TODO evaluate : should I throw in this case ... probably later when some form of GUI can catch the exception

	float sensx = *cfg.getFlt("input", "sensitivityX");
	float sensy = *cfg.getFlt("input", "sensitivityY");

	players[user] = std::make_unique<EntityPlayer>( 
		/* spawn time */		clock.now(),
		/* spawn position */	spacevec(),
								sensx,
								sensy,
		/* character speed */ 	(! *cfg.getInt("test","debug_movement") ) ? 7.6f : 30.6f );
	players[user]->setUser(user);
	onRegisterUser(user);
}

void Simulation::kickUser( SimulationUser* to_kick_user ){ players.erase(to_kick_user);}
