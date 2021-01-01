#include "Eye.hpp"

#include "glmutils.hpp"
#include "EntityPlayer.h"
#include "IWorld.h"

spacevec Eye::absolutePosition( IWorld& world ) const
{
	return entity.pos + world.fromMeters(character_eye_offset);
}

const vec3& Eye::offsetFromEntity() const
{
	return character_eye_offset;
}
vec3 Eye::direction() const
{
	return rotateVec( vec3(0.0f, 0.0f, 1.0f), rotation );
}
vec3 Eye::up() const
{
	return rotateVec( vec3(0.0f, 1.0f, 0.0f), rotation );
}

void Eye::setRotation( const vec3& rotation ) { this->rotation = rotation; }

Eye::Eye( Entity& entity ) : entity( entity ) {}