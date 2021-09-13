//
// Created by Konrad Fuger on 17.02.21.
//

#include "PacketUtils.hpp"
#include "InetPacketPayload.hpp"

using namespace std;
using namespace TUHH_INTAIRNET_MCSOTDMA;

MacId PacketUtils::getSrcAddress(L2Packet* packet) {
    auto baseHeader = PacketUtils::getBaseHeader(packet);
    return baseHeader->src_id;
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
            srej.push_back(SequenceNumber(anchor - (16 - i)));
        }
    }

    return srej;
}

int PacketUtils::diff(SequenceNumber higher, SequenceNumber lower) {
    int result = 0;
    auto counter = SequenceNumber(lower.get());

    while(higher.get() != counter.get()) {
        counter.increment();
        result ++;
    }

    return result;
}

void PacketUtils::setSrejList(L2HeaderUnicast *header, vector<SequenceNumber> srej) {
    SequenceNumber anchor = SequenceNumber(header->getSeqnoNextExpected());
    auto srej_bitmap = header->srej_bitmap;

    for(int i=0; i< srej.size(); i++) {
        int diff = PacketUtils::diff(anchor, srej[i]);

        if(diff > 16) {
            anchor = SequenceNumber(srej[i]) + 16;
        }
    }

    for(int i=0; i< srej.size(); i++) {
        int diff = PacketUtils::diff(anchor, srej[i]);

        // Ignore all seqnos out of range
        if(diff >= 0) {
            srej_bitmap[16-diff] = true;
        }

    }

    header->setSeqnoNextExpected(SequenceNumber(anchor));
    header->setSrejBitmap(srej_bitmap);

    return;
}

PacketFragment PacketUtils::copyFragment(PacketFragment fragment) {
    auto header = fragment.first->copy();
    auto payload = (InetPacketPayload*)fragment.second;
    if(payload != nullptr) {
        payload = (InetPacketPayload*)fragment.second->copy();
        if(payload->size > 0) {
            payload->original = nullptr;
        }
    }
    return make_pair(header, payload);
}
