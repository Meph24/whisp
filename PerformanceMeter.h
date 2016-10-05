#pragma once

#include "SFML\Window.hpp"
#include <string>

#define FLAG_NOW 1
#define FLAG_AVG 2
#define FLAG_SPIKES 4
#define FLAG_ALL_TIME_MAX 8
#define FLAG_ALL_TIME_MIN 16

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
	bool registerTime(int stepID);

	//clear data about the maximum ever measured time for each step
	void clearMax();

	//clear data about the minimum ever measured time for each step
	void clearMin();

	//returns the last measured time for the given step (in us)
	int getTime(int stepID);

	//only works after having collected a lot of data, (time in us)
	float getAVG(int stepID);

	//=max measured but with a half life time
	float getSpikes(int stepID);

	//returns maximum time ever measured for the given step (in us)
	int getMaxMeasured(int stepID);

	//returns minimum time ever measured for the given step (in us)
	int getMinMeasured(int stepID);

	void setMaxTolerated(int stepID, int us);

	//give a name to a step so this class can format a string for you
	void PerformanceMeter::setName(std::string name, int  stepID);

	//returns a string describing the performance of the given step, ready to print
	std::string getInfo(int stepID,int infoFlags);

	//reset the timer to 0
	void reset();

	//clears all collected info
	void clear();




	void setSpikeHalfLifeTime(float seconds);

private:
	
	void exceededMax(int stepID);

	sf::Clock clock; //TODO replace

	int n;

	std::string * names;

	int * time;
	int * maxTolerated;
	int * maxMeasured;
	int * minMeasured;
	float * avg;
	float * spikes;


	float spikeHalfLifeInv = 1.0f/10000000;
	float spikeMultiplier=0.00000005f;
public:

	//avg[stepID] = avg[stepID] * (1 - avgWeight) + t * avgWeight;
	float avgWeight = 0.01f;

	//use a faster algorithm for applying the half life of spikes (inaccurate)
	bool useFastApproximation = true;
};
