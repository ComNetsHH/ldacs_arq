//
// Created by Konrad Fuger on 13.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArqProcess.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class SelectiveRepeatArqProcessTest : public CppUnit::TestFixture {
public:
    void addSegmentSimple() {
    }

CPPUNIT_TEST_SUITE(SelectiveRepeatArqProcessTest);
        CPPUNIT_TEST(addSegmentSimple);
    CPPUNIT_TEST_SUITE_END();
};