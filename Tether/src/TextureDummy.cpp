/*
 * TextureDummy.cpp
 *
 *  Created on:	Feb 1, 2018
 *      Author:	HL65536
 *     Version:	1.0
 */

#include "TextureDummy.h"
#include <iostream>
TextureDummy::TextureDummy():
ITexture(0)
{

}

void TextureDummy::update()
{
	std::cout<<"Texture not set properly, dummy was called!"<<std::endl;
}

void TextureDummy::update(TexParamSet* texParams)
{
	std::cout<<"Texture not set properly, dummy was called!"<<std::endl;
}

void TextureDummy::bind()
{
	std::cout<<"Texture not set properly, dummy was called!"<<std::endl;
}

TextureDummy::~TextureDummy()
{}

