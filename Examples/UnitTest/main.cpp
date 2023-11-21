
#include <iostream>
#include "test.h"
#include "test_simple.h"

Test_simple test_simple;

int main(int argc, char** argv)
{
	std::cout << "Running "<< Test::getTests().size() << " tests...\n";
	Test::TestResults results;
	Test::runAllTests(results);
	Test::printResults(results);

	return 0;
}