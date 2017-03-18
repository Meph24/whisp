/*
 * ITexture.h
 *
 *  Created on:	14.02.2017
 *      Author:	HL65536
 *     Version:	2.1
 */

#ifndef SRC_ITEXTURE_H_
#define SRC_ITEXTURE_H_

#include "TexParamSet.h"
#include <GL/glew.h>
class ITexture
{
protected:
	GLuint texID;
	TexParamSet * params;
	bool first;
public:
	virtual void update() = 0;
	virtual void update(TexParamSet * texParams) = 0;
	virtual void bind() = 0;
	ITexture(TexParamSet * texParams);
	virtual ~ITexture();
};

#endif /* SRC_ITEXTURE_H_ */
