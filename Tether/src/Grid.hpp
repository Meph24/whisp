#ifndef GRID_HPP
#define GRID_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <vector>

using std::array;
using std::exp;
using std::fill;
using std::shared_ptr;
using std::vector;

template<size_t extent, typename T>
struct narray : public array< T, 2*extent + 1>
{
    T& at(int32_t index) { return array<T, 2*extent+1>::at(index + extent); }
    T& operator[](int32_t index){ return at(index); };
};

void drawCube() 
{
    glBegin(GL_POLYGON);
    glColor3f(   0.01,  0.01, 0.01 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();

    // White side - BACK
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  1.0, 1.0 );
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();
    
    // Purple side - RIGHT
    glBegin(GL_POLYGON);
    glColor3f(  1.0,  0.0,  1.0 );
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();
    
    // Green side - LEFT
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  1.0,  0.0 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
    
    // Blue side - TOP
    glBegin(GL_POLYGON);
    glColor3f(   0.0,  0.0,  1.0 );
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();
    
    // Red side - BOTTOM
    glBegin(GL_POLYGON);
    glColor3f(   1.0,  0.0,  0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
    
    glFlush();
}

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

#include "Spacevec.h"
#include "Entity.h"
#include "AABB.h"
#include "FloatSeconds.hpp"
#include "TickServiceProvider.h"
#include "IWorld.h"

struct GridEntity : public Entity
{
    Grid<32> grid;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

    GridEntity( const Grid<32>& grid ) : grid(grid) {} ;

    void tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 )
    {
        float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

        tick_begin_pos = pos;
        tick_begin_v = v;
        tick_begin_rot = rot;
        tick_begin_drot = drot;

        IWorld* iw = tsp->getIWorld();
        
        bb.low = iw->fromMeters(-vec3(2.0f,2.0f,2.0f)) + pos;
        bb.high = iw->fromMeters(vec3(2.0f,2.0f,2.0f)) + pos;

        //simulation testing code
        //TODO remove
        
        if(glm::length(iw->toMeters(pos)) > 500)
            v = -v;

        pos += v*tick_seconds;
        rot += drot* tick_seconds;
        grid.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
                                *	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
                                *	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
        );

    /*
        iw->collideAlgo->doChecks(
            (Collider*) this, (Entity*) this,
            tick_seconds, *tsp);
    */
        last_ticked = next_tick_begin;
    }

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 )
    {            
        float tickOffset = (float)FloatSeconds(ts-last_ticked);
        //if(!viewFrustum->inside(bb,iw)) return;
        spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
        vec3 interPosMeters = iw.toMeters(interPos);

        glColor3f(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        //apply position
        glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
        
        grid.draw();

        glPopMatrix();
    }

};

class Oxel
{
    const Oxel* m_parent = nullptr;
    mutable shared_ptr<array<Oxel, 8>> m_children = nullptr;

    bool full;
public:
    array<Oxel, 8>& children() { if(!isFiner()) finer(); return const_cast<array<Oxel, 8>&>(*m_children); }
    const array<Oxel, 8>& children() const { if(!isFiner()) finer(); return *m_children; }

    Oxel( const Oxel* parent )
        : m_parent(parent)
        , m_children(nullptr)
        , full(false)
    {}
    Oxel() : Oxel(nullptr) {}

    bool isRoot() const { return !m_parent; }
    bool isEmpty() const { return !full; }
    bool isFiner() const { return (bool) m_children; }
    void rougher() { m_children.reset(); }
    void finer() const
    { 
        if(isFiner()) return;
        m_children.reset( new array<Oxel, 8> );
        if(full) m_children->fill( Oxel(*this) );
    }

    void fill( bool b = true ) { full = b; }

    void drawHere() const
    {
        if(full) drawCube();
    }
};

struct OxelTree
{
    Oxel root;
    Oxel& operator*(){ return root; } 
    Oxel& operator->(){ return root; }

    int8_t root_granularity;
    float base_width;

    mat4 transform;

    OxelTree( int8_t root_granularity )
        : root_granularity(0)
        , base_width( pow(2.0,root_granularity) )
        , transform(1.0f)
    {}

    void drawOxel(const Oxel& oxel, int8_t granularity, int8_t mingranularity = 0) const
    {
        if(granularity < mingranularity) return;
        if( oxel.isFiner() )
        {
            glPushMatrix();
            glScalef( 0.5, 0.5, 0.5 );
            size_t i = 0;
            for( const Oxel& o: oxel.children() )
            {
                glPushMatrix();
                glTranslatef( 
                    (i & 0x1) ? 0.5 : -0.5, 
                    (i & 0x2) ? 0.5 : -0.5,
                    (i & 0x4) ? 0.5 : -0.5 
                );
                drawOxel( o, granularity-1, mingranularity );
                glPopMatrix();
                i++;
            }
            glPopMatrix();
        }
        else
        {
            oxel.drawHere();
        }
        
    }

    void drawHere() const
    {
        glPushMatrix();
        drawOxel( root , root_granularity, -8);
        glPopMatrix();
    }

    void setTransform( const mat4& transform ) { this->transform = transform; }
};


class OxelEntity : public Entity
{
    OxelTree t;

    vec3 rot, drot;
	spacevec tick_begin_pos, tick_begin_v;
	vec3 tick_begin_rot, tick_begin_drot;

public:
    OxelEntity( const OxelTree& t ) : t(t) {} ;

    void tick(	const SimClock::time_point& next_tick_begin,
						TickServiceProvider* tsp
					 )
    {
        float tick_seconds = (float) FloatSeconds(next_tick_begin - last_ticked);

        tick_begin_pos = pos;
        tick_begin_v = v;
        tick_begin_rot = rot;
        tick_begin_drot = drot;

        IWorld* iw = tsp->getIWorld();

        float aabblength = t.base_width * 1.5; //approx sqrt of 2
        bb.low = iw->fromMeters(-vec3(aabblength, aabblength, aabblength)) + pos;
        bb.high = iw->fromMeters(vec3(aabblength, aabblength, aabblength)) + pos;

        //simulation testing code
        //TODO remove
        
        if(glm::length(iw->toMeters(pos)) > 500)
            v = -v;

        pos += v*tick_seconds;
        rot += drot* tick_seconds;
        t.setTransform(		glm::rotate(glm::radians(rot.x), vec3(1,0,0))
                            *	glm::rotate(glm::radians(rot.y), vec3(0,1,0))
                            *	glm::rotate(glm::radians(rot.z), vec3(0,0,1))
        );

    /*
        iw->collideAlgo->doChecks(
            (Collider*) this, (Entity*) this,
            tick_seconds, *tsp);
    */
        last_ticked = next_tick_begin;
    }

	void draw(	const SimClock::time_point& ts, 
						Frustum* viewFrustum, 
						IWorld& iw,
						DrawServiceProvider* dsp
					 )
    {            
        float tickOffset = (float)FloatSeconds(ts-last_ticked);
        //if(!viewFrustum->inside(bb,iw)) return;
        spacevec interPos = pos + v*tickOffset - viewFrustum->observerPos;
        vec3 interPosMeters = iw.toMeters(interPos);

        glColor3f(1.0f, 0.0f, 0.0f);

        glPushMatrix();

        //apply position
        glTranslatef(interPosMeters.x, interPosMeters.y, interPosMeters.z);
        t.drawHere();

        glPopMatrix();
    }

};

#endif // GRID_HPP
