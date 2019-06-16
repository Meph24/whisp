/*
 * DrawServiceProvider.h
 *
 *  Created on:	Apr 9, 2018
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_DRAWSERVICEPROVIDER_H_
#define SRC_DRAWSERVICEPROVIDER_H_

class DrawServiceProvider
{
public:
	ICamera3D * cam;

	DrawServiceProvider();
	virtual ~DrawServiceProvider();

	vec3 getForwardViewVec();//the direction where the current view is looking
	vec3 getUpViewVec();//the direction where up is in the current view

	void transformViewToGUI();//must be reverted after draw calls finished
	void revertView();

};

#endif /* SRC_DRAWSERVICEPROVIDER_H_ */
