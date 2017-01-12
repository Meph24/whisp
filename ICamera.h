#pragma once
class ICamera
{
	float mat[16];

	

public:

	float posX, posY, posZ;

	//calculates the matrix and applies it to opengl matrix
	virtual void apply()=0;
	//applies and saves matrix for fast reuse with applyAgain(); uses apply()
	void applyAndStore();
	//applies last saved matrix
	void applyAgain();
	//updates saved matrix without applying it for fast use with applyAgain(); basically calls applyFirst and reverts changes to opengl matrix
	void updateStoredMatrix();

	void setPosition(float x, float y, float z);
	virtual void reset() = 0;
	virtual void resetAngle() = 0;
	virtual void changeAngleBy(float alpha, float beta, float gamma) = 0;
	virtual void changeZoomBy(float factor) = 0;
	virtual void resetZoom() = 0;
	ICamera();
	~ICamera();
};