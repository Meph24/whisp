#include "Oxel.hpp"
#include <GL/glew.h>
#include "drawUtils.hpp"
#include "utils.hpp"
#include <cmath>
#include <utility>

void Oxel::draw() const
{
    material->applyManualDrawParametersToOpenGl();
    drawCube();
}

OxelTree::LenType OxelTree::oxelWidth(int8_t granularity) { return baselen * std::pow(2.0f, granularity); }

OxelTree::VecType OxelTree::calculateParentCenter(int8_t this_granularity, const VecType& this_center, uint8_t this_index)
{
    VecType ret = this_center;
    LenType offset_length = pow(2.0, this_granularity) / 2.0f;
    ret.x -= ((this_index&0x1)?1.0f:-1.0f) * offset_length;
    ret.y -= ((this_index&0x2)?1.0f:-1.0f) * offset_length;
    ret.z -= ((this_index&0x4)?1.0f:-1.0f) * offset_length;
    return ret;
}

OxelTree::VecType OxelTree::calculateChildCenter(int8_t this_granularity, const VecType& this_center, uint8_t child_index)
{
    VecType ret = this_center;
    LenType offset_length = pow(2.0, this_granularity) / 4.0f;
    ret.x += ((child_index&0x1)?1.0f:-1.0f) * offset_length;
    ret.y += ((child_index&0x2)?1.0f:-1.0f) * offset_length;
    ret.z += ((child_index&0x4)?1.0f:-1.0f) * offset_length;
    return ret;
}

bool OxelTree::Crawler_Base::valid() const { return node_; }
OxelTree::Crawler_Base::operator bool() const { return valid(); }

Octree<Oxel>::Node& OxelTree::Crawler_Base::node() { return *this->node_; }
Oxel& OxelTree::Crawler_Base::value() { return this->node_->value; }

OxelTree::Viewer::Viewer( OxelTree& tree ) 
{ 
    this->tree = &tree;
    this->node_ = &tree.tree.root;
    this->granularity = tree.root_granularity;
    this->center = VecType( 0 );
}

OxelTree::Viewer::Viewer( const OxelTree::Creator& other )
    : Crawler_Base( (const Crawler_Base&) other )
{}
OxelTree::Viewer& OxelTree::Viewer::operator=( const OxelTree::Creator& other )
{
    tree = other.tree; node_ = other.node_; granularity = other.granularity; center = other.center;
    return *this;
}

OxelTree::Viewer OxelTree::Viewer::child( uint8_t node_index ) const
{
    if(node_->isLeaf()) return Viewer();

    Viewer ret = *this;
    ret.node_ = node_->child(node_index);
    ret.center = OxelTree::calculateChildCenter(granularity, center, node_index);
    --ret.granularity;
    return ret;
}

OxelTree::Viewer OxelTree::Viewer::parent() const
{
    if(node_->isRoot()) return Viewer();

    Viewer ret;
    ret.node_ = node_->parent();
    ret.center = calculateParentCenter(granularity, center, node_->nodeIndex());
    ++ret.granularity;
    return ret;
}

OxelTree::Viewer OxelTree::Viewer::decendant( const vector<uint8_t>& decendant_route ) const
{
    Viewer ret = *this;
    for(uint8_t direction : decendant_route)
    {
        if(!ret) return ret;
        ret = ret.child(direction);
    }
    return ret;
}

OxelTree::Viewer OxelTree::Viewer::ancestor( unsigned int generations ) const
{
    Viewer ret = *this;
    for (size_t i = 0; i < generations; i++)
    {
        if(!ret) return ret;
        ret = ret.parent();
    }
    return ret;
}

OxelTree::Viewer OxelTree::Viewer::cousin( int64_t x, int64_t y, int64_t z ) const
{
    if( *this ) return Viewer();

    uint8_t target_node_index = this->node_->nodeIndex() ^
    x && x%2 ? 0x1 : 0x0 ^
    y && y%2 ? 0x2 : 0x0 ^
    z && z%2 ? 0x4 : 0x0;

    x = signedrshift(x, 1);
    y = signedrshift(y, 1);
    z = signedrshift(z, 1);

    Viewer ret;
    if(x || y || z)     ret = this->cousin(x, y, z);
    else                ret = parent();
    if(!ret)  return ret;
    return ret.child(target_node_index);
}

