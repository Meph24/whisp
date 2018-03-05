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
#include "Tickable.h"
#include "MatrixLib.h"
class Entity: public Tickable
{
public:
	vec3 pos;
	vec3 v;
	vec3 aabbOlow;//AABB offset to pos on the low end
	vec3 aabbOhigh;//AABB offset to pos on the high end//TODO best way?

	virtual void draw()=0;
	virtual void onAABBintersect(Entity * other);
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
