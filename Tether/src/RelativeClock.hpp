#ifndef RELATIVECLOCK_HPP
#     define RELATIVECLOCK_HPP

#include <chrono>

#include "WarnErrReporter.h"

using namespace std::chrono;

/**
 * @brief A Clock that runs in a relative speed to another clock.
 *
 * The clock enacts some possibility for constraints in the flow of time.
 * Other clock related properties (that the BaseClock must also provide):
 *	* microseconds accuracy
 *	* steadyness
 *
 * @tparam BaseClock The BaseClock is the clock the RelativeClock is relative to.
 */
template<typename BaseClock>
struct RelativeClock
{
	//std::chrono typedefs
	typedef typename BaseClock::duration duration;
	typedef typename BaseClock::duration::rep rep;
	typedef typename BaseClock::duration::period period;
	typedef std::chrono::time_point<RelativeClock<BaseClock>, duration> time_point;
	static const bool is_steady = BaseClock::is_steady;

	const BaseClock& baseclock;

private:
	typename	BaseClock::duration		max_base_time;
				duration				max_rel_time;
public:


private:
	typename	BaseClock::time_point	last_update_base_time;
				time_point				last_update_rel_time;

	float last_rate; //rate the last time before it was changed
	float target_rate; //the rate of relative timeflow that is aimed to be achieved
	float to_set_target_rate;
	typename BaseClock::duration to_set_max_base_time;
	duration to_set_max_rel_time;
public:
	RelativeClock(const BaseClock& baseclock, float target_rate, typename BaseClock::duration max_base_time,
			duration max_rel_time)
		: baseclock(baseclock)
		, max_base_time(max_base_time)
		, max_rel_time(max_rel_time)
		, last_rate(target_rate)
		, target_rate(target_rate)
		, to_set_target_rate(target_rate)
		, to_set_max_base_time(max_base_time)
		, to_set_max_rel_time(max_rel_time)
	{
		last_update_base_time = baseclock.now();
		last_update_rel_time = tick();
	}


	void setNextTargetRate(float new_target_rate){ to_set_target_rate = new_target_rate; }
	float targetRate() const { return target_rate; }
	void setNextMaxBaseTime(const typename BaseClock::duration& new_max_base_time) {to_set_max_base_time = new_max_base_time;}
	typename BaseClock::duration maxBaseTime() const { return max_base_time; }
	void setNextMaxRelTime(const duration& new_max_rel_time) {to_set_max_rel_time = new_max_rel_time;}
	duration maxRelTime() const { return max_rel_time; }

	float lastRate() const { return last_rate; }

	time_point tick()
	{
		typename BaseClock::time_point current_base_time = baseclock.now();
		typename BaseClock::duration elapsed_base_time = current_base_time - last_update_base_time;

		if(! elapsed_base_time.count()) return last_update_rel_time;

		/*
		if(elapsed_base_time<microseconds(0))
		{
			WarnErrReporter::timeBackwardErr("found a time elapsed <0");
			elapsed_base_time=microseconds(0);
		}
		*/

		//duration that can elapse in base-time is constraint
		//so discontiguousness in base time (from reltime perspective) can be midigated
		typename BaseClock::duration constraint_elapsed_base_time=std::min(elapsed_base_time, max_base_time);

		duration elapsed_rel_time = duration_cast<duration>(constraint_elapsed_base_time * target_rate);

		//duration that can elapse in relative-time is contstraint
		//for requirements given by the simulation might include constraining the time to simulate
		elapsed_rel_time=std::min(elapsed_rel_time, max_rel_time);

		last_rate = elapsed_rel_time/elapsed_base_time;

		last_update_rel_time = last_update_rel_time + elapsed_rel_time;
		last_update_base_time = current_base_time;

		target_rate = to_set_target_rate;
		max_base_time = to_set_max_base_time;
		max_rel_time = to_set_max_rel_time;

		return last_update_rel_time;
	}

	time_point now() const
	{
		typename BaseClock::time_point current_base_time = baseclock.now();
		typename BaseClock::duration elapsed_base_time= current_base_time - last_update_base_time;
		elapsed_base_time=elapsed_base_time>max_base_time?max_base_time:elapsed_base_time;

		if(! elapsed_base_time.count()) return last_update_rel_time;

		/*
		if(elapsed_base_time<microseconds(0))
		{
			WarnErrReporter::timeBackwardErr("found a time elapsed <0");
			elapsed_base_time=microseconds(0);
		}
		*/

		duration elapsed_rel_time = duration_cast<duration>(elapsed_base_time * last_rate);
		elapsed_rel_time=elapsed_rel_time>max_rel_time?max_rel_time:elapsed_rel_time;
		return last_update_rel_time + elapsed_rel_time;
	}
};

#endif /* RELATIVECLOCK_HPP */
