#pragma once

#include "SFML\Window.hpp"
#include <string>

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
	void setMaxTolerated(int stepID, int us);

private:

	sf::Clock clock;
	int n;
	//std::string * stuff; //TODO implement
	int * time;
	int * maxTolerated;
	int * maxMeasured;
	float * avg;
	float avgWeight = 0.01f;

	void exceededMax(int stepID);
};

