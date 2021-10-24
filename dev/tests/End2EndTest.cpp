//
// Created by Konrad Fuger on 24.10.21.
//

//
// Created by Konrad Fuger on 22.02.21.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../SelectiveRepeatArq.hpp"
#include "../PacketUtils.hpp"
#include "./MockRlc.hpp"
#include <iostream>
#include <array>


using namespace std;
using namespace TUHH_INTAIRNET_ARQ;

class End2EndTest : public CppUnit::TestFixture {

    void print_srej_bitmap(array<bool, 16> bitmap) {
        cout << "Bitmap" << endl;
        for(int i = 0; i < 16; i++) {
            if(i < 10) {
                cout << " ";
            }
            cout << " "<< i;
        }

        cout << endl;

        for(int i = 0; i< 16; i++) {
            cout << (bitmap[i] ? "  1": "  0");
        }
        cout << endl;
    }

    void print_srej_list(vector<SequenceNumber> srej) {
        for(int i= 0; i< srej.size(); i++) {
            cout << i << " " << (int)(srej[i].get()) << endl;
        }
        cout << endl;
    }

public:
    void test() {
        MacId leftId(1);
        MacId rightId(2);
        auto leftArq = new SelectiveRepeatArq(100, 100);
        auto rightArq = new SelectiveRepeatArq(100, 100);
        auto leftRlc = new MockRlc(leftId);
        auto rightRlc = new MockRlc(leftId);

        leftArq->setUpperLayer((IRlc*)leftRlc);
        rightArq->setUpperLayer((IRlc*)rightRlc);

        for(int i = 0; i< 400; i++) {

            auto leftPacket = leftArq->requestSegment(100, rightId);
            rightArq->receiveFromLower(leftPacket);

            auto rightPacket = rightArq->requestSegment(100, leftId);
            leftArq->receiveFromLower(rightPacket);


            // -> print
            auto leftUnicastHeader = (L2HeaderUnicast*)(leftPacket->getHeaders()[1]);
            //cout << leftPacket->print() << " " << (int)(leftUnicastHeader->seqno.get()) <<endl;
        }


        CPPUNIT_ASSERT_EQUAL(2,2);

        delete leftArq;
        delete rightArq;
    }



CPPUNIT_TEST_SUITE(End2EndTest);
        CPPUNIT_TEST(test);
    CPPUNIT_TEST_SUITE_END();
};