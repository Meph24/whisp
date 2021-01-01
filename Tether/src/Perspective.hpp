#ifndef SRC_DRAWSERVICEPROVIDER_H_
#define SRC_DRAWSERVICEPROVIDER_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "CameraTP.h"
#include "DebugScreen.h"
#include "EntityPlayer.h"
#include "glmutils.hpp"
#include "Graphics2D.h"
#include "GraphicsRessources.hpp"
#include "ITexture.h"
#include "PerformanceMeter.h"
#include "SimClock.hpp"

using glm::vec3;
using std::unique_ptr;

class ITexture;
class Simulation;

//use with caution: when using this, code sections outside of these regions are called twice
#define TRANSPARENT_SECTION_DO_LATER(priority) \
	if(!perspective.isTransparentPass) perspective.registerTransparentCallback(priority,this); \
	if(perspective.isTransparentPass)

#define OTHER_SECTION \
	else


struct Perspective 
{
	GraphicsRessources graphics_ressources;

	bool enable_aabbs = false;
	bool enable_hud = true;
	bool enable_third_person = false;
	bool enable_debug = false;
	bool zoomed = false;
	float zoom_mult = 8.0f;

	static const float default_zoom;

	float third_person_distance_min = 2;
	float third_person_distance_max = 20;

	EntityPlayer* avatar = nullptr;
	Simulation* simulation = nullptr;

	Graphics2D graphics2d;
	unique_ptr<CameraTP> camera;

	std::vector<std::pair<float,Drawable *> > callbackList;
	bool isTransparentPass = false;
	unique_ptr<Frustum> newFrustumApplyPerspective(				SimClock::time_point t,
																bool fresh,
																float viewDistRestriction=-1	);

	//priority 0-1, higher prio GUI elements cover lower prio ones when they overlap
	void transformViewToGUI(float priority=0.5f);//call revertView after draw calls finished!!!; must not be activated multiple times within any instances at the same time (call revertView in between!)
	void revertView();

	void drawAvatarPerspective();
	void drawGameOver();
	void drawHUD();

//----------------------

	 Perspective(sf::Window* window, EntityPlayer* observing_entity, Simulation* simulation);

	void enableAABBDrawing(bool b = false);
	void enableDebugScreen(bool b = false);
	void enableHUD(bool b = true);
	void enableThirdPerson(bool b = false);
	bool isThirdPerson() const;

	void setThirdPersonDistance(float meters);

	vec3 forwardVec();
	vec3 upVec();
	vec3 getCamPos();
	float getAspectRatio();
	spacevec spectatorEyePosition();

	void draw();

	void registerTransparentCallback(float priority,Drawable * callbackRequester);
	void doTransparentCallbacks(const SimClock::time_point& t, Frustum * viewFrustum, IWorld& it);

private:
	PerformanceMeter pmGraphics;
	PerformanceMeter::SingleTimer timer_graphics;
	PerformanceMeter::SingleTimer timer_graphics_world;
	PerformanceMeter::SingleTimer timer_graphics_debug;
	PerformanceMeter::SingleTimer timer_graphics_flush;
	DebugScreen dsGraphics;
	DebugScreen dsLogic;
};

#endif /* SRC_DRAWSERVICEPROVIDER_H_ */
