#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/vec4.hpp>
#include <GL/glew.h>

using glm::vec4;

struct MaterialList;

struct Material
{
    vec4 color;
    void applyManualDrawParametersToOpenGl() const;
    static MaterialList list;
};

struct MaterialList
{
    Material air = { {1.0, 1.0, 1.0, 0.0} };
    Material rock = { {0.3, 0.3, 0.3, 1.0} };
    Material dirt = { {0.2, 0.2, 0.0, 1.0} };
};

#endif // MATERIAL_HPP
