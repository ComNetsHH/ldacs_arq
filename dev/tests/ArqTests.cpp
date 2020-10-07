//
// Created by Sebastian Lindner on 31.08.20.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../tuhh_intairnet_arq.h"

class ArqTests : public CppUnit::TestFixture {
public:
	void setUp() override {
	
	}
	
	void tearDown() override {
	
	}
	
	void test1() {
		hello();
	}
	
	CPPUNIT_TEST_SUITE(ArqTests);
		CPPUNIT_TEST(test1);
	CPPUNIT_TEST_SUITE_END();
};