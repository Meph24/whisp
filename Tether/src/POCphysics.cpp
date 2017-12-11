/*
 * POCphysics.cpp
 *
 *  Created on:	Jul 7, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "POCphysics.h"

POCphysics::POCphysics()
{
	// TODO Auto-generated constructor stub

}

POCphysics::~POCphysics()
{
	// TODO Auto-generated destructor stub
}

float POCphysics::collides(MPoint a,MPoint b,flt radius)
{
	/*
	* find first time instant where
	* (relPos+relV*time)²<radius²
	*/
	vec3 relPos=b.pos-a.pos;
	vec3 relV=b.vel-a.vel;
	flt r2=radius*radius;
	std::cout<<"relPos: "<<relPos<<std::endl;
	std::cout<<"relV: "<<relV<<std::endl;
	std::cout<<"r2: "<<r2<<std::endl;
	vec3 relPV2=(relPos*relV);
	flt relPos2=relPos.lengthSq();
	flt relV2=relV.lengthSq();
	std::cout<<"relPV2: "<<relPV2<<std::endl;
	std::cout<<"relPos2: "<<relPos2<<std::endl;
	std::cout<<"relV2: "<<relV2<<std::endl;
	if(relV2==0) return -1;
	flt rr2=relPos2-r2;
	flt beta=relPV2.x+relPV2.y+relPV2.z;
	flt d=beta*beta-relV2*rr2;//wikipedia Mitternachtsformel
	if(d<0) return -1;//TODO Wurzel?!
	d=sqrt(d);
	flt x1=(-beta+d)/relV2;
	flt x2=(-beta-d)/relV2;
	return x1<x2?x1:x2;
}
