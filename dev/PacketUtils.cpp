//
// Created by Konrad Fuger on 17.02.21.
//

#include "PacketUtils.hpp"

using namespace std;
using namespace TUHH_INTAIRNET_MCSOTDMA;

MacId PacketUtils::getSrcAddress(L2Packet* packet) {
    auto baseHeader = PacketUtils::getBaseHeader(packet);
    return baseHeader->icao_src_id;
}

L2HeaderBase* PacketUtils::getBaseHeader(L2Packet* packet) {
    auto headers = packet->getHeaders();
    auto baseHeader = (L2HeaderBase*)headers[0];
    return baseHeader;
}

vector<PacketFragment> PacketUtils::getUnicastFragments(L2Packet* packet) {
    vector<PacketFragment> fragments;
    auto headers = packet->getHeaders();
    auto payloads = packet->getPayloads();

    for(int i= 1; i< headers.size(); i++) {
        if(headers[i]->frame_type == L2Header::FrameType::unicast) {
            auto fragment = make_pair(headers[i], payloads[i]);
            fragments.push_back(fragment);
        }
    }
    return fragments;
}

vector<SequenceNumber> PacketUtils::getSrejList(L2HeaderUnicast *header) {
    vector<SequenceNumber> srej;
    SequenceNumber anchor = header->getSeqnoNextExpected();
    auto srej_bitmap = header->srej_bitmap;
    for(int i = 0; i< srej_bitmap.size(); i++) {
        if(srej_bitmap[i]) {
            srej.push_back(anchor - (16 - i));
        }
    }


    return srej;
}

void PacketUtils::setSrejList(L2HeaderUnicast *header, vector<SequenceNumber> srej) {
    SequenceNumber anchor = header->getSeqnoNextExpected();
    auto srej_bitmap = header->srej_bitmap;

    for(int i=0; i< srej.size(); i++) {
        auto diff = anchor.get() - srej[i].get();
        srej_bitmap[16-diff] = true;
    }
    header->srej_bitmap = srej_bitmap;
    return;
}
