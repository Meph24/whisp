/*
 * IGameMode.h
 *
 *  Created on:	Dec 11, 2019
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_IGAMEMODE_H_
#define SRC_IGAMEMODE_H_

#include <map>
#include <memory>
#include <string>

#include "AdaptiveQuality.h"
#include "Cfg.hpp"
#include "EntityPlayer.h"
#include "SimClock.hpp"
#include "TickServiceProvider.h"
#include "User.hpp"

using std::map;
using std::string;
using std::unique_ptr;

class Simulation: public TickServiceProvider
{
public:
	map<User*, unique_ptr<EntityPlayer> > players;

	SimClock clock;
	Cfg& cfg;

	PerformanceMeter pmLogic;

	Simulation(const WallClock& reference_clock, Cfg& cfg);
	virtual ~Simulation() = default;

	virtual void init()=0;
	virtual void step()=0;

	unique_ptr<Perspective> getPerspective( User* user );

	void registerUser(User* new_user);
	virtual void onRegisterUser( User* );
	void kickUser(User* to_kick_user);
};

#endif /* SRC_IGAMEMODE_H_ */
