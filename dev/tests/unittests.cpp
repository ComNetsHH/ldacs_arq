#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include "ArqTests.cpp"
#include "SequenceNumberTests.cpp"
#include "L2SegmentHeaderTest.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    CppUnit::TestResult result;
    CppUnit::TestResultCollector collectedResults;
    CppUnit::BriefTestProgressListener progress;
    CppUnit::TextUi::TestRunner runner;

    result.addListener (&collectedResults);
    result.addListener (&progress);

	runner.addTest(ArqTests::suite());
    runner.addTest(SequenceNumberTests::suite());
    runner.addTest(L2SegmentHeaderTest::suite());
	runner.run(result);
	return collectedResults.wasSuccessful() ? 0 : 1;

}