const Octree<Oxel>::Node& OxelTree::Viewer::node() const { return *this->node_; }
const Oxel& OxelTree::Viewer::value() const { return this->node_->value; }

// Creator -------------------------------------------------------------------

OxelTree::Creator::OxelInitializer OxelTree::Creator::default_oxel_initializer = 
    [](const Creator& creator)->Oxel { return Oxel(); };

OxelTree::Creator::Creator( OxelTree& tree, OxelTree::Creator::OxelInitializer initializer ) 
{ 
    this->tree = &tree;
    this->node_ = &tree.tree.root;
    this->granularity = tree.root_granularity;
    this->center = VecType( 0 );
    this->initializer = initializer;
}

OxelTree::Creator::Creator( OxelTree& tree ) : Creator( tree, default_oxel_initializer ) {}

OxelTree::Creator::Creator( const OxelTree::Viewer& other, OxelTree::Creator::OxelInitializer initializer )
    : Crawler_Base( (const Crawler_Base&) other )
    , initializer(initializer)
{}

void OxelTree::Creator::setOxelInitializer(OxelInitializer initializer)
{
    this->initializer = initializer;
}

OxelTree::Creator OxelTree::Creator::child( uint8_t node_index ) const
{
    if( granularity <= std::numeric_limits<int8_t>::min()) return Creator();
    if(node_->isLeaf()) 
    { 
        node_->expand();
        for(uint8_t i = 0; i<8 ; ++i) { initializer( this->child(i) ); }
    }

    Creator ret = *this;
    ret.node_ = node_->child(node_index);
    ret.center = OxelTree::calculateChildCenter(granularity, center, node_index);
    --ret.granularity;
    return ret;
}

OxelTree::Creator OxelTree::Creator::parent() const
{
    if( granularity >= std::numeric_limits<int8_t>::max() ) return Creator();
    if( node_->isRoot() ) tree->tree.higher();

    Creator ret;
    ret.node_ = node_->parent();
    ret.center = calculateParentCenter(granularity, center, node_->nodeIndex());
    ++ret.granularity;
    return ret;
}

OxelTree::Creator OxelTree::Creator::decendant( const vector<uint8_t>& decendant_route ) const
{
    if(decendant_route.empty()) return *this;
    if( std::numeric_limits<int8_t>::min() + (int8_t) decendant_route.size() > granularity ) return Creator();
    if(granularity >= std::numeric_limits<int8_t>::min()) return Creator();
    Creator ret = *this;
    for(uint8_t direction : decendant_route)
    {
        if(!ret) return ret;
        ret = ret.child(direction);
    }
    return ret;
}

OxelTree::Creator OxelTree::Creator::ancestor( uint8_t generations ) const
{
    if( std::numeric_limits<int8_t>::max() - generations < granularity ) return Creator();
    Creator ret = *this;
    for (size_t i = 0; i < generations; i++)
    {
        if(!ret) return ret;
        ret = ret.parent();
    }
    return ret;
}

OxelTree::Creator OxelTree::Creator::cousin( int64_t x, int64_t y, int64_t z ) const
{
    if( !*this ) return Creator();
    if( !x && !y && !z ) return *this;

    uint8_t target_node_index = this->node_->nodeIndex() ^
    x && x%2 ? 0x1 : 0x0 ^
    y && y%2 ? 0x2 : 0x0 ^
    z && z%2 ? 0x4 : 0x0;

    x = signedrshift(x, 1);
    y = signedrshift(y, 1);
    z = signedrshift(z, 1);

    Creator ret;
    if(x || y || z)     ret = this->cousin(x, y, z);
    else                ret = parent();
    if(!ret)            return ret;
    return ret.child(target_node_index);
}