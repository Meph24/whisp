/*
 * BoxSort.h
 *
 *  Created on:	29.03.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_BOXSORT_H_
#define SRC_BOXSORT_H_

#include <vector>
#include <type_traits>

#include "InteractFilterEntry.h"
#include "BoxSortTreeNode.h"

template<typename PhysicsIF>
class BoxSort
{
public:
	unsigned long long potentialChecks=0;
	unsigned long long checks=0;
	unsigned long long intersections=0;
	std::vector<InteractFilterEntry<PhysicsIF>>& registered;//ownership belongs to the specific filter (asym/sym, not this class)

	BoxSortTreeNode<PhysicsIF> * root;

	BoxSort(std::vector<InteractFilterEntry<PhysicsIF>>& Registered);
	virtual ~BoxSort();

	void buildTree(unsigned int partnerEntities,bool verbose);

	template<typename QueryIF=PhysicsIF,bool queryIsMaster=false>
	void query(InteractFilterEntry<QueryIF> qe,TickServiceProvider& tsp);//automatically excludes "me" from results

private:
	template<typename QueryIF,bool queryIsMaster>
	void query(InteractFilterEntry<QueryIF> qe,BoxSortTreeNode<PhysicsIF> * node,TickServiceProvider& tsp);//automatically excludes "me" from results

	template<typename QueryIF,bool queryIsMaster>
	void check(InteractFilterEntry<QueryIF> qe,InteractFilterEntry<PhysicsIF> entry,TickServiceProvider& tsp);//does nothing if me==entry
};

template<typename PhysicsIF>
inline BoxSort<PhysicsIF>::BoxSort(std::vector<InteractFilterEntry<PhysicsIF>>& Registered):
registered(Registered),root(0){}

template<typename PhysicsIF>
inline BoxSort<PhysicsIF>::~BoxSort()
{
	if(root) delete root;
}
#include <iostream>
template<typename PhysicsIF>
inline void BoxSort<PhysicsIF>::buildTree(unsigned int partnerEntities,bool verbose)
{
	if(verbose) std::cout<<std::endl<<"potential checks:"<<potentialChecks<<std::endl;
	if(verbose) std::cout<<"checks:"<<checks<<std::endl;
	if(verbose) std::cout<<"intersections:"<<intersections<<std::endl;
	checks=0;
	intersections=0;
	potentialChecks=0;
	//cleanup
	if(root) delete root;
	root=0;

	//root must not be initialized with 0 elements
	if(registered.empty()) return;

	//performance measures
	potentialChecks+=registered.size()*partnerEntities;

	//let tree build itself
	root=new BoxSortTreeNode<PhysicsIF>(registered);
}
template<typename PhysicsIF>
template<typename QueryIF,bool queryIsMaster>
inline void BoxSort<PhysicsIF>::query(InteractFilterEntry<QueryIF> qe,TickServiceProvider& tsp)
{
	if(root==0) return;
	query<QueryIF,queryIsMaster>(qe,root,tsp);
}

template<typename PhysicsIF>
template<typename QueryIF,bool queryIsMaster>
inline void BoxSort<PhysicsIF>::check(InteractFilterEntry<QueryIF> qe,InteractFilterEntry<PhysicsIF> entry,TickServiceProvider& tsp)
{
	checks++;
	if(!(qe.e.e->bb.doesIntersect(entry.e.e->bb))) return;
	if constexpr(std::is_same<QueryIF, PhysicsIF>::value)
	{
		if(qe.e.e==entry.e.e) return;
	}
	intersections++;

	if constexpr(queryIsMaster)
		qe.e.pIF->interact(qe.e.e,entry.e,qe.time,tsp);//TODO rethink "time" parameter
	else
		entry.e.pIF->interact(entry.e.e,qe.e,entry.time,tsp);
}

template<typename PhysicsIF>
template<typename QueryIF, bool queryIsMaster>
void BoxSort<PhysicsIF>::query(InteractFilterEntry<QueryIF> qe,BoxSortTreeNode<PhysicsIF>* node,TickServiceProvider& tsp)
{
	//checking node
	check<QueryIF,queryIsMaster>(qe,*(node->e),tsp);

	//checking left subtree
	if(node->lower)
	if(node->lowerBounds.doesIntersect(node->dim,qe.e.e->bb))
	{
		query<QueryIF,queryIsMaster>(qe,node->lower,tsp);
	}

	//checking right subtree
	if(node->higher)
	if(node->higherBounds.doesIntersect(node->dim,qe.e.e->bb))
	{
		query<QueryIF,queryIsMaster>(qe,node->higher,tsp);
	}
}

#endif /* SRC_BOXSORT_H_ */
