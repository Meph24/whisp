#include "BenchmarkManager.h"

#include "IWorld.h"
#include "TickServiceProvider.h"
#include "EventMapper.hpp"
#include "EventDefines.h"
#include "InteractFilterDefaultSym.h"
#include "InteractFilterDefaultAsym.h"
#include "FilterBoxSortSym.h"
#include "FilterBoxSortAsym.h"
#include "FilterHashSym.h"
#include "FilterHashAsym.h"
#include "BenchResult.h"
#include "BenchScenarioUniform.h"
#include "BenchScenarioUniformAsym.h"
#include "BenchScenarioShotgun.h"

BenchmarkManager::BenchmarkManager(IWorld* World):
world(World),pm(2)
{
	{
		auto algo=new InteractFilterDefaultSym<BenchSym>();
		algo->verbose=false;
		addSymAlgo(algo,"naive algorithm");
	}
	{
		auto algo=new FilterBoxSortSym<BenchSym>();
		algo->verbose=false;
		addSymAlgo(algo,"box sort");
	}
	{
		auto algo=new FilterHashSym<BenchSym>();
		algo->verbose=false;
		addSymAlgo(algo,"spatial hashing");
	}

	{
		auto algo=new InteractFilterDefaultAsym<BenchAsymMaster,BenchAsymSlave>();
		algo->verbose=false;
		addAsymAlgo(algo,"naive algorithm");
	}
	{
		auto algo=new FilterHashAsym<BenchAsymMaster,BenchAsymSlave>();
		algo->verbose=false;
		addAsymAlgo(algo,"spatial hashing");
	}
	{
		auto algo=new FilterBoxSortAsym<BenchAsymMaster,BenchAsymSlave>(MASTER_ONLY);
		algo->verbose=false;
		addAsymAlgo(algo,"box sort (master only)");
	}
	{
		auto algo=new FilterBoxSortAsym<BenchAsymMaster,BenchAsymSlave>(SLAVE_ONLY);
		algo->verbose=false;
		addAsymAlgo(algo,"box sort (slave only)");
	}
	{
		auto algo=new FilterBoxSortAsym<BenchAsymMaster,BenchAsymSlave>(1);
		algo->verbose=false;
		addAsymAlgo(algo,"box sort (switch at 50:50)");
	}

	addScenario(new BenchScenarioUniform());
	addScenario(new BenchScenarioUniformAsym());
	addScenario(new BenchScenarioShotgun());

	activeSet=&symScenarios;
	active=(*activeSet)[0];//symmetric scenarios must contain at least 1 scenario
	assert(!symAlgos.empty());
	assert(!asymAlgos.empty());
	assert(!symScenarios.empty());
	assert(!asymScenarios.empty());
}

void BenchmarkManager::tick(const SimClock::time_point& next_tick_begin, TickServiceProvider* tsp)
{
	IWorld * w=tsp->getIWorld();
	assert(w==world);
	assert(w);
	assert(repeats>0);
	if (tsp->eMap->getStatusAndReset(STATUS_ID_BENCHMARK))
	{
		if(isInactivePhase())
		{
			init(w);//start benchmark
			assert(repeatTimes.size()==0);
			assert(algoIndx==0);
			assert(scenarioNum==0);
			assert(isSymPhase());
			assert(w->benchAlgoAsym==asymAlgos[0]);
			assert(w->benchAlgoSym==symAlgos[0]);
		}
	}
	if(isInactivePhase()) return;//if no benchmark is running, do nothing
	assert(world->benchAlgoAsym);
	assert(world->benchAlgoSym);
	assert(activeSet);
	assert(active);
	if(repeatTimes.size()>=repeats)
	{
		evaluateData(tsp);
		nextParam();
	}
	pm.registerTime(0);//start timer for benchmark
}

void BenchmarkManager::notifyTickEnded()
{
	if(isInactivePhase()) return;
	pm.registerTime(1);//stop timer for benchmark
	lastTimeSeconds=pm.getTime(1)*0.000001f;//pm uses microseconds, therefore convert to seconds
	repeatTimes.push_back(lastTimeSeconds);
}

void BenchmarkManager::nextAlgo()
{
	algoIndx++;
	if(algoIndx>=getAlgoOverflowIndx())
	{
		algoIndx=0;
		nextPhase();
	}
	setAlgos(algoIndx);
}

