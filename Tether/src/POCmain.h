/*
 * POCmain.h
 *
 *  Created on:	Aug 10, 2017
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_POCMAIN_H_
#define SRC_POCMAIN_H_

#include "EventHandler.h"
#include "EventMapper.h"
#include "CameraManual2D.h"
class POCmain
{
	EventMapper em;
	CameraManual2D cam;
	PerformanceMeter pm;
	bool needsInit;
public:
	void init();//call after OpenGL init and before first frame, if not called before first frame, it gets called in first frame internally

	void frame();
	void event(EventHandler::event e);


	POCmain(int resX,int resY);
	~POCmain();
};

#endif /* SRC_POCMAIN_H_ */
