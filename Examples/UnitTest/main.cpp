
#include "test.h"

class TestTest : public Test
{
public:
	TestTest()
		: Test("TestTest")
	{
			ADD_TEST(TestTest::test1);
		
	}
	// Dummy test

	bool test1(TestResults& results)
	{
		TEST_START(results);

		TEST_MESSAGE("This is a test message");

		TEST_END;
	}
};


int main(int argc, char** argv)
{
	TestTest t;
	t.runTests();
	Test::TestResults r = t.getResults();

	return 0;
}