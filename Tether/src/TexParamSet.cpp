#include "TexParamSet.h"


TexParamSet::TexParamSet()
{
}


TexParamSet::~TexParamSet()
{
}

void TexParamSet::addI(GLenum e, GLint i)
{
	enumI.push_back(e);
	paramI.push_back(i);
}

void TexParamSet::addF(GLenum e, GLfloat f)
{
	enumF.push_back(e);
	paramF.push_back(f);
}

#include "ShortNames.h"
void TexParamSet::apply()
{
	u32 iCount=enumI.size();
	u32 fCount=enumF.size();
	for (u32 i = 0; i < iCount; i++)
	{
		glTexParameteri(GL_TEXTURE_2D, enumI[i], paramI[i]);
	}
	for (u32 i = 0; i < fCount; i++)
	{
		glTexParameterf(GL_TEXTURE_2D, enumF[i], paramF[i]);
	}
}
