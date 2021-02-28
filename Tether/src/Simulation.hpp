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
#include "Syncable.h"
#include "WorldDefault.h"

using std::map;
using std::string;
using std::unique_ptr;

class Simulation: public TickServiceProvider, public Syncable
{
public:
	map<User*, unique_ptr<EntityPlayer> > players;

	SimClock clock;
	Cfg& cfg;
	unique_ptr<IWorld> iw;

	PerformanceMeter pmLogic;


	Simulation(const WallClock& reference_clock, Cfg& cfg,unique_ptr<IWorld> iW);

	Simulation(const WallClock& reference_clock, Cfg& cfg);//create your own IWorld

	virtual ~Simulation() = default;

	virtual void init()=0;
	virtual void step()=0;
	virtual void drawOtherStuff(  const SimClock::time_point& draw_time,
            Frustum* viewFrustum,
            IWorld& iw,
            Perspective& perspective);

	unique_ptr<Perspective> getPerspective( User* user );

	void registerUser(User* new_user);
	virtual void onRegisterUser( User* );
	void kickUser(User* to_kick_user);

	virtual void getOwnedSyncables(std::vector<Syncable *> collectHere);

	virtual IWorld& world();
};

#endif /* SRC_IGAMEMODE_H_ */
