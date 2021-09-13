//
// Created by Konrad Fuger on 13.10.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArqProcess.hpp"
#include "../PacketUtils.hpp"
#include "L2Packet.hpp"

#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace TUHH_INTAIRNET_MCSOTDMA;

class SelectiveRepeatArqProcessTest : public CppUnit::TestFixture {

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
    void addSegmentInOrder() {
        // Add a single segment in order
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber arq_ack_no = SequenceNumber(51);
        unsigned int arq_ack_slot = 52;
        L2HeaderUnicast unicast_header = L2HeaderUnicast(dest_id, use_arq, arq_seqno, arq_ack_no, arq_ack_slot);
        TestPayload payload = TestPayload();
        PacketFragment s1 = make_pair(&unicast_header, &payload);
        SelectiveRepeatArqProcess process(MacId(1));
        process.processLowerLayerSegment(s1);
        auto segments = process.getInOrderSegments();
        CPPUNIT_ASSERT_EQUAL(1, (int) segments.size());
    }

    void addSegmentOutOfOrder() {
        // add two segments in reverse order
        MacId dest_id = MacId(99);
        TestPayload payload = TestPayload();
        L2HeaderUnicast h1 = L2HeaderUnicast(dest_id,
                                                         true,
                                                         SequenceNumber(2),
                                                         SequenceNumber(SEQNO_UNSET),
                                                         100);
        auto s1 = make_pair(&h1, &payload);
        SelectiveRepeatArqProcess process(MacId(1));
        process.processLowerLayerSegment(s1);
        auto segments = process.getInOrderSegments();

        // The first segment was added out of order. It cannot be retrieved
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        L2HeaderUnicast h2 = L2HeaderUnicast(dest_id,
                                             true,
                                             SequenceNumber(1),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s2 = make_pair(&h2, &payload);
        process.processLowerLayerSegment(s2);

        segments = process.getInOrderSegments();
        // Now the gap is filled and both segments are received
        CPPUNIT_ASSERT_EQUAL(2, (int) segments.size());
    }

    void addSeveralSegmentsOutOfOrder() {
        MacId dest_id = MacId(99);
        TestPayload payload = TestPayload();
        // Add three segments out of order (2,3,1)
        L2HeaderUnicast h1 = L2HeaderUnicast(dest_id,
                                             true,
                                             SequenceNumber(2),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s1 = make_pair(&h1, &payload);
        SelectiveRepeatArqProcess process(dest_id);
        process.processLowerLayerSegment(s1);
        auto segments = process.getInOrderSegments();

        // No in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        L2HeaderUnicast h2 = L2HeaderUnicast(dest_id,
                                             true,
                                             SequenceNumber(3),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s2 = make_pair(&h2, &payload);
        process.processLowerLayerSegment(s2);
        segments = process.getInOrderSegments();

        // Still no in order segment
        CPPUNIT_ASSERT_EQUAL(0, (int) segments.size());

        L2HeaderUnicast h3 = L2HeaderUnicast(dest_id,
                                             true,
                                             SequenceNumber(1),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s3 = make_pair(&h3, &payload);
        process.processLowerLayerSegment(s3);
        segments = process.getInOrderSegments();

        // Now we can get all three
        CPPUNIT_ASSERT_EQUAL(3, (int) segments.size());
    }

    void testHasRtxSegment() {
        SelectiveRepeatArqProcess process(MacId(1));
        CPPUNIT_ASSERT(!process.hasRtxSegment(10));
    }

    void testAddSegmentFromUpperLayer() {
        TestPayload payload = TestPayload();
        L2HeaderUnicast header = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(1),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto segment = make_pair(&header, &payload);
        SelectiveRepeatArqProcess process(MacId(1));
        process.processUpperLayerSegment(segment);
        CPPUNIT_ASSERT_EQUAL(1, (int) header.getSeqno().get());
        process.processUpperLayerSegment(segment);
        CPPUNIT_ASSERT_EQUAL(2, (int) header.getSeqno().get());
    }


    void testSelectiveRejection() {
        TestPayload payload = TestPayload();
        SelectiveRepeatArqProcess process(MacId(1));
        // Add three segments out of order (3,2,1)
        L2HeaderUnicast h1 = L2HeaderUnicast(MacId(1),
                                                 true,
                                                 SequenceNumber(3),
                                                 SequenceNumber(SEQNO_UNSET),
                                                 100);
        auto s1 = make_pair(&h1, &payload);
        process.processLowerLayerSegment(s1);


        auto selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(2, (int) selectiveRejections.size());

        L2HeaderUnicast h2 = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(2),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s2 = make_pair(&h2, &payload);
        process.processLowerLayerSegment(s2);

        selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(1, (int) selectiveRejections.size());

        L2HeaderUnicast h3 = L2HeaderUnicast(MacId(1),
                true,
                SequenceNumber(1),
                SequenceNumber(SEQNO_UNSET),
                100);
        auto s3 = make_pair(&h3, &payload);
        process.processLowerLayerSegment(s3);

        selectiveRejections = process.getSrejList();
        CPPUNIT_ASSERT_EQUAL(0, (int) selectiveRejections.size());
    }

    void handlesAck() {
        TestPayload payload = TestPayload();
        SelectiveRepeatArqProcess process(MacId(1));
        L2HeaderUnicast h1 = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(SEQNO_UNSET),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s1 = make_pair(&h1, &payload);
        process.processUpperLayerSegment(s1);

        L2HeaderUnicast h2 = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(SEQNO_UNSET),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s2 = make_pair(&h2, &payload);
        process.processUpperLayerSegment(s2);

        L2HeaderUnicast h3 = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(SEQNO_UNSET),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);
        auto s3 = make_pair(&h3, &payload);
        process.processUpperLayerSegment(s3);

        L2HeaderUnicast replyHeader = L2HeaderUnicast(MacId(1),
                                             true,
                                             SequenceNumber(SEQNO_UNSET),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        replyHeader.setSeqnoNextExpected(SequenceNumber(4));
        vector<SequenceNumber> srej = {SequenceNumber(2)};
        PacketUtils::setSrejList(&replyHeader, srej);
        auto reply = make_pair(&replyHeader, &payload);
        process.processLowerLayerSegment(reply);

        CPPUNIT_ASSERT(process.hasRtxSegment(1000));


        L2Packet *rtxSegment = process.getRtxSegment(1000);
        auto unicastHeaders = PacketUtils::getUnicastFragments(rtxSegment);

        cout << rtxSegment->print() << endl;

        CPPUNIT_ASSERT_EQUAL(1, (int)unicastHeaders.size());
        L2HeaderUnicast *rtxHeader = (L2HeaderUnicast*) unicastHeaders[0].first;

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