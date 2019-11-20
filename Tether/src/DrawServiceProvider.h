/*
 * DrawServiceProvider.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWSERVICEPROVIDER_H_
#define SRC_DRAWSERVICEPROVIDER_H_

#include"glmutils.hpp"
using glm::vec3;
#include "Timestamp.h"

class Drawable;
class Frustum;
class ChunkManager;
class Graphics2D;
class ICamera3D;
class ITexture;

#include <vector>
#include <algorithm>
#include <utility>

//use with caution: when using this, code sections outside of these regions are called twice
#define TRANSPARENT_SECTION_DO_LATER(priority) \
	if(!dsp->isTransparentPass) dsp->registerTransparentCallback(priority,this); \
	if(dsp->isTransparentPass)

#define OTHER_SECTION \
	else




class DrawServiceProvider
{
protected:
	std::vector<std::pair<float,Drawable *> > callbackList;

	ICamera3D * cam;
	void setCam(ICamera3D * myCam);
//	bool depthStatus;
public:
	Graphics2D * g;//the default graphics object
	bool isTransparentPass=false;


	DrawServiceProvider();//set camera after init!!!!!
	virtual ~DrawServiceProvider();

	vec3 getForwardViewVec();//the direction where the current view is looking
	vec3 getUpViewVec();//the direction where up is in the current view

	vec3 getCamPos();

	//priority 0-1, higher prio GUI elements cover lower prio ones when they overlap
	void transformViewToGUI(float priority=0.5f);//call revertView after draw calls finished!!!; must not be activated multiple times within any instances at the same time (call revertView in between!)
	void revertView();

	float getAspectRatio();

	virtual bool reinitGraphics();//returns if graphics must be reinitialized at beginning of draw() (do stuff like reconfigure aspect ratio, ...)
	virtual ITexture * suggestFont();//returns 0 if no suggestion is made

	void registerTransparentCallback(float priority,Drawable * callbackRequester);
	void doTransparentCallbacks(Timestamp t,Frustum * viewFrustum,ChunkManager* cm);


};

#endif /* SRC_DRAWSERVICEPROVIDER_H_ */
