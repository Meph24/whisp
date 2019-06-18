/*
 * DrawServiceProvider.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWSERVICEPROVIDER_H_
#define SRC_DRAWSERVICEPROVIDER_H_
#include "ICamera3D.h"
class DrawServiceProvider
{
protected:

	ICamera3D * cam;
	void setCam(ICamera3D * myCam);
//	bool depthStatus;
public:


	DrawServiceProvider();//set camera after init!!!!!
	virtual ~DrawServiceProvider();

	vec3 getForwardViewVec();//the direction where the current view is looking
	vec3 getUpViewVec();//the direction where up is in the current view

	void transformViewToGUI();//call revertView after draw calls finished!!!; must not be activated multiple times within any instances at the same time (call revertView in between!)
	void revertView();

};

#endif /* SRC_DRAWSERVICEPROVIDER_H_ */
