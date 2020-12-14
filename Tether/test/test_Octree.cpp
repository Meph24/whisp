#include <gtest/gtest.h>

#include "../src/Octree.hpp"

#include <string>

using std::string;

struct test_Octree_example : public ::testing::Test
{

    Octree<string> t, t_min;

    test_Octree_example ()
    {
        Octree<string>::Node* n = &t.root;
        string prefix = "0";
        n->value = prefix;
        for ( size_t i = 0; i < 5; i++ )
		{
            n->expand();
            for (size_t i = 0; i < 8; i++)
            {
                n->child(i)->value = prefix + std::to_string(i);
            }

            n = n->child(0);
            prefix = prefix + "0";
		}
    }
    ~test_Octree_example()
    {

    }
};

TEST(test_Octree, default_initialization)
{
    Octree<int> t;
    EXPECT_TRUE( t.root.isLeaf() );
    EXPECT_TRUE( t.root.isRoot() );
    EXPECT_EQ( 0, t.root.nodeIndex() );
    EXPECT_TRUE( t.root.ancestry().empty() );
    EXPECT_EQ( nullptr, t.root.children() );
    EXPECT_EQ( nullptr, t.root[0] );
}

TEST_F(test_Octree_example, root_properties)
{
    EXPECT_TRUE( t.root.isRoot() );
    EXPECT_EQ( 0, t.root.nodeIndex() );
    EXPECT_TRUE( t.root.ancestry().empty() );
    EXPECT_EQ( nullptr, t.root.parent() );
    EXPECT_EQ( nullptr, t.root.cousin(123, 456, 789) ); //root doesn't have cousins at all
}

TEST_F(test_Octree_example, leaf_properties)
{
    Octree<string>::Node& l = *t.root.child(1); //we only expanded at 0, so 1 is leaf
    EXPECT_TRUE( l.isLeaf() );
    EXPECT_EQ( nullptr, l.children() );
    for (size_t i = 0; i < 8; i++)
    { 
        EXPECT_EQ( nullptr, l.child(i) ); 
        EXPECT_EQ( nullptr, l[i] ); 
    }
}

TEST_F(test_Octree_example, collapse_node)
{
    EXPECT_FALSE( t.root.isLeaf() );
    t.root.collapse();
    EXPECT_TRUE( t.root.isLeaf() );
}

TEST(test_Octree, expand_node)
{
    Octree<int> t;
    EXPECT_TRUE( t.root.isLeaf() );
    t.root.expand();
    EXPECT_FALSE( t.root.isLeaf() );
}

TEST_F(test_Octree_example, node_index)
{
    
}

string test_postorderString( Octree<string>& t )
{
    string s;
    int i = 0;
    for(string& ps : t)
    {
        s += (!i?"":" ") + ps;
        ++i;
    }
    return s;
}

TEST_F(test_Octree_example, postorder_sorted)
{
    ASSERT_TRUE( 
        std::is_sorted( t.begin(), t.end(), 
                        [](const string& a, const string& b){ return a.compare(b) < 0; } ) 
        ) << test_postorderString(t);
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}