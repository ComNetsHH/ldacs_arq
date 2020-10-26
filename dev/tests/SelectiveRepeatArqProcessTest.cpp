//
// Created by Konrad Fuger on 13.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArqProcess.hpp"

#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;

class SelectiveRepeatArqProcessTest : public CppUnit::TestFixture {
public:
    void addSegmentInOrder() {
        // Add a single segment in order
        SequenceNumber seqNo(SEQNO_FIRST);
        L2SegmentHeader h1(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.addSegment(s1);
        auto segments = process.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(1, (int) segments.size());
    }

    void addSegmentOutOfOrder() {
        // add two segments in reverse order
        SequenceNumber seqNo(SequenceNumber(2));
        L2SegmentHeader h1(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.addSegment(s1);
        auto segments = process.getInOrderSegments();

        // The first segment was added out of order. It cannot be retrieved
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo2(SequenceNumber(1));
        L2SegmentHeader h2(seqNo2);
        L2Segment s2(h2);
        process.addSegment(s2);

        segments = process.getInOrderSegments();
        // Now the gap is filled and both segments are received
        CPPUNIT_ASSERT_EQUAL(2, (int) segments.size());
    }

    void addSeveralSegmentsOutOfOrder() {
        // Add three segments out of order (2,3,1)
        SequenceNumber seqNo(SequenceNumber(2));
        L2SegmentHeader h1(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.addSegment(s1);
        auto segments = process.getInOrderSegments();

        // No in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo2(SequenceNumber(3));
        L2SegmentHeader h2(seqNo2);
        L2Segment s2(h2);
        process.addSegment(s2);
        segments = process.getInOrderSegments();

        // Still no in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo3(SequenceNumber(1));
        L2SegmentHeader h3(seqNo3);
        L2Segment s3(h3);
        process.addSegment(s3);
        segments = process.getInOrderSegments();

        // Now we can get all three
        CPPUNIT_ASSERT_EQUAL(3, (int) segments.size());
    }

CPPUNIT_TEST_SUITE(SelectiveRepeatArqProcessTest);
        CPPUNIT_TEST(addSegmentInOrder);
        CPPUNIT_TEST(addSegmentOutOfOrder);
        CPPUNIT_TEST(addSeveralSegmentsOutOfOrder);
    CPPUNIT_TEST_SUITE_END();
};