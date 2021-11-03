#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include "SelectiveRepeatArqTest.cpp"
#include "SelectiveRepeatArqProcessTest.cpp"
#include "PacketUtilsTest.cpp"
#include "End2EndTest.cpp"

using namespace std;

int main(int argc, const char* argv[]) {
    CppUnit::TextUi::TestRunner runner;

    //runner.addTest(PacketUtilsTest::suite());
    //runner.addTest(SelectiveRepeatArqProcessTest::suite());
    runner.addTest(End2EndTest::suite());

    runner.run();
    return runner.result().wasSuccessful() ? 0 : 1;
}
