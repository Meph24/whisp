#include "Oxel.hpp"

#include <GL/glew.h>

#include "drawUtils.hpp"

bool Oxel::Form::empty() const { return !full; }

Oxel::Oxel()
    : m_children(nullptr)
{}

bool Oxel::isLeaf() const { return !(bool) m_children; }
const array<Oxel, 8>& Oxel::children() const { if(isLeaf()) expand(); return *m_children; }

array<Oxel, 8>& Oxel::children() { if(isLeaf()) expand(); return const_cast<array<Oxel, 8>&>(*m_children); }
void Oxel::collapse() { m_children.reset(); }
void Oxel::expand() const
{ 
    if( !isLeaf() ) return;
    m_children.reset( new array<Oxel, 8> );
    if(form.full) m_children->fill( Oxel(*this) );
}

void Oxel::drawHere() const
{
    if(form.full) drawCube();
}

Oxel& OxelTree::operator*(){ return root; } 
Oxel& OxelTree::operator->(){ return root; }
void OxelTree::higher( uint8_t supplant_index )
{
    if(supplant_index < 0 || supplant_index >= 8) return;
    if(root_granularity >= std::numeric_limits<int8_t>::max()) return;

    Oxel newoxel;
    newoxel.children()[supplant_index] = root;
    root = newoxel;
    root_granularity++;
}
void OxelTree::lower( uint8_t supplant_index )
{
    if(supplant_index < 0 || supplant_index >= 8) return;
    if(root_granularity <= std::numeric_limits<int8_t>::min()) return;

    root = root.children()[supplant_index];
    root_granularity--;
}

OxelTree::OxelTree( int8_t root_granularity )
        : root_granularity(0)
        , base_width( pow(2.0,root_granularity) )
        , transform(1.0f)
    {}

void OxelTree::drawOxel(const Oxel& oxel, int8_t granularity, int8_t mingranularity) const
{
    if( granularity < mingranularity ) return;
    if( !oxel.isLeaf() )
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

void OxelTree::drawHere() const
{
    glPushMatrix();
    drawOxel( root , root_granularity, -8);
    glPopMatrix();
}

void OxelTree::setTransform( const mat4& transform ) { this->transform = transform; }

