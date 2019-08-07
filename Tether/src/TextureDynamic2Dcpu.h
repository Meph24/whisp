/*
 * TextureDynamic2Dcpu.h
 *
 *  Created on:	14.02.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#ifndef SRC_TEXTUREDYNAMIC2DCPU_H_
#define SRC_TEXTUREDYNAMIC2DCPU_H_

#include "ITexture.h"

class TextureDynamic2Dcpu: public ITexture
{
	const void * dataLoc;
	int w,h;
	bool called=0;
public:
	void bind();
	void update();
	void update(TexParamSet * texParams);

	TextureDynamic2Dcpu(TexParamSet * texParams, const void * imageData,int width,int height);//TODO additional parameters for glTexImage2D
	~TextureDynamic2Dcpu();
};

#endif /* SRC_TEXTUREDYNAMIC2DCPU_H_ */
