#include "test.h"
#include <iostream>

Test::Test(const std::string& name)
	: m_name(name)
	, m_breakTestOnFail(true)
{

}
Test::~Test()
{

}


bool Test::runTests()
{
	m_results = TestResults();
	return runTests(m_results);
}
bool Test::runTests(TestResults& results)
{
	results.name = m_name;
	results.subResults.reserve(m_testFunctions.size());

	bool success = true;
	for (size_t i = 0; i < m_testFunctions.size(); ++i)
	{
		TestResults r;
		success &= m_testFunctions[i](r);
		results.subResults.push_back(r);
	}
	for (size_t i = 0; i < m_subTests.size(); ++i)
	{
		TestResults r;
		success &= m_subTests[i]->runTests(r);
		results.subResults.push_back(r);
	}
	m_results = results;
	return success;
}

const Test::TestResults& Test::getResults() const
{
	return m_results;
}

void Test::setBreakOnFail(bool breakOnFail)
{
	m_breakTestOnFail = breakOnFail;
}
bool Test::doesBreakOnFail() const
{
	return m_breakTestOnFail;
}

void Test::printResults() const
{
	printResultsRecursive(m_results, 0);
}
void Test::printResultsRecursive(const TestResults& results, int depth)
{
	for (int i = 0; i < depth; ++i)
		std::cout << "  ";
	std::cout << results.name << ": " << (results.success ? "Success" : "Failed") << std::endl;
	for (size_t i = 0; i < results.results.size(); ++i)
	{
		for (int i = 0; i < depth; ++i)
			std::cout << "  ";
		std::string stateString;
		switch(results.results[i].state)
		{ 
			case ResultState::success:
			stateString = "Success";
			break;
			case ResultState::error:
			stateString = "Error";
			break;
			case ResultState::none:
			stateString = "None";
			break;
		}
		std::cout << "  " << results.results[i].message << ": " << stateString << std::endl;
	}
	for (size_t i = 0; i < results.subResults.size(); ++i)
	{
		printResultsRecursive(results.subResults[i], depth + 1);
	}
}

/*void Test::addTest(TestFunction func)
{
	m_testFunctions.push_back(func);
}*/
void Test::addTest(Test* subTest)
{
	m_subTests.push_back(subTest);
}
