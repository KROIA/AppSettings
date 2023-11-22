
#include <iostream>
#include "tests.h"

TEST_INSTANTIATE(Test_simple);

int main(int argc, char** argv)
{
	std::cout << "Running "<< Test::getTests().size() << " tests...\n";
	Test::TestResults results;
	Test::runAllTests(results);
	Test::printResults(results);

	return 0;
}