#ifndef OCTREE_HPP
#define OCTREE_HPP

#include <array>
#include <vector>
#include <stdexcept>
#include <string>
#include "clone_ptr.h"

#include "glmutils.hpp"
#include "Spacevec.h"

using std::array;
using std::vector;
using std::string;

template<typename T>
struct Octree
{
    class Node
    {
        clone_ptr<array<Node, 8>> Mchildren;
        Node* Mparent;

        Node();
        Node(Node& parent);
    public:
        Node(const Node&) = default;
        Node(Node&&) = default;

        Node& operator=( const Node& ) = default;
        Node& operator=( Node&& ) = default;    

        T value;

        bool isLeaf() const;
        bool isRoot() const;
        uint8_t nodeIndex() const;

        vector<Node*> ancestry();
        Node* parent();
        array<Node, 8>* children();
        Node* child( uint8_t );
        Node* operator[]( uint8_t );
        Node* cousin(int64_t x, int64_t y, int64_t z);

        void collapse();
        void expand();

        friend class Octree;
    } root;

    Octree() = default;

    bool higher( uint8_t supplant_index = 0 );
    void lower( uint8_t supplant_index );

    class PostOrderIterator
    {
        Octree::Node* node;
        Octree::Node* tree_root;

        void advancePostorder();
    public:
        PostOrderIterator(); // is enditerator
        PostOrderIterator( Node* tree_root );

        Octree<T>::PostOrderIterator& operator++();
        Octree<T>::PostOrderIterator operator++(int);

        T& operator*();
        T* operator->();

        bool operator< (const PostOrderIterator&) const;
        bool operator>= (const PostOrderIterator&) const;
        bool operator> (const PostOrderIterator&) const;
        bool operator<= (const PostOrderIterator&) const;
        bool operator== (const PostOrderIterator&) const;
        bool operator!= (const PostOrderIterator&) const;
    };

    typedef PostOrderIterator iterator;
    typedef iterator Iterator;
    Iterator begin();
    Iterator end();
};

#include "Octree.imp"

#endif // OCTREE_HPP
