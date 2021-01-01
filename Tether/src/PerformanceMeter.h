/*
 * PerformanceMeter.h
 *
 *  Created on:	before 18.03.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_PERFORMANCEMETER_H_
#define SRC_PERFORMANCEMETER_H_

#include "Drawable.h"

class Graphics2D;

#include <SFML/Window.hpp>
#include <string>
#include <deque>
#include "FloatSeconds.hpp"

#define FLAG_NOW 1
#define FLAG_RECENTAVG 2
#define FLAG_SPIKES 4
#define FLAG_ALL_TIME_MAX 8
#define FLAG_ALL_TIME_MIN 16

class PerformanceMeter: public Drawable
{
	struct StepData
	{
		std::deque<float> times;
		std::string name;
		void registerTime(float time);
		float getLastTime();
		float getAVG();
		float getRecentMax();
		void clearMin();
		void clearMax();
		void clearAllTimeData();
		void shrinkTimeBuffer(int by);
		float min;
		float max;
		void draw(std::deque<float>& roundtrip,Graphics2D * g);
		//TODO reenable float maxTolerated;
	};
public:
	struct SingleTimer
	{
		PerformanceMeter * pm=0;//TODO make methods safe for null pointer
		int myID=-1;
		void registerTime();
		void setAsRoundtripMarker(std::string roundtripName);
		StepData& getData();
	};

	//collects data about the performance of a loop with a certain number of internal steps
	PerformanceMeter(FloatSeconds historyLength,FloatSeconds WarmupTime,bool BenchmarkMode=false);
	~PerformanceMeter();

	bool benchmarkMode;//if enabled, tries to exclude time spent within PerformanceMeter

	/*
	//TODO update
	usage:
	PerformanceMeter pm(3);
	//[
		//optional:
		doStuffThatNeedsTime();
		pm.reset();
	//]
	while(condition)
	{
		doThingA();
		pm.registerTime(0);
		if(something)
		{
			doThingB();
			pm.registerTime(1);
		}
		doThingC();
		pm.registerTime(2);
	}

	cout<<"most time ever spent doing thing A: "<<(pm.getMaxMeasured(0)/1000.0f)<<"ms"<<endl;
	
	*/



	//clear data about the maximum ever measured time for each step
	void clearMax();

	//clear data about the minimum ever measured time for each step
	void clearMin();

	//returns a string describing the performance of the given step, ready to print
	//works for a maximum time <100s
	std::string getInfo(int stepID,int infoFlags);
	int getStepCount();

	//draws graph of all time information
	void draw(const SimClock::time_point& draw_time, Frustum * viewFrustum, IWorld& iw, Perspective& perspective);

	//reset the timer to 0
	void resetTimer();

	//clears all collected info
	void clearTimeData();

	SingleTimer createTimestep(std::string name);
private:

	void registerTime(int stepID);//TODO add more comments

	std::string getInfo(StepData& data,int infoFlags);

	float warmupTime;

	sf::Clock clock; //TODO replace

	std::vector<StepData> stepData;
	StepData roundtrip;

	float roundtripTime=0;

	float historyLen;
	int roundtripIndex=-1;//-1 means none specified

};


#endif /* SRC_PERFORMANCEMETER_H__ */
