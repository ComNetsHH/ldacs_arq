//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArq.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class SelectiveRepeatArqTest : public CppUnit::TestFixture {
public:
	void addSegmentSimple() {
        SelectiveRepeatArq arq = SelectiveRepeatArq(100, 100);
        L2SegmentHeader header = L2SegmentHeader(SequenceNumber());
        header.setSrcAddress(MacAddress(1));
        L2Segment segment = L2Segment(header);

        arq.receiveFromLowerLayer(segment);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 1);

        L2SegmentHeader header2 = L2SegmentHeader(SequenceNumber());
        header.setSrcAddress(MacAddress(2));
        L2Segment segment2 = L2Segment(header);

        arq.receiveFromLowerLayer(segment2);

        CPPUNIT_ASSERT_EQUAL(arq.getNumProcesses(), 2);
	}
	
	CPPUNIT_TEST_SUITE(SelectiveRepeatArqTest);
		CPPUNIT_TEST(addSegmentSimple);
	CPPUNIT_TEST_SUITE_END();
};