//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cstdint>
#include <iostream>
#include "../SequenceNumber.hpp"

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

class SequenceNumberTests : public CppUnit::TestFixture {
public:
    void testModulo() {
        SequenceNumber seqno = SequenceNumber();
        CPPUNIT_ASSERT_EQUAL(SEQNO_UNSET, seqno.get());
        for (uint8_t i = 0; i < SEQNO_MAX; i++) {
            CPPUNIT_ASSERT_EQUAL(i, seqno.get());
            seqno.increment();
        }
        CPPUNIT_ASSERT_EQUAL(SEQNO_FIRST, seqno.get());

    }

    void testCompare() {
        uint8_t windowSize = 100;
        SequenceNumber seqno1 = SequenceNumber(1);
        SequenceNumber seqno2 = SequenceNumber(1);
        SequenceNumber seqno3 = SequenceNumber(2);
        SequenceNumber seqno4 = SequenceNumber(SEQNO_MAX);
        SequenceNumber seqno5 = SequenceNumber(SEQNO_FIRST + 30);
        SequenceNumber seqno6 = SequenceNumber(SEQNO_FIRST + 100);

        CPPUNIT_ASSERT(seqno1 == seqno2);
        CPPUNIT_ASSERT(!(seqno1 == seqno3));
        CPPUNIT_ASSERT(seqno1.isLowerThan(seqno3, windowSize));
        CPPUNIT_ASSERT(seqno3.isHigherThan(seqno1, windowSize));
        CPPUNIT_ASSERT(seqno5.isHigherThan(seqno1, windowSize));
        CPPUNIT_ASSERT(!seqno6.isHigherThan(seqno1, windowSize));
        CPPUNIT_ASSERT(seqno4.isLowerThan(seqno1, windowSize));
    }

    void testArithmetic() {
        SequenceNumber seqno1 = SequenceNumber(1);
        SequenceNumber seqno2 = seqno1 + 1;

        CPPUNIT_ASSERT_EQUAL(1, (int) seqno1.get());
        CPPUNIT_ASSERT_EQUAL(2, (int) seqno2.get());

        SequenceNumber seqno3 = seqno1 - 1;
        CPPUNIT_ASSERT_EQUAL(SEQNO_MAX, seqno3.get());

        SequenceNumber seqno4 = seqno3 + 1;
        CPPUNIT_ASSERT_EQUAL(seqno1.get(), seqno4.get());

    }

CPPUNIT_TEST_SUITE(SequenceNumberTests);
        CPPUNIT_TEST(testModulo);
        CPPUNIT_TEST(testCompare);
        CPPUNIT_TEST(testArithmetic);
    CPPUNIT_TEST_SUITE_END();
};