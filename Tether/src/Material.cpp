#ifndef MATERIAL_CPP
#define MATERIAL_CPP

#include "Material.hpp"

void Material::applyManualDrawParametersToOpenGl() const
{
    glColor4f(color.x, color.y, color.z, color.w);
}

MaterialList Material::list = MaterialList();

#endif // MATERIAL_CPP
