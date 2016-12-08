#include "TexParamSet.h"


TexParamSet::TexParamSet(int maxParamsI, int maxParamsF) :
iMax(maxParamsI),
fMax(maxParamsF),
iCount(0),
fCount(0)
{
	enumI = new GLenum[iMax + fMax];
	enumF = enumI + iMax;

	paramI = new GLint[iMax];
	paramF = new GLfloat[fMax];
}


TexParamSet::~TexParamSet()
{
	delete[] enumI;
	delete[] paramI;
	delete[] paramF;
}

bool TexParamSet::addI(GLenum e, GLint i)
{
	if (iCount < iMax)
	{
		enumI[iCount] = e;
		paramI[iCount] = i;
		iCount++;
		return true;
	}
	return false;
}

bool TexParamSet::addF(GLenum e, GLfloat f)
{
	if (fCount < fMax)
	{
		enumF[fCount] = e;
		paramF[fCount] = f;
		fCount++;
		return true;
	}
	return false;
}

#include <iostream>
void TexParamSet::apply()
{
	std::cout << "i" << iCount<<"/"<<iMax<<std::endl;
	std::cout << "f" << fCount<<"/"<<fMax<<std::endl;
	for (int i = 0; i < iCount; i++)
	{
		glTexParameteri(GL_TEXTURE_2D, enumI[i], paramI[i]);
		std::cout << "i=" << i << "  " << enumI[i] << "   " << paramI[i]<<std::endl;
	}
	for (int i = 0; i < fCount; i++)
	{
		glTexParameterf(GL_TEXTURE_2D, enumF[i], paramF[i]);
		std::cout << "f=" << i << "  " << enumF[i] << "   " << paramF[i] << std::endl;
	}
}
