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

using std::map;
using std::string;
using std::unique_ptr;

class Simulation: public TickServiceProvider, public Syncable
{
public:
	map<SimulationUser*, unique_ptr<EntityPlayer> > players;

	SimClock clock;
	Cfg& cfg;

	PerformanceMeter pmLogic;

	Simulation(const WallClock& reference_clock, Cfg& cfg);
	virtual ~Simulation() = default;

	virtual void init()=0;
	virtual void step()=0;

	unique_ptr<Perspective> getPerspective( LocalUser* user );

	void registerUser(SimulationUser* new_user);
	virtual void onRegisterUser( SimulationUser* );
	void kickUser(SimulationUser* to_kick_user);
};

#endif /* SRC_IGAMEMODE_H_ */
