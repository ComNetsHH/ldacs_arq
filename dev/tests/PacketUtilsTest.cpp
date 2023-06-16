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
#include "../PacketUtils.hpp"
#include <iostream>
#include <array>


using namespace std;

class PacketUtilsTest : public CppUnit::TestFixture {

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
    void testSrejConversion() {
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber seq_no_next = SequenceNumber(51);
        unsigned int arq_ack_slot = 52;

        vector<SequenceNumber> srej_original;
        L2HeaderPP *header = new L2HeaderPP(dest_id, use_arq, arq_seqno, seq_no_next, arq_ack_slot);

        srej_original.push_back(SequenceNumber(50));

        PacketUtils::setSrejList(header, srej_original);

        vector<SequenceNumber> srej = PacketUtils::getSrejList(header);

        //print_srej_list(srej);

        //print_srej_bitmap(header->srej_bitmap);
        CPPUNIT_ASSERT(!header->srej_bitmap[13]);
        CPPUNIT_ASSERT(!header->srej_bitmap[14]);
        CPPUNIT_ASSERT(header->srej_bitmap[15]);
    }

    void testSrejBitmapDuringWrapAround() {
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber seq_no_next = SequenceNumber(50);
        unsigned int arq_ack_slot = 52;
        vector<SequenceNumber> srej;

        L2HeaderPP *header = new L2HeaderPP(dest_id, use_arq, arq_seqno, seq_no_next, arq_ack_slot);

        srej.push_back(SequenceNumber(254));
        srej.push_back(SequenceNumber(253));
        srej.push_back(SequenceNumber(252));

        PacketUtils::setSrejList(header, srej);
        vector<SequenceNumber> srej_new = PacketUtils::getSrejList(header);

        CPPUNIT_ASSERT_EQUAL(3, (int)srej_new.size());
    }

    void testTooManySrej() {
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber seq_no_next = SequenceNumber(25);
        unsigned int arq_ack_slot = 52;
        vector<SequenceNumber> srej;

        L2HeaderPP *header = new L2HeaderPP(dest_id, use_arq, arq_seqno, seq_no_next, arq_ack_slot);

        srej.push_back(SequenceNumber(2));
        srej.push_back(SequenceNumber(6));
        srej.push_back(SequenceNumber(7));
        srej.push_back(SequenceNumber(8));
        srej.push_back(SequenceNumber(17));
        srej.push_back(SequenceNumber(18));
        srej.push_back(SequenceNumber(20));
        srej.push_back(SequenceNumber(22));
        srej.push_back(SequenceNumber(23));

        PacketUtils::setSrejList(header, srej);
        vector<SequenceNumber> srej_new = PacketUtils::getSrejList(header);

        //print_srej_bitmap(header->srej_bitmap);

        CPPUNIT_ASSERT_EQUAL(5, (int)srej_new.size());
    }


    void testWrapAround() {
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber seq_no_next = SequenceNumber(1);
        unsigned int arq_ack_slot = 52;
        vector<SequenceNumber> srej;

        L2HeaderPP *header = new L2HeaderPP(dest_id, use_arq, arq_seqno, seq_no_next, arq_ack_slot);

        srej.push_back(SequenceNumber(254));

        PacketUtils::setSrejList(header, srej);
        vector<SequenceNumber> srej_new = PacketUtils::getSrejList(header);

        //print_srej_bitmap(header->srej_bitmap);
        //print_srej_list(srej_new);

        CPPUNIT_ASSERT_EQUAL((int)(srej_new[0].get()), 254);
    }



CPPUNIT_TEST_SUITE(PacketUtilsTest);
        CPPUNIT_TEST(testSrejConversion);
        CPPUNIT_TEST(testSrejBitmapDuringWrapAround);
        CPPUNIT_TEST(testTooManySrej);
        CPPUNIT_TEST(testWrapAround);
    CPPUNIT_TEST_SUITE_END();
};