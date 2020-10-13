#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "SelectiveRepeatArqTest.cpp"
#include "SelectiveRepeatArqProcessTest.cpp"
#include "SequenceNumberTests.cpp"
#include "L2SegmentHeaderTest.cpp"
#include "E2ETest.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    CppUnit::TestResult result;
    CppUnit::TestResultCollector collectedResults;
    CppUnit::BriefTestProgressListener progress;
    CppUnit::TextUi::TestRunner runner;

    result.addListener (&collectedResults);
    result.addListener (&progress);

    runner.addTest(SelectiveRepeatArqTest::suite());
    runner.addTest(SelectiveRepeatArqProcessTest::suite());
    runner.addTest(SequenceNumberTests::suite());
    runner.addTest(L2SegmentHeaderTest::suite());
    runner.addTest(E2ETest::suite());
	runner.run(result);
	return collectedResults.wasSuccessful() ? 0 : 1;

}
