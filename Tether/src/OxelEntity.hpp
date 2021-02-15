#ifndef OXELENTITY_HPP
#define OXELENTITY_HPP

#include "Oxel.hpp"
#include "TickServiceProvider.h"
#include "IWorld.h"
#include "AABB.h"
#include "FloatSeconds.hpp"
#include "Spacevec.h"
#include "Entity.h"

class OxelEntity : public Entity
{
    OxelTree t;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

    void drawOxelTree( const OxelTree::Viewer& viewer, const int8_t& mingranularity = -5 );
public:
    OxelEntity( const OxelTree& t );

    void tick(	const SimClock::time_point& next_tick_begin,
				TickServiceProvider* tsp
				);

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						Perspective& perspective
					 );
};

#endif // OXELENTITY_HPP
