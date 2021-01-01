#include "GraphicsRessources.hpp"

#include "TextureStatic2D.h"
#include "EntityProjectileBulletLike.h"

GraphicsRessources::GraphicsRessources()
{
	tps.reset(new TexParamSet());
	tps->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps->addF(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_NEAREST);
	tps->addF(GL_TEXTURE_MAX_ANISOTROPY_EXT,16);//TODO compatibility check
	tps->addF(GL_TEXTURE_LOD_BIAS,0);
	tps->enableMipmap();
	zombieTex = std::make_unique<TextureStatic2D>(tps.get(), "./res/zombie.png");
	zombieTex->update();
	grass = std::make_unique<TextureStatic2D>(tps.get(), "./res/grass_top.png");
	grass->update();
	tree = std::make_unique<TextureStatic2D>(tps.get(), "./res/log_oak.png");
	tree->update();

	tps2 = std::make_unique<TexParamSet>();
	tps2->addI(GL_TEXTURE_WRAP_S, GL_REPEAT);
	tps2->addI(GL_TEXTURE_WRAP_T, GL_REPEAT);
	tps2->addF(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	tps2->addF(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	shot = std::make_unique<TextureStatic2D>(tps2.get(), "./res/fireball.png");
	shot->update();
	leaves = std::make_unique<TextureStatic2D>(tps2.get(), "./res/leaves.png");
	leaves->update();

	

	EntityProjectileBulletLike::setTexture(shot.get()); //TODO refactor static of EntityProjectile Bullet like
}