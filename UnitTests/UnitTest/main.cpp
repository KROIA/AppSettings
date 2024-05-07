
#include <iostream>
#include "tests.h"

TEST_INSTANTIATE(Test_simple);
TEST_INSTANTIATE(Test_nasted);

int main(int argc, char** argv)
{
    std::cout << "Running "<< UnitTest::Test::getTests().size() << " tests...\n";
    UnitTest::Test::TestResults results;
    UnitTest::Test::runAllTests(results);
    UnitTest::Test::printResults(results);

	return 0;
}
