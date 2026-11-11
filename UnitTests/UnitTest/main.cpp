
#include <iostream>
#include "tests.h"

TEST_INSTANTIATE(Test_simple);
TEST_INSTANTIATE(Test_nasted);
TEST_INSTANTIATE(Test_types);
TEST_INSTANTIATE(Test_defaults);
TEST_INSTANTIATE(Test_validation);

int main(int argc, char** argv)
{
    std::cout << "Running "<< UnitTest::Test::getTests().size() << " tests...\n";
    UnitTest::Test::TestResults results;
    UnitTest::Test::runAllTests(results);
    UnitTest::Test::printResults(results);

    return results.getSuccess();
}
