/*
 * BoxSortTreeNode.h
 *
 *  Created on:	03.04.2020
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_BOXSORTTREENODE_H_
#define SRC_BOXSORTTREENODE_H_

#include "InteractFilterEntry.h"
#include "Spacevec.h"
#include "AABB.h"

#include "myAssert.h"

#include <vector>
#include <cstdlib>
#include <algorithm>

#include "AABB1D.h"

template<typename PhysicsIF>
class BoxSortTreeNode
{
public:
	InteractFilterEntry<PhysicsIF> * e=0;
	spacelen spacevec::* dim;
	spacelen key;
	AABB1D lowerBounds;//maximum extent of left subtree
	AABB1D higherBounds;//maximum extent of right subtree
	BoxSortTreeNode<PhysicsIF> * lower=0;//left subtree
	BoxSortTreeNode<PhysicsIF> * higher=0;//right subtree

	BoxSortTreeNode(std::vector<InteractFilterEntry<PhysicsIF>>& toSort,spacelen spacevec::* d=&spacevec::x);
	virtual ~BoxSortTreeNode();
private:
	void init(InteractFilterEntry<PhysicsIF> setE);
//	spacelen spacevec::* nextDim();
	spacelen spacevec::* getBestDim(vec3 sizeRatio);
};

template<typename PhysicsIF>
inline BoxSortTreeNode<PhysicsIF>::BoxSortTreeNode(std::vector<InteractFilterEntry<PhysicsIF> >& toSort,spacelen spacevec::* d):
dim(d)
{
	auto toSortSize=toSort.size();
	assert(toSortSize!=0);
	if(toSortSize==1)
	{
		init(toSort[0]);
		return;
	}

	//find median or estimate it
	spacelen medianValue;
	unsigned int samples=9;
	if(toSortSize<samples) samples=toSortSize;
	if(samples%2==0) samples--;
	assert(samples>0);
	assert(samples%2==1);
	assert(samples<=9);
	std::vector<unsigned int> choices(samples);
	std::vector<spacelen> keys(samples);
//	AABB totalBox;
//	spacevec sizes;
//	sizes.set0();
//	bool AABBinited=false;
	for(unsigned int i=0;i<samples;i++)
	{
		choices[i]=std::rand()%toSortSize;//TODO evaluate range
		keys[i]=toSort[choices[i]].e.e->bb.low.*dim;
		AABB curBox=toSort[choices[i]].e.e->bb;
//		if(!AABBinited)
//		{
//			AABBinited=true;
//			totalBox=curBox;
//		}
//		else
//		{
//			totalBox.extend(curBox);
//		}
//		sizes+=curBox.high-curBox.low;
	}
//	spacevec extent=totalBox.high-totalBox.low;
//	vec3 sizeRatio=extent/sizes;

    std::nth_element(keys.begin(), keys.begin() + samples/2, keys.end());
    medianValue=keys[samples/2];
    unsigned int medianIndx=-1;
	for(unsigned int i=0;i<samples;i++)
	{
		InteractFilterEntry<PhysicsIF> entry=toSort[choices[i]];
		if(entry.e.e->bb.low.*dim==medianValue)
		{
			medianIndx=i;
			break;
		}
	}
	assert(medianIndx!=(unsigned int)-1);
	init(toSort[choices[medianIndx]]);
	std::vector<InteractFilterEntry<PhysicsIF> > toLower;
	std::vector<InteractFilterEntry<PhysicsIF> > toHigher;
	bool lowerBoundsInited=false;
	bool higherBoundsInited=false;

	//remove median from vector
	toSort.erase(toSort.begin()+choices[medianIndx]);
//	toSort[choices[medianIndx]]=toSort.back();
//	toSort.pop_back();

	//divide vector into left and right subtree
	spacevec sizesL;
	sizesL.set0();
	spacevec sizesH;
	sizesH.set0();
	AABB lowerBounds3D;
	AABB higherBounds3D;
	for(InteractFilterEntry<PhysicsIF> entry: toSort)
	{
		if(entry.e.e->bb.low.*dim<medianValue)
		{
			toLower.push_back(entry);
			if(!lowerBoundsInited)
			{
				lowerBounds=AABB1D(entry.e.e->bb.low.*dim);
				lowerBounds3D=entry.e.e->bb;
				lowerBoundsInited=true;
			}
			lowerBounds.extend(dim,entry.e.e->bb);
			lowerBounds3D.extend(entry.e.e->bb);
			sizesL+=entry.e.e->bb.high-entry.e.e->bb.low;
		}
		else
		{
			toHigher.push_back(entry);
			if(!higherBoundsInited)
			{
				higherBounds=AABB1D(entry.e.e->bb.low.*dim);
				higherBounds3D=entry.e.e->bb;
				higherBoundsInited=true;
			}
			higherBounds.extend(dim,entry.e.e->bb);
			higherBounds3D.extend(entry.e.e->bb);
			sizesH+=entry.e.e->bb.high-entry.e.e->bb.low;
		}
	}
	assert(toLower.size()+toHigher.size()+1==toSortSize);
	if(!toLower.empty()) lower=new BoxSortTreeNode(toLower,getBestDim((lowerBounds3D.high-lowerBounds3D.low)/sizesL));
	if(!toHigher.empty()) higher=new BoxSortTreeNode(toHigher,getBestDim((higherBounds3D.high-higherBounds3D.low)/sizesH));
}

template<typename PhysicsIF>
inline BoxSortTreeNode<PhysicsIF>::~BoxSortTreeNode()
{
	if(lower) delete lower;
	if(higher) delete higher;
	delete e;
}

template<typename PhysicsIF>
inline void BoxSortTreeNode<PhysicsIF>::init(InteractFilterEntry<PhysicsIF> setE)
{
	assert(e==0);
	e=new InteractFilterEntry<PhysicsIF>(setE);
	key=e->e.e->bb.low.*dim;
}

template<typename PhysicsIF>
inline spacelen spacevec::*BoxSortTreeNode<PhysicsIF>::getBestDim(vec3 sizeRatio)
{
	if((sizeRatio.x>sizeRatio.y)&&(sizeRatio.x>sizeRatio.z)) return &spacevec::x;
	if(sizeRatio.y>sizeRatio.z) return &spacevec::y;
	return &spacevec::z;
}

//template<typename PhysicsIF>
//inline spacelen spacevec::* BoxSortTreeNode<PhysicsIF>::nextDim()
//{
//	if(dim==&spacevec::x) return &spacevec::y;
//	if(dim==&spacevec::y) return &spacevec::z;
//	return &spacevec::x;
//}

#endif /* SRC_BOXSORTTREENODE_H_ */
