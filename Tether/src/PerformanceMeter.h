#pragma once

#include "SFML/Window.hpp"
#include <string>

#define FLAG_NOW 1
#define FLAG_RECENTAVG 2
#define FLAG_SPIKES 4
#define FLAG_ALL_TIME_MAX 8
#define FLAG_ALL_TIME_MIN 16
#define FLAG_TOTALAVG 32

class PerformanceMeter
{
public:

	//collects data about the performance of a loop with [stepCount] internal steps
	PerformanceMeter(int stepCount);
	~PerformanceMeter();

	/*
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



	//return: if measurement exceeded maximum time
	bool registerTime(int stepID);//TODO add more comments

	//clear data about the maximum ever measured time for each step
	void clearMax();

	//clear data about the minimum ever measured time for each step
	void clearMin();

	//returns the last measured time for the given step (in us)
	int getTime(int stepID);

	//only works after having collected a lot of data, (time in us)
	float getRecentAVG(int stepID);

	//=max measured but with a half life time
	float getSpikes(int stepID);

	//returns maximum time ever measured for the given step (in us)
	int getMaxMeasured(int stepID);

	//returns minimum time ever measured for the given step (in us)
	int getMinMeasured(int stepID);

	void setMaxTolerated(int stepID, int us);

	//gets inaccurate over time because of limited float precision //TODO add double option
	float getTotalAVG(int stepID);
	void clearTotalAVG();

	//give a name to a step so this class can format a string for you
	void setName(std::string name, int  stepID);

	//returns a string describing the performance of the given step, ready to print
	std::string getInfo(int stepID,int infoFlags);

	//reset the timer to 0
	void reset();

	//clears all collected info
	void clear();



	int getStepCount();

	void setSpikeHalfLifeTime(float seconds);

private:
	
	void exceededMax(int stepID);

	sf::Clock clock; //TODO replace

	int n;

	std::string * names; //TODO format shit (+' ')

	int * time;
	int * maxTolerated;
	int * maxMeasured;
	int * minMeasured;
	int * runs;
	float * avgRecent;
	float * sum;//TODO add double option
	float * spikes;

	float spikeHalfLifeInv = 1.0f/10000000;
	float spikeMultiplier=0.00000005f;
public:

	float roundtriptime;//addes as another time that can be queried (ID=n-1)//maybe put back to private
	int roundtripUpdateIndex;//at which of the registerTime calls the roundtriptime averages, sipkes etc. should be updated

	//avgRecent[stepID] = avgRecent[stepID] * (1 - avgWeight) + t * avgWeight;
	float avgWeight = 0.01f;

	//use faster algorithms (e.g. for applying the half life of spikes (inaccurate))
	bool useFastApproximation = true;//TODO apply to other calculations
};

