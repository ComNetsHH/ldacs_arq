// The L-Band Digital Aeronautical Communications System (LDACS) Automatic Repeat Request (ARQ) implements the ARQ protocol for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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
        SelectiveRepeatArq arq = SelectiveRepeatArq(MacId(2),100, 100);
        TestPayload payload = TestPayload();
        L2HeaderPP h1 = L2HeaderPP(MacId(100),
                                             true,
                                             SequenceNumber(2),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 1);

        L2HeaderPP h2 = L2HeaderPP(MacId(100),
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
        SelectiveRepeatArq arq = SelectiveRepeatArq(MacId(2),100, 100);
        // Add a single segment in order
        TestPayload payload = TestPayload();
        L2HeaderPP h1 = L2HeaderPP(MacId(100),
                                             true,
                                             SequenceNumber(SEQNO_FIRST),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);
        L2HeaderPP h2 = L2HeaderPP(MacId(100),
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
        SelectiveRepeatArq arq = SelectiveRepeatArq(MacId(2),100, 100);
        // Add a single segment in order
        TestPayload payload = TestPayload();
        L2HeaderPP h1 = L2HeaderPP(MacId(100),
                                             true,
                                             SequenceNumber(13),
                                             SequenceNumber(SEQNO_UNSET),
                                             100);

        //h1.setSrcAddress(MacId(1));
        L2Packet s1;
        s1.addMessage(&h1, &payload);
        arq.receiveFromLower(&s1);
        L2HeaderPP h2 = L2HeaderPP(MacId(100),
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
        SelectiveRepeatArq arq = SelectiveRepeatArq(MacId(2),100, 100);
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