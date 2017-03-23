
#ifndef SRC_ICAMERA3D_H_
#define SRC_ICAMERA3D_H_

#include "ICamera.h"
class ICamera3D :
	public ICamera
{

public:

	float alpha, beta, gamma;

	virtual void apply()=0;

	ICamera3D();
	virtual ~ICamera3D();
};
#endif /*SRC_ICAMERA3D_H_*/
