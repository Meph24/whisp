/*
 * POCmain.cpp
 *
 *  Created on:	Aug 10, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "POCmain.h"

#include "POCphysics.h"
#include <GL/glew.h>

POCmain::POCmain(int resX,int resY):
em(),cam(resX,resY),pm(1),needsInit(true)
{
	em.registerAction(2048,MAPPER_MODE_ADD_RESETM,-3,0,-resX/2);
	em.registerAction(2049,MAPPER_MODE_ADD_RESETM,-3,1,-resY/2);
	em.registerAction(2050,MAPPER_MODE_ADD,0,2,0);
	em.registerAction(2052,MAPPER_MODE_REPLACE,0,3,0);
	em.registerAction(2048,MAPPER_MODE_ADD_RESETM,3,100,-resX/2);
	em.registerAction(2049,MAPPER_MODE_ADD_RESETM,3,100,-resY/2);
}

POCmain::~POCmain()
{
	// TODO Auto-generated destructor stub
}
#include <iostream>

void POCmain::frame()
{
	pm.registerTime(0);
	float t=pm.getTime(0)/1000000.0f;

	if(needsInit) init();
	float sens=0.001;//TODO relative to screen size
	float sensR=0.001;
	cam.posX-=cam.zoom*sens*em.getStatusAndReset(0);
	cam.posY+=cam.zoom*sens*em.getStatusAndReset(1);
	int steps=em.getStatusAndReset(2);
	if(steps) printf("steps: %d",steps);
	while(steps>0)
	{
		cam.zoom*=2;
		steps--;
	}
	while(steps<0)
	{
		cam.zoom/=2;
		steps++;
	}
	cam.gamma+=sensR*em.status[3]*t;

	//TODO timing?
	//TODO timing problem: float is not accurate enough:
	//1µs after 8 seconds
	//1ms after 2h20min
	//1s after 97 days

	cam.applyFresh();
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glColor3f(0,1,0);
	glVertex3f(0.1,0.1,0);
	glVertex3f(-0.1,0.1,0);
	glVertex3f(-0.1,-0.1,0);
	glVertex3f(0.1,-0.1,0);

	glEnd();
	//TODO other things
}

#include <fstream>
void POCmain::init()
{
	if(!needsInit) return;
	needsInit=false;

	MPoint p1=MPoint();
	p1.pos={-4,-0.86602540378443864676372317075294f,0};
	p1.vel={1,0,0};

	MPoint p2=MPoint();
	p2.pos={4,0.86602540378443864676372317075294f,0};
	p2.vel={-1,0,0};

	POCphysics phys=POCphysics();
	float t=phys.collides(p1,p2,2);
	std::cout<<"returned time: "<<t<<std::endl;



	/*int steps=40;
	float * arr=new float[steps];
	for(int i = 0; i<steps; i++)
	{
		p1.pos={-4,(-1.0f/steps)*i,0};
		p1.vel={1,0,0};
		p2.pos={4,(1.0f/steps)*i,0};
		p2.vel={-1,0,0};
		t=phys.co
	}
	for(int i = 0; i<steps; i++)
	{
		int count=(int)((arr[i]-3)*200);
		flt wertA=((float)i/steps);
		flt wertB=sqrt(1-wertA*wertA);
		for(int j = 0; j<count; j++)
		{
			std::cout<<'#';
		}
		std::cout<<"      "<<arr[i]<<"    "<<(arr[i]+wertB)<<std::endl;
	}
	delete arr;
*/

	//TODO init stuff, NO reinit!!!

}


void POCmain::event(EventHandler::event e)
{
	//std::cout<<"status="<<em.status<<std::endl;
	//std::cout<<"ID="<<em.ID<<std::endl;
	//std::cout<<"m="<<em.m<<std::endl;
	//std::cout<<"arrSize="<<em.arrSize<<std::endl;
	//std::cout<<"stSize="<<em.stSize<<std::endl;
	em.event(e);

	std::cout<<"Event arrived: ID="<<e.ID<<" value="<<e.value<<std::endl;
	//std::cout<<"status[0]="<<em.status[0]<<" status[1]="<<em.status[1]<<std::endl;
	//std::cout<<"condition[0]="<<em.m[0].condition<<" condition[1]="<<em.m[1].condition<<std::endl;
}
