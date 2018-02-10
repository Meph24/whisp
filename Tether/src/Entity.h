/*
 * Entity.h
 *
 *  Created on:	18.01.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_ENTITY_H_
#define SRC_ENTITY_H_
class TickServiceProvider;
class Entity;
#include "Tickable.h"
#include "MatrixLib.h"
class Entity: public Tickable
{
public:
	vec3 pos;
	vec3 v;

	virtual void draw()=0;
	Entity();
	virtual ~Entity();
};





//12.03.2017
/*
#include "vec3.h"
#include "ID.h"
class Entity
{
public:

	ID UID;

	vec3 pos;
	vec3 speed;

	bool markDelete;

	virtual void tick(Entity * past,Entity * future,float sec)=0;

	Entity();
	virtual ~Entity();
};
*/
#endif /* SRC_ENTITY_H_ */
