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
#include <string>
#include <memory>

#include "TickServiceProvider.h"
#include "SimClock.hpp"
#include "Cfg.hpp"
#include "User.hpp"
#include "EntityPlayer.h"
#include "AdaptiveQuality.h"

using std::map;
using std::string;
using std::unique_ptr;


class IGameMode: public TickServiceProvider
{
public:
	map<User*, unique_ptr<EntityPlayer> > players;

	SimClock clock;
	Cfg& cfg;

	PerformanceMeter pmLogic;

	IGameMode(const WallClock& reference_clock, Cfg& cfg);
	virtual ~IGameMode() = default;

	virtual void init()=0;
	virtual void step()=0;

	unique_ptr<DrawServiceProvider> getPerspective( User* user );

	void registerUser(User* new_user);
	virtual void onRegisterUser( User* );
	void kickUser(User* to_kick_user);
};

#endif /* SRC_IGAMEMODE_H_ */
