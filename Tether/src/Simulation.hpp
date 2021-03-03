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
	map<SimulationUser*, EntityPlayer* > players;

	SimClock clock;
	Cfg& cfg;
	unique_ptr<IWorld> iw;

	PerformanceMeter pmLogic;

	Simulation(const WallClock& reference_clock, Cfg& cfg,unique_ptr<IWorld> iW);
	Simulation(const WallClock& reference_clock, Cfg& cfg);

	virtual ~Simulation() = default;

	virtual void init()=0;
	virtual void step()=0;
	virtual void drawOtherStuff(  const SimClock::time_point& draw_time,
            Frustum* viewFrustum,
            IWorld& iw,
            Perspective& perspective);

	unique_ptr<Perspective> getPerspective( LocalUser* user );

	EntityPlayer* registerUser(SimulationUser* new_user);
	EntityPlayer* userAvatar(SimulationUser* user);
	virtual void onRegisterUser( SimulationUser* );
	void kickUser(SimulationUser* to_kick_user);

	virtual void getOwnedSyncables(std::vector<Syncable *> collectHere);

	virtual IWorld& world();
};

#endif /* SRC_IGAMEMODE_H_ */
