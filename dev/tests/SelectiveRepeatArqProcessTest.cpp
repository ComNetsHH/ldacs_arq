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
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.processLowerLayerSegment(&s1);
        auto segments = process.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(1, (int) segments.size());
    }

    void addSegmentOutOfOrder() {
        // add two segments in reverse order
        SequenceNumber seqNo(SequenceNumber(2));
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.processLowerLayerSegment(&s1);
        auto segments = process.getInOrderSegments();

        // The first segment was added out of order. It cannot be retrieved
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo2(SequenceNumber(1));
        L2SegmentHeader *h2 = new L2SegmentHeader(seqNo2);
        L2Segment s2(h2);
        process.processLowerLayerSegment(&s2);

        segments = process.getInOrderSegments();
        // Now the gap is filled and both segments are received
        CPPUNIT_ASSERT_EQUAL(2, (int) segments.size());
    }

    void addSeveralSegmentsOutOfOrder() {
        // Add three segments out of order (2,3,1)
        SequenceNumber seqNo(SequenceNumber(2));
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);
        process.processLowerLayerSegment(&s1);
        auto segments = process.getInOrderSegments();

        // No in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo2(SequenceNumber(3));
        L2SegmentHeader *h2 = new L2SegmentHeader(seqNo2);
        L2Segment s2(h2);
        process.processLowerLayerSegment(&s2);
        segments = process.getInOrderSegments();

        // Still no in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        SequenceNumber seqNo3(SequenceNumber(1));
        L2SegmentHeader *h3 = new L2SegmentHeader(seqNo3);
        L2Segment s3(h3);
        process.processLowerLayerSegment(&s3);
        segments = process.getInOrderSegments();

        // Now we can get all three
        CPPUNIT_ASSERT_EQUAL(3, (int) segments.size());
    }

    void testHasRtxSegment() {
        SelectiveRepeatArqProcess process(1);
        CPPUNIT_ASSERT(!process.hasRtxSegment(10));
    }

    void testAddSegmentFromUpperLayer() {
        L2SegmentHeader header(1);
        L2Segment *segment = new L2Segment(&header);
        SelectiveRepeatArqProcess process(1);
        process.processUpperLayerSegment(segment);
        CPPUNIT_ASSERT_EQUAL(1, (int) header.getSeqno().get());
        process.processUpperLayerSegment(segment);
        CPPUNIT_ASSERT_EQUAL(2, (int) header.getSeqno().get());
    }


    void testSelectiveRejection() {
        // Add three segments out of order (3,2,1)
        SequenceNumber seqNo(SequenceNumber(3));
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        L2Segment s1(h1);
        SelectiveRepeatArqProcess process(1);

        process.processLowerLayerSegment(&s1);


        auto selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(2, (int) selectiveRejections.size());

        SequenceNumber seqNo2(SequenceNumber(2));
        L2SegmentHeader *h2 = new L2SegmentHeader(seqNo2);
        L2Segment s2(h2);
        process.processLowerLayerSegment(&s2);

        selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(1, (int) selectiveRejections.size());

        SequenceNumber seqNo3(SequenceNumber(1));
        L2SegmentHeader *h3 = new L2SegmentHeader(seqNo3);
        L2Segment s3(h3);
        process.processLowerLayerSegment(&s3);

        selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(0, (int) selectiveRejections.size());
    }

    void handlesAck() {
        SelectiveRepeatArqProcess process(1);
        SequenceNumber seqNo1(SEQNO_UNSET);
        L2SegmentHeader h1(seqNo1);
        L2Segment s1(&h1);
        process.processUpperLayerSegment(&s1);

        SequenceNumber seqNo2(SEQNO_UNSET);
        L2SegmentHeader h2(seqNo1);
        L2Segment s2(&h2);
        process.processUpperLayerSegment(&s2);

        SequenceNumber seqNo3(SEQNO_UNSET);
        L2SegmentHeader h3(seqNo3);
        L2Segment s3(&h3);
        process.processUpperLayerSegment(&s3);

        SequenceNumber replySeqNo(SEQNO_UNSET);
        L2SegmentHeader replyHeader(replySeqNo);
        replyHeader.setSeqnoNextExpected(SequenceNumber(4));
        vector<SequenceNumber> srej = {SequenceNumber(2)};
        replyHeader.setSrejList(srej);
        L2Segment reply(&replyHeader);
        process.processLowerLayerSegment(&reply);

        CPPUNIT_ASSERT(process.hasRtxSegment(100));

        L2Segment *rtxSegment = process.getRtxSegment(100);
        L2SegmentHeader *rtxHeader = rtxSegment->getHeader();

        CPPUNIT_ASSERT(rtxHeader->getSeqno() == h2.getSeqno());
        CPPUNIT_ASSERT(!process.hasRtxSegment(100));

    }

CPPUNIT_TEST_SUITE(SelectiveRepeatArqProcessTest);
        CPPUNIT_TEST(addSegmentInOrder);
        CPPUNIT_TEST(addSegmentOutOfOrder);
        CPPUNIT_TEST(addSeveralSegmentsOutOfOrder);
        CPPUNIT_TEST(testHasRtxSegment);
        CPPUNIT_TEST(testAddSegmentFromUpperLayer);
        CPPUNIT_TEST(testSelectiveRejection);
        CPPUNIT_TEST(handlesAck);
    CPPUNIT_TEST_SUITE_END();
};