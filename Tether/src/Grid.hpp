#ifndef GRID_HPP
#define GRID_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <vector>

#include <GL/glew.h>

#include "glmutils.hpp"

#include "drawUtils.hpp"

using std::array;
using std::exp;
using std::fill;
using std::shared_ptr;
using std::vector;

using glm::mat4;

template<size_t extent, typename T>
struct narray : public array< T, 2*extent + 1>
{
    T& at(int32_t index) { return array<T, 2*extent+1>::at(index + extent); }
    T& operator[](int32_t index){ return at(index); };
};

template<int extent>
struct Grid
{
    struct Element { bool existence; Element() : existence(false) {} };
    narray<extent, narray<extent, narray<extent, Element>>> g;

    mat4 transform;

    Element& at(int32_t x, int32_t y, int32_t z){ return g[x][y][z]; }
    Element& operator()(int32_t x, int32_t y, int32_t z){ return at(x, y, z); }

    Grid() : transform(1.0f) { at(0,0,0).existence = true; } 

    void setTransform(const mat4& newm) { transform = newm; }

    void draw()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);            

        glPushMatrix();
	    glMultMatrixf((float*) glm::value_ptr(glm::transpose(transform)));

        int count = 3;
        for (int x = -count;  x < count;  x++)
        {
            for (int y = -count; y < count; y++)
            {
                for (int z = -count; z < count; z++)
                {
                    if (at(x, y, z).existence)
                    {
                        glPushMatrix();                            
                        glTranslatef((float) x, (float) y, (float) z);
                        drawCube();
                        glPopMatrix();
                    }
                }
            }
        }

        glPopMatrix();
        glDisable(GL_CULL_FACE);
    }
};

#endif // GRID_HPP
