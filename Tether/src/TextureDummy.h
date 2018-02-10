/*
 * TextureDummy.h
 *
 *  Created on:	Feb 1, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#ifndef SRC_TEXTUREDUMMY_H_
#define SRC_TEXTUREDUMMY_H_

#include "ITexture.h"

class TextureDummy: public ITexture
{
public:
	void update();
	void update(TexParamSet * texParams);
	void bind();
	TextureDummy();
	~TextureDummy();
};

#endif /* SRC_TEXTUREDUMMY_H_ */
