#include "Perspective.hpp"

#include <GL/glew.h>
#include <SFML/Window.hpp>

#include "EntityProjectileBulletLike.h"
#include "Frustum.h"
#include "Graphics2D.h"
#include "ICamera3D.h"
#include "IWorld.h"
#include "MainApp.h"
#include "Simulation.hpp"
#include "TextureStatic2D.h"
#include "WarnErrReporter.h"
#include "Zombie_Gun.h"

const float Perspective::default_zoom = 1.0f;

Perspective::Perspective(sf::Window* window, EntityPlayer* observing_entity, Simulation* simulation)
	: avatar(observing_entity)
	, simulation(simulation)
	, graphics2d(64, window->getSize().x / window->getSize().y)
	, camera(std::make_unique<CameraTP>( observing_entity->eye ))
	, pmGraphics(8s, 1s)
	, dsGraphics(&pmGraphics, &graphics2d)
	, dsLogic(&simulation->pmLogic, &graphics2d)
{
	enableThirdPerson(enable_third_person);
	camera->height=window->getSize().y;
	camera->width=window->getSize().x;
	camera->maxView=1024*8;
	camera->zoom=Perspective::default_zoom;

	timer_graphics=pmGraphics.createTimestep		("            other");
	timer_graphics.setAsRoundtripMarker				("Total render time");
	timer_graphics_world=pmGraphics.createTimestep	("   world contents");
	timer_graphics_debug=pmGraphics.createTimestep	("     debug screen");
	timer_graphics_flush=pmGraphics.createTimestep	(" GPU work + syncs");
}

void Perspective::enableAABBDrawing(bool b){ enable_aabbs = b; }
void Perspective::enableDebugScreen(bool b){ enable_debug = b; }
void Perspective::enableHUD(bool b){ enable_hud = b; }

void Perspective::enableThirdPerson(bool on)
{
	if(on)
	{
		setThirdPersonDistance(camera->dist);
		camera->minView=0.5f;
	}
	else
	{
		camera->dist=0;
		camera->minView=0.125f;
	}
}

void Perspective::setThirdPersonDistance(float meters)
{
	if(meters > third_person_distance_max) meters = third_person_distance_max;
	if(meters < third_person_distance_min) meters = third_person_distance_min;
	camera->dist = meters;
}

bool Perspective::isThirdPerson() const
{
	return camera->dist >= third_person_distance_min;
}

