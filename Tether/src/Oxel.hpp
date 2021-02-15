#ifndef OXEL_HPP
#define OXEL_HPP

#include <functional>

#include "glmutils.hpp"
#include "Octree.hpp"

#include "Material.hpp"

using std::function;

struct Oxel
{
    Material *material = &Material::list.air;
    void draw() const;
};

struct OxelTree
{
    typedef vec3 VecType;
    typedef float LenType;
    //TODO evaluate fixpoint decimals as a valid type
    // float will no be enough at some point

    Octree<Oxel> tree;
    int8_t root_granularity = 0;

    //the width of an oxel with granularity = 0
    LenType baselen = 1.0f;
    LenType oxelWidth(int8_t granularity);

    OxelTree() = default;

private:
    //helpers

    static VecType calculateParentCenter(int8_t this_granularity, const VecType& this_center, uint8_t this_index);
    static VecType calculateChildCenter(int8_t this_granularity, const VecType& this_center, uint8_t child_index);

public:
    struct Crawler_Base
    {
        OxelTree* tree = nullptr;
        Octree<Oxel>::Node* node_ = nullptr;

        int8_t granularity = std::numeric_limits<int8_t>::max();
        OxelTree::VecType center = VecType(0.0f);

        bool valid() const;
        operator bool() const;

        Octree<Oxel>::Node& node();
        Oxel& value();
    };
    

    struct Creator;
    struct Viewer : public Crawler_Base
    {
        Viewer() = default;
        Viewer( OxelTree& );
        Viewer( const Viewer& ) = default;
        Viewer& operator=( const Viewer& ) = default;
        Viewer( const OxelTree::Creator& );
        Viewer& operator=( const OxelTree::Creator& );

        Viewer child ( uint8_t node_index ) const;
        Viewer parent() const;
        Viewer decendant( const vector<uint8_t>& decendant_route ) const;
        Viewer ancestor( unsigned int generations ) const;
        Viewer cousin( int64_t, int64_t, int64_t ) const;

        const Octree<Oxel>::Node& node() const;
        const Oxel& value() const;
    };
    struct Creator : public Crawler_Base
    {
        using OxelInitializer = std::function<Oxel (const Creator&)>;
        static OxelInitializer default_oxel_initializer;
        OxelInitializer initializer = default_oxel_initializer;

        Creator() = default;
        Creator( OxelTree& );
        Creator( OxelTree&, OxelInitializer);
        Creator( const Creator& ) = default;
        Creator& operator=( const Creator& other ) = default;
        Creator( const OxelTree::Viewer&, OxelInitializer );

        void setOxelInitializer(OxelInitializer);

        Creator child ( uint8_t node_index ) const;
        Creator parent() const;
        Creator decendant( const vector<uint8_t>& decendant_route ) const;
        Creator ancestor( uint8_t generations ) const;
        Creator cousin( int64_t, int64_t, int64_t ) const;
    };
};


#endif // OXEL_HPP
