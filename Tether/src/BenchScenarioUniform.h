/*
 * BenchScenarioUniform.h
 *
 *  Created on:	13.05.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHSCENARIOUNIFORM_H_
#define SRC_BENCHSCENARIOUNIFORM_H_

#include "BenchScenario.h"

class BenchScenarioUniform: public BenchScenario
{
	BenchScenarioParam density;//average size of object in largest dimension / average distance of center points

protected:

	template<typename T>
	void spawnUniform(unsigned int count,IWorld * w);

public:
	BenchScenarioUniform();
	virtual ~BenchScenarioUniform();
	virtual void spawnEntities(IWorld * w);
	virtual bool isSym();
};

#include "Spacevec.h"
#include "IWorld.h"
#include "BenchmarkTest.h"
template<typename T>
inline void BenchScenarioUniform::spawnUniform(unsigned int count, IWorld* w)
{
	assert(w);
	spacelen unitLength;
	unitLength.floatpart=0;
	unitLength.intpart=1;

	//BenchmarkTest test;

	float averageObjectSizeMeters=w->toMeters(unitLength)*absoluteSize;
	float averageDistanceMeters=averageObjectSizeMeters/density;
	float spawnAreaSize=averageDistanceMeters*cbrt(objectCount);

	for(unsigned int i=0;i<count;i++)
	{
		vec3 positionVec(randomFloat(),randomFloat(),randomFloat());
		positionVec*=spawnAreaSize;

		vec3 sizeVec(randomFloat(),randomFloat(),randomFloat());
		float largest=sizeVec.x;
		if(sizeVec.y>largest) largest=sizeVec.y;
		if(sizeVec.z>largest) largest=sizeVec.z;
		sizeVec/=largest;//largest extent is now always 1

		float sizeMeters=randomFloat()*2*averageObjectSizeMeters;
		sizeVec*=sizeMeters;
		T * e=new T(w->fromMeters(sizeVec),w->fromMeters(positionVec));
		//test.registerEntity(e);
		w->requestEntitySpawn(e);
	}
	//std::cout<<test;
}

#endif /* SRC_BENCHSCENARIOUNIFORM_H_ */
