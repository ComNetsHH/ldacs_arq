#include <cppunit/ui/text/TestRunner.h>
#include <iostream>
#include "ArqTests.cpp"
#include "SequenceNumberTests.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
	CppUnit::TextUi::TestRunner runner;
	runner.addTest(ArqTests::suite());
	runner.addTest(SequenceNumberTests::suite());
	runner.run();
	return 0;
}
