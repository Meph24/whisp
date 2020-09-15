/*
 * BenchmarkManager.h
 *
 *  Created on:	04.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHMARKMANAGER_H_
#define SRC_BENCHMARKMANAGER_H_

#include "Tickable.h"

#include "PerformanceMeter.h"
#include "BenchAsymMaster.h"
#include "BenchAsymSlave.h"
#include "BenchSym.h"
#include "BenchScenario.h"

class IWorld;

template<typename PhysicsIF>
class InteractFilterAlgoSym;

template<typename MasterIF,typename SlaveIF>
class InteractFilterAlgoAsym;

#include <vector>

class BenchmarkManager: public Tickable
{
	IWorld * world;
	PerformanceMeter pm;

	void nextPhase();
	 void nextAlgo();
	  void nextScenario();
	   void nextParam();

	void evaluateData(TickServiceProvider * tsp);

	void clearEntities();

	void init(IWorld * w);

	void setAlgos(unsigned int indx);
	std::vector<InteractFilterAlgoSym<BenchSym>*> symAlgos;
	std::vector<InteractFilterAlgoAsym<BenchAsymMaster,BenchAsymSlave>*> asymAlgos;
	std::vector<std::string> symAlgoNames;
	std::vector<std::string> asymAlgoNames;
	std::vector<BenchScenario *> symScenarios;
	std::vector<BenchScenario *> asymScenarios;
	BenchScenario * active=0;
	std::vector<BenchScenario *> * activeSet=0;

	unsigned int benchmarkPhase=0;//values defined in phase getters, can be incremented for next phase
	std::vector<float> repeatTimes;
	unsigned int scenarioNum=0;
	unsigned int algoIndx=0;

	void addScenario(BenchScenario * s);

	unsigned int getAlgoOverflowIndx();

	void addAsymAlgo(InteractFilterAlgoAsym<BenchAsymMaster,BenchAsymSlave>* algo, std::string name);
	void addSymAlgo(InteractFilterAlgoSym<BenchSym>* algo, std::string name);

	float lastTimeSeconds=0;
public:
	unsigned int repeats=16;//how often is the same experiment repeated

	void tick(const SimClock::time_point& next_tick_begin,TickServiceProvider * tsp);//call before ticks, when it is safe to modify required values
	void notifyTickEnded();

	bool isInactivePhase();
	bool isSymPhase();
	bool isAsymPhase();
	bool isFinalPhase();
	std::string getCurrentAlgorithmName();

	BenchmarkManager(IWorld * World);
	virtual ~BenchmarkManager();
};

#endif /* SRC_BENCHMARKMANAGER_H_ */
