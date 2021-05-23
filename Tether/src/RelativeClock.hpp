#ifndef RELATIVECLOCK_HPP
#     define RELATIVECLOCK_HPP

#include <chrono>

#include "WarnErrReporter.h"
#include "NestedSyncable.h"
#include <SFML/Network.hpp>
#include <iostream>
#include "FloatSeconds.hpp"

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
struct RelativeClock: public NestedSyncable
{
	//std::chrono typedefs
	typedef typename BaseClock::duration duration;
	typedef typename BaseClock::duration::rep rep;
	typedef typename BaseClock::duration::period period;
	typedef std::chrono::time_point<RelativeClock<BaseClock>, duration> time_point;
	constexpr static bool is_steady = BaseClock::is_steady;

	const BaseClock& baseclock;

private:
	typename	BaseClock::duration		max_base_time;
				duration				max_rel_time;
public:

	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);

private:
	typename	BaseClock::time_point	last_update_base_time;
				time_point				last_update_rel_time;

	float last_rate; //rate the last time before it was changed
	float target_rate; //the rate of relative timeflow that is aimed to be achieved
	float to_set_target_rate;
	typename BaseClock::duration to_set_max_base_time;
	duration to_set_max_rel_time;

	RelativeClock(const BaseClock& baseclock, float target_rate, typename BaseClock::duration max_base_time,
			duration max_rel_time,bool bullshit)
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

public:
	RelativeClock(const BaseClock& baseclock, float target_rate,duration max_base_time,duration max_rel_time,sf::Packet* p=nullptr);//constructor with optional deserialization
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

	time_point sim(const typename BaseClock::time_point& Current_base_time,const time_point& Last_update_rel_time,const typename BaseClock::time_point& Last_update_base_time) const
	{
		typename BaseClock::duration elapsed_base_time= Current_base_time - Last_update_base_time;
		elapsed_base_time=elapsed_base_time>max_base_time?max_base_time:elapsed_base_time;

		if(! elapsed_base_time.count()) return Last_update_rel_time;

		/*
		if(elapsed_base_time<microseconds(0))
		{
			WarnErrReporter::timeBackwardErr("found a time elapsed <0");
			elapsed_base_time=microseconds(0);
		}
		*/

		duration elapsed_rel_time = duration_cast<duration>(elapsed_base_time * last_rate);
		elapsed_rel_time=elapsed_rel_time>max_rel_time?max_rel_time:elapsed_rel_time;
		return Last_update_rel_time + elapsed_rel_time;
	}

	time_point now() const
	{
		typename BaseClock::time_point current_base_time = baseclock.now();
		/*typename BaseClock::duration elapsed_base_time= current_base_time - last_update_base_time;
		elapsed_base_time=elapsed_base_time>max_base_time?max_base_time:elapsed_base_time;

		if(! elapsed_base_time.count()) return last_update_rel_time;

		duration elapsed_rel_time = duration_cast<duration>(elapsed_base_time * last_rate);
		elapsed_rel_time=elapsed_rel_time>max_rel_time?max_rel_time:elapsed_rel_time;*/
		return sim(current_base_time,last_update_rel_time,last_update_base_time);
		//return last_update_rel_time + elapsed_rel_time;
	}
};

template<typename BaseClock>
inline void RelativeClock<BaseClock>::serialize(sf::Packet& p, bool complete)
{
	p<<target_rate;
	p<<last_rate;
	p<<last_update_rel_time;
}
#include "myAssert.h"
#include "protocol.hpp"
#include "sfml_packet_utils.hpp"
#include "SyncableManager.h"
template<typename BaseClock>
inline void RelativeClock<BaseClock>::deserialize(sf::Packet& p,SyncableManager& sm)
{
	float f;
	SessionClock<SFMLClock>::time_point tp;
	p >> f;
	p >> f;
	p >> tp;
	throw std::runtime_error ( "RelativeClock::deserialize() : This method is not properly implemented for the given base-clock type. The BaseClock::duration must match with transmission protocoll, therefore this method is only implemented for one type of clock. We have flushed the respective packet contents according to function specifications, but this call definitely is wrong!" );
}

template<>
inline void RelativeClock<SessionClock<SFMLClock> >::deserialize(sf::Packet& p,SyncableManager& sm)
{
	typedef SessionClock<SFMLClock> BaseClock; //for syntax consistency

	sf::Packet hackPack1;
	sf::Packet hackPack2;
	syncprotocol::udp::Header h;
	hackPack1<<h;
	size_t timestampSize=hackPack1.getDataSize();
	assert(p.getDataSize()>=timestampSize);
	hackPack2.append(p.getData(),timestampSize);
	hackPack2>>h;

    BaseClock::time_point newBaseTime = time_point_cast<BaseClock::duration>(h.client_time); //Header might be implemented by a different time_point type, therfore cast needed

	time_point newRelTime;

	p>>target_rate;
	if(sm.verbose) std::cout<<"unpacking RelativeClock: target_rate="<<target_rate<<std::endl;
	p>>last_rate;
	if(sm.verbose) std::cout<<"last_rate="<<last_rate<<std::endl;
	p>>newRelTime;
	to_set_target_rate=target_rate;
	if(sm.verbose) std::cout<<"newRelTime="<<newRelTime.time_since_epoch().count()*0.000001f<<std::endl;

	typename BaseClock::time_point current_base_time = baseclock.now();
	time_point oldT=sim(current_base_time,last_update_rel_time,last_update_base_time);
	time_point newT=sim(current_base_time,newRelTime,newBaseTime);

	duration diff;
	if(newT>oldT) diff=newT-oldT;
	else diff=oldT-newT;
	duration tolerance=20ms;
	if(diff>tolerance*last_rate)
	{
		std::cout<<"updating clock, diff="<<(float)FloatSeconds(diff)<<std::endl;
		std::cout<<"clock elapsed="<<(float)FloatSeconds(current_base_time - newBaseTime)<<std::endl;
		last_update_base_time=newBaseTime;
		last_update_rel_time=newRelTime;
	}
}

template<typename BaseClock>
inline RelativeClock<BaseClock>::RelativeClock(const BaseClock& Baseclock,
		float Target_rate, duration Max_base_time, duration Max_rel_time,
		sf::Packet* p)
		: RelativeClock<BaseClock>(Baseclock,Target_rate,Max_base_time,Max_rel_time,true)
{
	if(p)
	{
		max_base_time=1h;
		max_rel_time=1h;
		//std::cout<<"max_base_time="<<(float)FloatSeconds(max_base_time)<<std::endl;
		*p>>target_rate;
		*p>>last_rate;
		*p>>last_update_rel_time;

		to_set_target_rate=target_rate;
		to_set_max_base_time=max_base_time;
		to_set_max_rel_time=max_rel_time;
	}
}

#endif /* RELATIVECLOCK_HPP */
