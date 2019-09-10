#include "../src/CumulativeMat.hpp"

#include <gtest/gtest.h>
#include <glm/ext.hpp>

struct test_CumulativeMat : public ::testing::Test
{
	CumulativeMat m;
public:
	test_CumulativeMat()
	{

	}

	~test_CumulativeMat()
	{
	
	}
};

TEST_F(test_CumulativeMat, stack)
{
	ASSERT_EQ(0, m.stacksize());
	m.push();
	ASSERT_EQ(1, m.stacksize());
	ASSERT_EQ(m.matstack()[0], glm::mat4(1.0f));
	m.pop();
	ASSERT_EQ(0, m.stacksize());
}

TEST_F(test_CumulativeMat, default_construct)
{
	//tested in fixture
	ASSERT_EQ(0, m.stacksize());
	ASSERT_EQ((glm::mat4) m , mat4(1.0f));
}


TEST_F(test_CumulativeMat, loadZero)
{
	m.loadZero();
	ASSERT_EQ((glm::mat4) m , mat4());
}

TEST_F(test_CumulativeMat, set)
{
	glm::mat4& mr = m;
	mr = glm::mat4(4.0f);
	ASSERT_EQ(mr, glm::mat4(4.0f));
}

TEST_F(test_CumulativeMat, loadIdentity)
{
	//since its the default we set it a different value first
	glm::mat4& mr = m;
    mr = glm::mat4(4.0f);
	ASSERT_EQ(mr, glm::mat4(4.0f));

	//then check loadIdentity
	m.loadIdentity();
	ASSERT_EQ(mr, glm::mat4(1.0f));
}

TEST_F(test_CumulativeMat, glm_functions_applicable)
{
	glm::mat4& mr = m;
	mr = glm::scale(mr, glm::vec3(2.0f, 3.0f, 4.0f));
	float fa [16] = {
						2.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 3.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 4.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f 
					};
	ASSERT_EQ((glm::mat4) m, glm::make_mat4(fa));
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
