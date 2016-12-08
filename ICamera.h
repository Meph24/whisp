#pragma once
class ICamera
{
	float mat[16];
	float posX, posY, posZ;
public:


	void apply();

	void setPosition(float x, float y, float z);
	virtual void reset() = 0;
	virtual void resetAngle() = 0;
	virtual void changeAngleBy(float alpha, float beta, float gamma) = 0;
	virtual void changeZoomBy(float factor) = 0;
	virtual void resetZoom() = 0;
	ICamera();
	~ICamera();
};

