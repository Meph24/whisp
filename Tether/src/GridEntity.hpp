#ifndef GRIDENTITY_HPP
#define GRIDENTITY_HPP

#include "Spacevec.h"
#include "Entity.h"
#include "Grid.hpp"

struct GridEntity : public Entity
{
    Grid<32> grid;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

    GridEntity( const Grid<32>& grid ) : grid(grid) {} ;

    void tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 );
    

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						Perspective& perspective
					 );
};

#endif // GRIDENTITY_HPP
