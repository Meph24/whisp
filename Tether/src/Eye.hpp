#ifndef EYE_HPP
#define EYE_HPP

#include <glm/vec3.hpp>
#include "Spacevec.h"

using glm::vec3;

class Entity;
class IWorld;

class Eye
{
    Entity& entity;
    vec3 character_eye_offset = vec3(0.0f, 0.8f, 0.0f);

public:
    vec3 rotation = vec3(0.0001f, 0.0001f, 0.0001f);

    Eye( Entity& );

    void setRotation( const vec3& );
    spacevec absolutePosition( IWorld& world ) const;
    const vec3& offsetFromEntity() const;
    vec3 direction() const;
    vec3 up() const;
};

#endif // EYE_HPP
