#ifndef OXEL_HPP
#define OXEL_HPP

#include <array>
#include <memory>
#include "glmutils.hpp"

using std::array;
using std::shared_ptr;
using glm::mat4;

class Oxel
{
    mutable shared_ptr<array<Oxel, 8>> m_children = nullptr;

public:
    struct Form
    {
        bool is_leaf = true;
        bool full = false;
        bool empty() const;
    }form;

    Oxel();

    bool isLeaf() const;
    const array<Oxel, 8>& children() const;

    array<Oxel, 8>& children();
    void collapse();
    void expand() const;
    void drawHere() const;

    friend class OxelTree;
};

struct OxelTree
{
    Oxel root;
    Oxel& operator*();
    Oxel& operator->();

    int8_t root_granularity;
    float base_width;

    void higher( uint8_t supplant_index );
    void lower( uint8_t supplant_index );

    mat4 transform;

    OxelTree( int8_t root_granularity );
    void drawOxel(const Oxel& oxel, int8_t granularity, int8_t mingranularity = 0) const;
    void drawHere() const;
    void setTransform( const mat4& transform );
};

#endif // OXEL_HPP
