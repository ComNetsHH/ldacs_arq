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

#include "PacketUtils.hpp"
#include "InetPacketPayload.hpp"

using namespace std;
using namespace TUHH_INTAIRNET_MCSOTDMA;

MacId PacketUtils::getSrcAddress(L2Packet* packet) {
    const L2Header* header = packet->getHeaders().at(0);
    if (header->frame_type == L2Header::unicast)
        return ((L2HeaderPP*) header)->src_id;
    else
        return ((L2HeaderSH*) header)->src_id;    
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

vector<SequenceNumber> PacketUtils::getSrejList(L2HeaderPP *header) {
    vector<SequenceNumber> srej;
    SequenceNumber anchor = header->getSeqnoNextExpected();
    auto srej_bitmap = header->srej_bitmap;
    for(int i = 0; i< srej_bitmap.size(); i++) {
        if(srej_bitmap[i]) {
            SequenceNumber x = anchor - (16 - i);
            srej.push_back(x);
        }
    }

    return srej;
}

int PacketUtils::diff(SequenceNumber one, SequenceNumber other, uint8_t windowSize ) {
    SequenceNumber higher;
    SequenceNumber lower;
    int sign = 0;

    if(one.isHigherThan(other, windowSize)) {
        higher = one;
        lower = other;
        sign = 1;
    } else {
        higher = other;
        lower = one;
        sign = -1;
    }

    int result = 0;
    auto counter = SequenceNumber(lower.get());

    while(higher.get() != counter.get()) {
        counter.increment();
        result++;
    }

    return sign * result;
}

void PacketUtils::setSrejList(L2HeaderPP *header, vector<SequenceNumber> srej) {
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
        if(diff >= 1 && diff <= 16) {
            //cout << diff << endl;
            srej_bitmap[16-diff] = true;
        }

    }



    header->setSeqnoNextExpected(SequenceNumber(anchor));
    header->setSrejBitmap(srej_bitmap);

    return;
}

/**
PacketFragment PacketUtils::copyFragment(PacketFragment fragment, std::function<L2Packet::Payload*(L2Packet::Payload*)> copyFkt) {
    auto header = fragment.first->copy();
    auto payload = (InetPacketPayload*)fragment.second;
    if(payload != nullptr) {
        payload = (InetPacketPayload*)copyFkt(fragment.second);
    }
    return make_pair(header, payload);
}

 **/
