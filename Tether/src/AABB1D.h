/*
 * AABB1D.h
 *
 *  Created on:	04.04.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_AABB1D_H_
#define SRC_AABB1D_H_

#include "Spacevec.h"
#include "AABB.h"

class AABB1D
{
public:
	spacelen low;
	spacelen high;


	bool doesIntersect(spacelen spacevec::* dim,AABB bb);
	bool doesIntersect(spacelen spacevec::* dim,AABB * bb);

	void extend(spacelen spacevec::* dim,AABB bb);
	void extend(spacelen spacevec::* dim,AABB * bb);


	AABB1D();//uninitialized
	AABB1D(spacelen pt);
	virtual ~AABB1D();
};

#endif /* SRC_AABB1D_H_ */
