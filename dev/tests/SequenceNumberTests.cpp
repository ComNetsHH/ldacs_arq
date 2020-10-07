//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cstdint>
#include "../SequenceNumber.hpp"

using namespace TUHH_INTAIRNET_ARQ;

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
	
	CPPUNIT_TEST_SUITE(SequenceNumberTests);
		CPPUNIT_TEST(testModulo);
	CPPUNIT_TEST_SUITE_END();
};