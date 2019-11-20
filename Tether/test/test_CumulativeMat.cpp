#include "../src/CumulativeMat.hpp"

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../src/glmutils.hpp"

using glm::vec3;
using glm::mat4;

struct test_CumulativeMat : public ::testing::Test
{
	CumulativeMat m;
	mat4 m2;
public:
	test_CumulativeMat()
		: m2(1.0f, 3.0f, 4.0f, 2.0f,
			 10.0f, 0.5f, -2.5f, -1.0f,
			 1.0f, 2.0f, 3.0f, 4.0f,
			 5.0f, 6.0f, 7.0f, 16.0f)
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
	ASSERT_EQ(m.current() , mat4(1.0f));
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
	m = glm::scale(m, glm::vec3(2.0f, 3.0f, 4.0f));
	float fa [16] = {
						2.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 3.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 4.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f 
					};
	ASSERT_EQ((glm::mat4) m, glm::make_mat4(fa));
}

TEST_F(test_CumulativeMat, glm_multiple_multiple_functions)
{
	mat4 ret1 = m2 
		* glm::rotate(glm::radians(30.0f), vec3(1, 0, 0))
		* glm::rotate(glm::radians(60.0f), vec3(0, 1, 0))
		* glm::rotate(glm::radians(90.0f), vec3(0, 0, 1));
	
	mat4 ret2 = m2 
		* 
		(
		glm::rotate(glm::radians(30.0f), vec3(1, 0, 0))
		* glm::rotate(glm::radians(60.0f), vec3(0, 1, 0))
		* glm::rotate(glm::radians(90.0f), vec3(0, 0, 1))
		);
	
	
	EXPECT_TRUE(glm::matEqualsE(ret1, ret2, 0.00001f)) << '\n' << ret1 << '\n' << ret2 << '\n';
}

int main (int argc , char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
