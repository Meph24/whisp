/*
 * BenchEntityMaster.h
 *
 *  Created on:	30.04.2020
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_BENCHENTITYMASTER_H_
#define SRC_BENCHENTITYMASTER_H_

#include "BenchAsymMaster.h"
#include "Entity.h"

class BenchEntityMaster: public Entity, public BenchAsymMaster
{
public:
	BenchEntityMaster();
	virtual ~BenchEntityMaster();
};

#endif /* SRC_BENCHENTITYMASTER_H_ */
