/*
 * TextureDynamic2Dcpu.cpp
 *
 *  Created on:	14.02.2017
 *      Author:	HL65536
 *     Version:	2.0
 */

#include "TextureDynamic2Dcpu.h"

TextureDynamic2Dcpu::TextureDynamic2Dcpu(TexParamSet * texParams, const void * imageData,int width,int height):
ITexture(texParams),dataLoc(imageData),w(width),h(height)
{

}
TextureDynamic2Dcpu::~TextureDynamic2Dcpu()
{

}

void TextureDynamic2Dcpu::bind()
{
	glBindTexture(GL_TEXTURE_2D, texID);
}
#include "myAssert.h"

//TEST
#include <iostream>
void TextureDynamic2Dcpu::update()
{


	assert(dataLoc!=NULL);


	if(!called)
	{
		called=true;
		glGenTextures(1, &texID);		//1st Para: number of TextureIds generated ; 2nd Para array to store IDs
	}
	bind();
	params->apply();


	//TEST
	std::cout << "Loaded RAM tex: ID"<<texID<<std::endl;//throw std::runtime_error("Texture " + std::string(fName) + " unable to load !");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataLoc);
}

void TextureDynamic2Dcpu::update(TexParamSet * texParams)
{
	params = texParams;
	update();
}
