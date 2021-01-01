
#ifndef SRC_ICAMERA_H_
#define SRC_ICAMERA_H_

#include <GL/glew.h>

#include "Eye.hpp"
class ICamera
{
public:
	Eye* eye;

	float zoom;// = tan(fov/2); smaller value means things look bigger
	float width;// width of screen in pixels
	float height;// height of screen in pixels

	float maxView, minView;//min and max view distance

	//calculates the matrices and applies them to modelview and projection matrices
	virtual void apply()=0;
	void applyFresh();//resets the matrices and calls apply() afterwards


	ICamera(Eye& eye);
	virtual ~ICamera() = default;
};

#endif /*SRC_ICAMERA_H_*/
