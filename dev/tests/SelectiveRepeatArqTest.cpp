//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArq.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class SelectiveRepeatArqTest : public CppUnit::TestFixture {
public:
	void createProcesses() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        L2SegmentHeader header = L2SegmentHeader(SequenceNumber());
        header.setSrcAddress(MacAddress(1));
        L2Segment segment = L2Segment(&header);

        arq.receiveFromLowerLayer(&segment);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 1);

        L2SegmentHeader header2 = L2SegmentHeader(SequenceNumber());
        header.setSrcAddress(MacAddress(2));
        L2Segment segment2 = L2Segment(&header);

        arq.receiveFromLowerLayer(&segment2);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 2);
	}

    void addSegmentInOrder() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // Add a single segment in order
        SequenceNumber seqNo(SEQNO_FIRST);
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        h1->setSrcAddress(MacAddress(2));
        L2Segment s1(h1);
        L2SegmentHeader *h2 = new L2SegmentHeader(seqNo);
        h2->setSrcAddress(MacAddress(1));
        L2Segment s2(h2);
        arq.receiveFromLowerLayer(&s1);
        arq.receiveFromLowerLayer(&s2);
        auto segments = arq.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(2, (int) segments.size());
    }

    void addSegmentOutOfOrder() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // Add a single segment in order
        SequenceNumber seqNo(12);
        L2SegmentHeader *h1 = new L2SegmentHeader(seqNo);
        h1->setSrcAddress(MacAddress(2));
        L2Segment s1(h1);
        L2SegmentHeader *h2 = new L2SegmentHeader(seqNo);
        h2->setSrcAddress(MacAddress(1));
        L2Segment s2(h2);
        arq.receiveFromLowerLayer(&s1);
        arq.receiveFromLowerLayer(&s2);
        auto segments = arq.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());
    }

    void addUpperLayerSegment() {

	}

    void checkRtx() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // check for RTX of MacAddress 0
        CPPUNIT_ASSERT_EQUAL(false, arq.hasRtxSegment(0, 100));
        CPPUNIT_ASSERT_EQUAL(false, arq.hasRtxSegment(1, 100));

        CPPUNIT_ASSERT(arq.getRtxSegment(0, 100) == nullptr);
        CPPUNIT_ASSERT(arq.getRtxSegment(1, 100) == nullptr);
	}
	
	CPPUNIT_TEST_SUITE(SelectiveRepeatArqTest);
        CPPUNIT_TEST(createProcesses);
        CPPUNIT_TEST(addSegmentInOrder);
        CPPUNIT_TEST(addSegmentOutOfOrder);
        CPPUNIT_TEST(checkRtx);
	CPPUNIT_TEST_SUITE_END();
};