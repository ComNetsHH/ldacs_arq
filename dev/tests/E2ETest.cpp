//
// Created by Konrad Fuger on 13.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../L2Segment.hpp"
#include "../SelectiveRepeatArq.hpp"
#include "mock/MockHost.h"

using namespace TUHH_INTAIRNET_ARQ;

class E2ETest : public CppUnit::TestFixture {
public:
    void e2e() {
        Channel *channel = new Channel;
        MockHost sender(1, channel);
        MockHost receiver(2, channel);

    }

CPPUNIT_TEST_SUITE(E2ETest);
        CPPUNIT_TEST(e2e);
    CPPUNIT_TEST_SUITE_END();
};