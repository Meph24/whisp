/*
 * BenchmarkTest.cpp
 *
 *  Created on:	01.10.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "BenchmarkTest.h"

#include "Entity.h"


BenchmarkTest::BenchmarkTest()
{}

void BenchmarkTest::registerEntity(Entity* e)
{
	bbs.push_back(&(e->bb));
}

void BenchmarkTest::reset()
{
	bbs.clear();
}

BenchmarkTest::~BenchmarkTest()
{}

std::ostream& operator <<(std::ostream& os, const BenchmarkTest& bt)
{
	if(bt.bbs.empty()) return os;
	AABB totalExtent=*(bt.bbs[0]);
	for(AABB * bb:bt.bbs)
	{
		totalExtent.extend(bb);
	}
	spacevec centerPoint=totalExtent.low+(totalExtent.high-totalExtent.low)*0.5f;
	int centerCount=0;
	for(AABB * bb:bt.bbs)
	{
		if(bb->contains(centerPoint)) centerCount++;
	}
	return os<<'\n'<<"Total Spawn Area:"<<totalExtent<<"\nEntities: "<<bt.bbs.size()<<"\ncenter density: "<<centerCount<<std::endl;
}