#include <iostream>
void BenchmarkManager::evaluateData(TickServiceProvider* tsp)
{
	BenchResult res(repeatTimes);
	unsigned long long interactions=tsp->interactionCounter/repeats;
	std::string arbitraryWhitespaces=(~(tsp->arbitraryNumber))<42?"  ":" ";//use arbitraryNumber for something so it is not optimized away
	tsp->interactionCounter=0;
	tsp->arbitraryNumber=0;
	std::cout<<"ret.append(result(currentAlgorithmName = '"<<getCurrentAlgorithmName()<<"', "<<*active<<","<<arbitraryWhitespaces<<"timeAvg = "<<res.avg<<", timeMedian = "<<res.median<<", timeMin = "<<res.min<<", timeMax = "<<res.max<<", interactions = "<<interactions<<"))"<<std::endl;
}

void BenchmarkManager::nextParam()
{
	clearEntities();
	while(active->prepareNextParameters(lastTimeSeconds))
	{
		nextScenario();
		if(isInactivePhase()) return;
	}
	//debug start
//	for(int i=0;i<2048;i++)
//	{
//		active->spawnEntities(world);
//		clearEntities();
//	}
	//debug end
	active->spawnEntities(world);
}

void BenchmarkManager::nextScenario()
{
	scenarioNum++;
	if(scenarioNum>=activeSet->size())
	{
		scenarioNum=0;
		nextAlgo();
	}
	active=(*activeSet)[0];
}

void BenchmarkManager::setAlgos(unsigned int indx)
{
	if(indx<symAlgos.size())//if index does not fit it is the wrong kind anyway, so who cares
		world->benchAlgoSym=symAlgos[indx];
	if(indx<asymAlgos.size())//if index does not fit it is the wrong kind anyway, so who cares
		world->benchAlgoAsym=asymAlgos[indx];
}


void BenchmarkManager::addScenario(BenchScenario* s)
{
	if(s->isSym()) symScenarios.push_back(s);
	else asymScenarios.push_back(s);
}

bool BenchmarkManager::isSymPhase()
{
	return benchmarkPhase==1;
}

bool BenchmarkManager::isAsymPhase()
{
	return benchmarkPhase==2;
}

bool BenchmarkManager::isInactivePhase()
{
	return benchmarkPhase==0;
}

bool BenchmarkManager::isFinalPhase()
{
	return benchmarkPhase>=3;
}

void BenchmarkManager::clearEntities()
{
	world->clearEntities();
}

void BenchmarkManager::init(IWorld * w)
{
	nextPhase();
	setAlgos(algoIndx);
	clearEntities();
	w->collideAlgo->verbose=false;
	w->projectileAlgo->verbose=false;
	w->pushAlgo->verbose=false;
	srand(42);
	active->spawnEntities(world);
}

void BenchmarkManager::nextPhase()
{
	benchmarkPhase++;
	if(isAsymPhase())
	{
		activeSet=&asymScenarios;
	}
	else
	{
		activeSet=&symScenarios;
	}
	if(isFinalPhase())
	{
		benchmarkPhase=0;
	}
}

unsigned int BenchmarkManager::getAlgoOverflowIndx()
{
	if(isSymPhase()) return symAlgos.size();
	if(isAsymPhase()) return asymAlgos.size();
	return 0;
}

void BenchmarkManager::addAsymAlgo(InteractFilterAlgoAsym<BenchAsymMaster, BenchAsymSlave>* algo,std::string name)
{
	asymAlgos.push_back(algo);
	asymAlgoNames.push_back(name+" (asymmetric variant)");
}

void BenchmarkManager::addSymAlgo(InteractFilterAlgoSym<BenchSym>* algo, std::string name)
{
	symAlgos.push_back(algo);
	symAlgoNames.push_back(name+" (symmetric variant)");
}

std::string BenchmarkManager::getCurrentAlgorithmName()
{
	if(isAsymPhase())
	{
		return asymAlgoNames[algoIndx];
	}
	else
	{
		return symAlgoNames[algoIndx];
	}
}

BenchmarkManager::~BenchmarkManager()
{
	for(auto ptr: symAlgos) delete ptr;
	for(auto ptr: asymAlgos) delete ptr;
	for(auto ptr: symScenarios) delete ptr;
	for(auto ptr: asymScenarios) delete ptr;
}

