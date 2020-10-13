//
// Created by Konrad Fuger on 13.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../L2Segment.hpp"
#include "../SelectiveRepeatArq.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class SimulationKernel {
public:
    SelectiveRepeatArq sender;
    SelectiveRepeatArq receiver;

/*    SimulationKernel(SelectiveRepeatArq sender, SelectiveRepeatArq receiver) {
        this->sender = sender;
        this->receiver = receiver;
    }*/

    void run() {
        //L2Segment this.sender.getSegment();
    }

};

class E2ETest : public CppUnit::TestFixture {
public:
    void e2e() {

        // intsantiate

    }

CPPUNIT_TEST_SUITE(E2ETest);
        CPPUNIT_TEST(e2e);
    CPPUNIT_TEST_SUITE_END();
};