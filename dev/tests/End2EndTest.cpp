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
    void wrapAroundTest1() {
        MacId leftId(2);
        MacId rightId(3);
        auto leftArq = new SelectiveRepeatArq(leftId, 100, 100);
        auto rightArq = new SelectiveRepeatArq(rightId, 100, 100);
        auto leftRlc = new MockRlc(leftId);
        auto rightRlc = new MockRlc(rightId);

        leftArq->setUpperLayer((IRlc*)leftRlc);
        rightArq->setUpperLayer((IRlc*)rightRlc);

        int lastSeqNoLeft = 0;
        int lastSeqNoRight = 0;

        for(int i = 0; i< 260; i++) {

            auto leftPacket = leftArq->requestSegment(1600, rightId);

            if(i != 253) {
                rightArq->receiveFromLower(leftPacket);
            }

            auto rightPacket = rightArq->requestSegment(1600, leftId);
            leftArq->receiveFromLower(rightPacket);

            auto leftUnicastHeader = (L2HeaderPP*)(leftPacket->getHeaders()[1]);
            auto rightUnicastHeader = (L2HeaderPP*)(rightPacket->getHeaders()[1]);

            lastSeqNoLeft = (int)(leftUnicastHeader->seqno.get());
            lastSeqNoRight = (int)(rightUnicastHeader->seqno.get());
        }


        CPPUNIT_ASSERT_EQUAL(lastSeqNoRight,lastSeqNoLeft+1);

        delete leftArq;
        delete rightArq;
    }

    void wrapAroundTest2() {
        MacId leftId(2);
        MacId rightId(3);
        auto leftArq = new SelectiveRepeatArq(leftId, 100, 100);
        auto rightArq = new SelectiveRepeatArq(rightId, 100, 100);
        auto leftRlc = new MockRlc(leftId);
        auto rightRlc = new MockRlc(rightId);

        leftArq->setUpperLayer((IRlc*)leftRlc);
        rightArq->setUpperLayer((IRlc*)rightRlc);

        int lastSeqNoLeft = 0;
        int lastSeqNoRight = 0;

        for(int i = 0; i< 249; i++) {

            auto leftPacket = leftArq->requestSegment(1600, rightId);
            rightArq->receiveFromLower(leftPacket);

            auto rightPacket = rightArq->requestSegment(1600, leftId);
            leftArq->receiveFromLower(rightPacket);
        }

        for (int i = 0; i< 14; i++) {
            cout << "###### " << i << " ######" << endl;
            auto leftPacket = leftArq->requestSegment(1600, rightId);

            if(i == 3 || i == 5 || i == 7 ) {
                rightArq->receiveFromLower(leftPacket);
            }

            auto rightPacket = rightArq->requestSegment(1600, leftId);

            leftArq->receiveFromLower(rightPacket);
            auto leftUnicastHeader = (L2HeaderPP*)(leftPacket->getHeaders()[1]);
            auto rightUnicastHeader = (L2HeaderPP*)(rightPacket->getHeaders()[1]);
            cout << "Arq(2) sends " << (int)(leftUnicastHeader->seqno.get()) << endl;
            print_srej_list(PacketUtils::getSrejList(leftUnicastHeader));
        }


        CPPUNIT_ASSERT_EQUAL(lastSeqNoRight,lastSeqNoLeft+1);

        delete leftArq;
        delete rightArq;
    }



CPPUNIT_TEST_SUITE(End2EndTest);
        CPPUNIT_TEST(wrapAroundTest2);
    CPPUNIT_TEST_SUITE_END();
};