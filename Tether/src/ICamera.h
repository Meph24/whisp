
#ifndef SRC_ICAMERA_H_
#define SRC_ICAMERA_H_

#include <GL/glew.h>
class ICamera
{
public:

	float posX, posY, posZ;
	float zoom;// = tan(fov/2)
	float width;// width of screen in pixels
	float height;// height of screen in pixels

	float maxView, minView;//min and max view distance

	//calculates the matrices and applies them to modelview and projection matrices
	virtual void apply()=0;
	void applyFresh();//resets the matrices and calls apply() afterwards


	ICamera();
	virtual ~ICamera();
};

#endif /*SRC_ICAMERA_H_*/
