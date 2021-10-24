//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArq.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class SelectiveRepeatArqTest : public CppUnit::TestFixture {

    class TestPayload : public L2Packet::Payload {
        unsigned int getBits() const override {
            return 1;
        }

        L2Packet::Payload* copy() const override {
            auto* copy = new TestPayload();
            return copy;
        };
    };

public:
	void createProcesses() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        TestPayload payload = TestPayload();
        L2HeaderUnicast h1 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(2),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 1);

        L2HeaderUnicast h2 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(2),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h2.setSrcAddress(MacId(2));
        L2Packet s2;
        s2.addMessage(&h2, &payload);

        arq.receiveFromLower(&s2);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 2);
	}

    void addSegmentInOrder() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // Add a single segment in order
        TestPayload payload = TestPayload();
        L2HeaderUnicast h1 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(SEQNO_FIRST),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);
        L2HeaderUnicast h2 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(SEQNO_FIRST),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(2));
        L2Packet s2;
        s2.addMessage(&h2, &payload);
        arq.receiveFromLower(&s1);
        arq.receiveFromLower(&s2);
        auto segments = arq.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(2, (int) segments.size());
    }

    void addSegmentOutOfOrder() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // Add a single segment in order
        TestPayload payload = TestPayload();
        L2HeaderUnicast h1 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(13),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);
        L2HeaderUnicast h2 = L2HeaderUnicast(MacId(100),
                                             true,
                                             SequenceNumber(12),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(2));
        L2Packet s2;
        s2.addMessage(&h2, &payload);
        arq.receiveFromLower(&s1);
        arq.receiveFromLower(&s2);
        auto segments = arq.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());
    }

    void addUpperLayerSegment() {

	}

    void checkRtx() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        // check for RTX of MacAddress 0
        CPPUNIT_ASSERT_EQUAL(false, arq.hasRtxSegment(MacId(0), 100));
        CPPUNIT_ASSERT_EQUAL(false, arq.hasRtxSegment(MacId(1), 100));

        CPPUNIT_ASSERT(arq.getRtxSegment(MacId(0), 100) == nullptr);
        CPPUNIT_ASSERT(arq.getRtxSegment(MacId(1), 100) == nullptr);
	}
	
	CPPUNIT_TEST_SUITE(SelectiveRepeatArqTest);
        CPPUNIT_TEST(createProcesses);
        CPPUNIT_TEST(addSegmentInOrder);
        CPPUNIT_TEST(addSegmentOutOfOrder);
        CPPUNIT_TEST(checkRtx);
	CPPUNIT_TEST_SUITE_END();
};