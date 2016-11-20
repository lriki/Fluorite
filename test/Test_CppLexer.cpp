#include "TestConfig.h"

class Test_Parser_CppLexer : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
	}
	virtual void TearDown() {}

	AnalyzerContext	m_context;
};

//------------------------------------------------------------------------------
TEST_F(Test_Parser_CppLexer, Basic)
{
	// <Test> Ž¯•ÊŽq
	{
		m_context.RegisterInputMemoryCode("test", "abc");

	}
}

