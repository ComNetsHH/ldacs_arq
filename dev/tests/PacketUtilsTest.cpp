//
// Created by Konrad Fuger on 22.02.21.
//

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../PacketUtils.hpp"
#include <iostream>
#include <array>


using namespace std;

class PacketUtilsTest : public CppUnit::TestFixture {

    void print_srej_bitmap(array<bool, 16> bitmap) {
        cout << endl << "Bitmap" << endl;
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
    }

    void print_srej_list(vector<SequenceNumber> srej) {
        cout << endl;
        for(int i= 0; i< srej.size(); i++) {
            cout << i << " " << (int)(srej[i].get()) << endl;
        }
    }

public:
    void testSrejConversion() {
        MacId dest_id = MacId(99);
        bool use_arq = true;
        SequenceNumber arq_seqno = SequenceNumber(SEQNO_FIRST);
        SequenceNumber seq_no_next = SequenceNumber(51);
        unsigned int arq_ack_slot = 52;

        vector<SequenceNumber> srej_original;
        L2HeaderUnicast *header = new L2HeaderUnicast(dest_id, use_arq, arq_seqno, seq_no_next, arq_ack_slot);

        srej_original.push_back(SequenceNumber(50));

        PacketUtils::setSrejList(header, srej_original);

        vector<SequenceNumber> srej = PacketUtils::getSrejList(header);

        print_srej_list(srej);

        print_srej_bitmap(header->srej_bitmap);

        CPPUNIT_ASSERT(true);
    }



CPPUNIT_TEST_SUITE(PacketUtilsTest);
        CPPUNIT_TEST(testSrejConversion);
    CPPUNIT_TEST_SUITE_END();
};