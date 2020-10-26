//
// Created by Konrad Fuger on 12.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cstdint>
#include <vector>
#include <iostream>

#include "../L2SegmentHeader.hpp"

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

class L2SegmentHeaderTest : public CppUnit::TestFixture {
public:
    void testBasics() {
        L2SegmentHeader header;
        SequenceNumber seqNo(1);

        vector<SequenceNumber> selRejList;
        selRejList.push_back(SequenceNumber(2));
        selRejList.push_back(SequenceNumber(3));

        header.setSeqno(seqNo);
        header.setSeqnoNextExpected(seqNo);
        header.setSrejList(selRejList);

        CPPUNIT_ASSERT_EQUAL(header.getSrejListLength(), uint8_t(2));
        CPPUNIT_ASSERT_EQUAL(header.getSrejList().size(), (unsigned long) 2);
        CPPUNIT_ASSERT_EQUAL(header.getSeqno().get(), seqNo.get());
        CPPUNIT_ASSERT_EQUAL(header.getSeqnoNextExpected().get(), seqNo.get());
    }


    void testSizeComputation() {
        L2SegmentHeader header;
        vector<SequenceNumber> selRejList;
        selRejList.push_back(SequenceNumber(2));
        selRejList.push_back(SequenceNumber(3));
        header.setSrejList(selRejList);

        CPPUNIT_ASSERT_EQUAL(header.getSize(), uint64_t(5));
    }

CPPUNIT_TEST_SUITE(L2SegmentHeaderTest);
        CPPUNIT_TEST(testBasics);
        CPPUNIT_TEST(testSizeComputation);
    CPPUNIT_TEST_SUITE_END();
};