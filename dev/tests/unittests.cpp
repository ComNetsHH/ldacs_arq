#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "SelectiveRepeatArqTest.cpp"
#include "SelectiveRepeatArqProcessTest.cpp"
#include "PacketUtilsTest.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    //    CppUnit::TestResult result;
//    CppUnit::TestResultCollector collectedResults;
//    CppUnit::BriefTestProgressListener progress;
    CppUnit::TextUi::TestRunner runner;

//    result.addListener (&collectedResults);
//    result.addListener (&progress);

    runner.addTest(PacketUtilsTest::suite());
    runner.addTest(SelectiveRepeatArqProcessTest::suite());

//    runner.run(result);
    runner.run();
    return runner.result().wasSuccessful() ? 0 : 1;
//    return collectedResults.wasSuccessful() ? 0 : 1;

}
