/*
 * ZombieTree.h
 *
 *  Created on:	28.12.2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_ZOMBIETREE_H_
#define SRC_ZOMBIETREE_H_

#include "Entity.h"

#include "Spacevec.h"

class ITexture;

class Zombie_Tree: public Entity
{
	void drawLog();
	void drawLeaves();
	float dLeaves = 6;// >=1
	float rootSize = 8;
public:
	float d;
	float h;

	Zombie_Tree(spacevec position);
	Zombie_Tree(spacevec position, float diameter, float height, float diameterLeaves);
	~Zombie_Tree();
	virtual void draw(const SimClock::time_point& t,Frustum * viewFrustum,IWorld& iw, Perspective&);
	virtual void tick(const SimClock::time_point& t,TickServiceProvider * tsp);


	void serialize(sf::Packet& p,bool complete);
	void deserialize(sf::Packet& p,SyncableManager & sm);
	Zombie_Tree(sf::Packet p,SyncableManager& sm);//deserialize constructor
	void getOwnedSyncables(std::vector<Syncable *> collectHere);
	void getReferencedSyncables(std::vector<Syncable *> collectHere);
	u32 getClassID();
};

#endif /* SRC_ZOMBIETREE_H_ */
