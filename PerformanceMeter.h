#pragma once

#include "SFML\Window.hpp"
#include <string>

#define FLAG_NOW 1
#define FLAG_AVG 2
#define FLAG_SPIKES 4
#define FLAG_ALL_TIME_MAX 8

class PerformanceMeter
{
public:
	PerformanceMeter(int count);
	~PerformanceMeter();

	//return: exceeded maximum time
	bool registerTime(int stepID);
	void clearMax();
	int getMaxMeasured(int stepID);
	int getTime(int stepID);
	float getAVG(int stepID);
	float getSpikes(int stepID);
	void setMaxTolerated(int stepID, int us);
	void PerformanceMeter::setName(std::string name, int  stepID);
	std::string getInfo(int stepID,int infoFlags);
	void reset();
	void clear();
	void setSpikeHalfLifeTime(float seconds);

private:
	
	void exceededMax(int stepID);

	sf::Clock clock; //TODO replace
	int n;

	std::string * names; //TODO implement

	int * time;
	int * maxTolerated;
	int * maxMeasured;
	float * avg;
	float * spikes;


	float spikeHalfLifeInv = 1.0f/10000000;
	float spikeMultiplier=0.00000005f;
public:
	float avgWeight = 0.01f;
	bool useFastApproximation = true;
};

