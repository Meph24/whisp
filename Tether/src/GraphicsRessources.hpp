#ifndef GRAPHICSRESSOURCES_HPP
#define GRAPHICSRESSOURCES_HPP

#include <memory>
#include "ITexture.h"
#include "TexParamSet.h"

using std::unique_ptr;

struct GraphicsRessources
{
	unique_ptr<ITexture> zombieTex;
	unique_ptr<ITexture> grass;
	unique_ptr<ITexture> shot;
	unique_ptr<ITexture> tree;
	unique_ptr<ITexture> leaves;

	unique_ptr<TexParamSet> tps;

	unique_ptr<TexParamSet> tps2;

	GraphicsRessources();
};

#endif // GRAPHICSRESSOURCES_HPP