void Perspective::drawGameOver()
{
	transformViewToGUI();
	glColor3f(1, 0, 0);
	graphics2d.drawString("GAME OVER", -0.8f, -0.2f, 0.4f);
	glColor3f(0, 1, 0);
	graphics2d.drawString("R=restart", -0.8f, -0.6f, 0.3f);

	char scoreString[8];

	int scoreTemp = avatar->score;
	for (int i = 0; i < 8; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	graphics2d.drawString("score:", -0.8f, 0.8f, 0.1f);
	graphics2d.drawString(scoreString, -0.8f, 0.62f, 0.1f, 8);
	revertView();
}

unique_ptr<Frustum> Perspective::newFrustumApplyPerspective(	SimClock::time_point t,
															bool fresh,
															float viewDistRestriction	)
{
	float zoom_factor=zoomed?zoom_mult:1;
	camera->zoom=default_zoom/zoom_factor;
	float time=(float)FloatSeconds(t-avatar->last_ticked);
	spacevec curPos= avatar->pos + avatar->v*time;


	if(fresh) camera->applyFresh();
	else camera->apply();
	vec3 fwd=camera->getForwardVector();
	sf::Listener::setDirection(fwd.x,fwd.y,fwd.z);
	vec3 upv=camera->getUpVector();
	sf::Listener::setUpVector(upv.x,upv.y,upv.z);
	const vec3& pos = camera->eye->offsetFromEntity();
	sf::Listener::setPosition(pos.x, pos.y, pos.z);

	unique_ptr<Frustum> ret = std::make_unique<Frustum>();
	ret->observerPos=curPos;
	bool lookingUp = camera->eye->rotation.x < 0;
	if(lookingUp)
	{
		ret->planes[0]=camera->getUpperPlane();
		ret->planes[3]=camera->getLowerPlane();
	}
	else
	{
		ret->planes[0]=camera->getLowerPlane();
		ret->planes[3]=camera->getUpperPlane();
	}
	ret->planes[1]=camera->getLeftPlane();
	ret->planes[2]=camera->getRightPlane();
	ret->planes[4]=camera->getFarPlane(viewDistRestriction);
	if(FRUSTUM_PLANE_COUNT==6)
	{
		ret->planes[5]=camera->getNearPlane();
	}
	return ret;
}

void Perspective::draw()
{
	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix
	if( avatar->HP < 0 ) drawGameOver();
	else
	{
		drawAvatarPerspective();
		if(enable_hud) drawHUD();
	}
}

void Perspective::drawAvatarPerspective()
{
	const SimClock::time_point t = simulation->clock.now();

	enableThirdPerson(enable_third_person);

	glMatrixMode(GL_MODELVIEW);      // To operate on Model-View matrix

	timer_graphics.registerTime();

	unique_ptr<Frustum> viewFrustum =
		newFrustumApplyPerspective(t, true); //TODO dangerouse allocations and ownership transferation

	graphics_ressources.grass->bind();

	simulation->world().draw(t, viewFrustum.get(), simulation->world(), *this);

	timer_graphics_world.registerTime();

	if(isThirdPerson())
		avatar->draw( t, viewFrustum.get(), simulation->world() , *this );

	if(enable_debug)
	{
		simulation->pmLogic.draw(t, viewFrustum.get(), simulation->world(), *this);

		transformViewToGUI(1.0f);
		glColor3f(1, 0, 1);
		glEnable(GL_TEXTURE_2D);
		vec3 ppos = simulation->world().toMeters(avatar->pos);
		int offset=dsGraphics.draw(ppos.x, ppos.y, ppos.z, 0);
		dsLogic.draw(avatar->pos.x.intpart,avatar->pos.y.intpart,avatar->pos.z.intpart,offset);
		glDisable(GL_TEXTURE_2D);
		revertView();
	}
	timer_graphics_debug.registerTime();

	sf::sleep(sf::microseconds(1000));
	glFlush();
	timer_graphics_flush.registerTime();
}

void Perspective::drawHUD()
{
	glEnable(GL_TEXTURE_2D);
	transformViewToGUI();
	glColor3f(0, 1, 0);
	glPushMatrix();
	char scoreString[8];
	int scoreTemp = avatar->score;
	for (int i = 0; i < 8; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10)+'0';
		scoreTemp /= 10;
	}
	graphics2d.drawString("score:", -0.8f, 0.8f, 0.1f);
	graphics2d.drawString(scoreString, -0.8f, 0.62f, 0.1f, 8);

	scoreTemp = avatar->HP;
	for (int i = 0; i < 3; i++)
	{
		scoreString[7 - i] = (scoreTemp % 10) + '0';
		scoreTemp /= 10;
	}
	graphics2d.drawString("health:", -0.2f, 0.8f, 0.1f);
	graphics2d.drawString(scoreString+5, -0.2f, 0.62f, 0.1f, 3);

	glColor3f(1, 1, 0);
	string current_weapon_name = "None";
	if(avatar->current_gun) current_weapon_name = avatar->current_gun->name;
	graphics2d.drawString("Weapon:", 0.6f, -0.66f, 0.1f);
	graphics2d.drawString(current_weapon_name, 0.6f, -0.82f, 0.1f);

	glColor3f(0, 1, 0);
	glPopMatrix();
	float crosshairSize = 0.005f;
	int crosshairAmount = 4;

	glDisable(GL_TEXTURE_2D);
	glColor3f(1, avatar->hitmark, 0);

	glPushMatrix();
	for (int i = 0; i < crosshairAmount; i++)
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0, crosshairSize, 1);
		glVertex3f(-crosshairSize, crosshairSize * 6, 1);
		glVertex3f(crosshairSize, crosshairSize * 6, 1);
		glEnd();
		glRotatef(360.0f / crosshairAmount, 0, 0, 1);
	}
	glPopMatrix();
	revertView();
}

vec3 Perspective::forwardVec() { return camera->getForwardVector(); }
vec3 Perspective::upVec(){ return camera->getUpVector(); }
float Perspective::getAspectRatio() { return camera->width/camera->height; }
vec3 Perspective::getCamPos(){ return camera->eye->offsetFromEntity(); /* TODO accurate for 3rd person? */ }

void Perspective::transformViewToGUI(float priority)
{
	glPushMatrix();
//	depthStatus=glIsEnabled(GL_DEPTH_TEST);
//	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	float prioScale=2.25f-priority;
	if(!camera)
	{
		glScalef(prioScale,prioScale,-prioScale);
		WarnErrReporter::notInitializedErr("camera not initialized (in transformViewToGUI)");
	}
	else
	{
		glScalef(camera->minView*prioScale*camera->zoom, camera->minView*prioScale*camera->zoom, -camera->minView*prioScale);
	}
}

void Perspective::revertView()
{
	glPopMatrix();
//	if(depthStatus) glEnable(GL_DEPTH_TEST);
}

void Perspective::registerTransparentCallback(float priority,Drawable* callbackRequester)
{
	callbackList.push_back(std::make_pair(priority,callbackRequester));
}

void Perspective::doTransparentCallbacks(const SimClock::time_point& t,Frustum * viewFrustum, IWorld& iw)
{
	std::sort(callbackList.begin(), callbackList.end());
	isTransparentPass=true;
	for(auto e : callbackList)
	{
		e.second->draw(t, viewFrustum, iw, *this);
	}
	isTransparentPass=false;
}
