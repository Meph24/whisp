/*
 * SimClock::time_pointManager.h
 *
 *  Created on:	28.04.2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TIMESTAMPMANAGER_H_
#define SRC_TIMESTAMPMANAGER_H_

#include <chrono>
#include <ratio>

#include <SFML/Window.hpp>

#include "ShortNames.h"

#include "WarnErrReporter.h"

using namespace std::chrono;

/*
typedef duration<float> FloatSeconds;

float flt(const FloatSeconds& fs) { return fs.count(); }
*/

typedef float FloatSecondsFloatingType;

struct FloatSeconds : public duration<FloatSecondsFloatingType>
{ 
	using duration::duration; //inherit constructors
	explicit operator FloatSecondsFloatingType() { return count(); }
};


/*
template<typename BaseClock>
struct RelativeClock
{
	typedef microseconds duration;
	typedef duration::rep rep;
	typedef duration::period period;
	typedef std::chrono::time_point<RelativeClock<BaseClock>, duration> time_point;
	static const bool is_steady = true;


	const BaseClock& baseclock;
	typename BaseClock::duration max_master_wall_time;
	duration max_master_game_time;

	typename BaseClock::time_point last_master_time;

	float last_master_rate;
	float target_rate;//can be changed freely at runtime

	RelativeClock(float TargetRate,float MaxMasterWallTime,float MaxMasterGameTime)
		: max_master_wall_time(MaxMasterWallTime*1000)
		, max_master_game_time(MaxMasterGameTime*1000)
		, target_rate(TargetRate)
	{
		last_master_time= baseclock.now();
	}

	time_point update()
	{
		typename BaseClock::time_point current_base_time = baseclock.now();
		typename BaseClock::duration elapsed_base_time = current_base_time - last_master_time;
		if(elapsed_base_time<microseconds(0))
		{
			WarnErrReporter::timeBackwardErr("found a time elapsed <0");
			elapsed_base_time=microseconds(0);
		}
		//duration that can elapse in base-time is constraint
		elapsed_base_time=elapsed_base_time>max_master_wall_time?max_master_wall_time:elapsed_base_time;
		duration elapsed_relative_time = elapsed_base_time * target_rate;
		//duration that can elapse in relative-time is contstraint
		elapsed_relative_time=elapsed_relative_time>max_master_game_time?max_master_game_time:elapsed_relative_time;
		last_master_rate=elapsed_relative_time/elapsed_base_time;
		return last_master_time + elapsed_relative_time;
	}

	time_point now()
	{
		typename BaseClock::time_point current_base_time=baseclock.now();
		typename BaseClock::duration elapsed_base_time= current_base_time - last_master_time;
		if(elapsed_base_time<microseconds(0))
		{
			WarnErrReporter::timeBackwardErr("found a time elapsed <0");
			elapsed_base_time=microseconds(0);
		}
		duration elapsed_relative_time = elapsed_base_time * last_master_rate;
		return last_master_time + elapsed_relative_time;
	}
};

typedef RelativeClock<SFMLClock> SimClock;
*/
#endif /* SRC_TIMESTAMPMANAGER_H_ */
