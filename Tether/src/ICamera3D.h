
#ifndef SRC_ICAMERA3D_H_
#define SRC_ICAMERA3D_H_

#include "ICamera.h"
#include "DivisionPlane.h"
#include "MatrixLib2.h"
class ICamera3D: public ICamera
{
	vec3 getNormal(vec3 v);
	MatrixLib2 ml;
protected:
	float leftPart;
	float rightPart;
	float upPart;
	float downPart;
	void applyFrustum();
public:

	float alpha, beta, gamma;

	virtual void apply()=0;

	//only works if derived Cameras use the standard definition of alpha/beta/gamma
	vec3 getNearNormal();
	vec3 getFarNormal();
	vec3 getRightNormal();
	vec3 getLeftNormal();
	vec3 getUpperNormal();
	vec3 getLowerNormal();


	//these give the DivisionPlanes from the last applyFrustum() or apply() call
	virtual DivisionPlane getNearPlane()=0;
	virtual DivisionPlane getFarPlane(float overrideViewDist=-1)=0;
	virtual DivisionPlane getRightPlane()=0;
	virtual DivisionPlane getLeftPlane()=0;
	virtual DivisionPlane getUpperPlane()=0;
	virtual DivisionPlane getLowerPlane()=0;



	ICamera3D();
	virtual ~ICamera3D();
};
#endif /*SRC_ICAMERA3D_H_*/
