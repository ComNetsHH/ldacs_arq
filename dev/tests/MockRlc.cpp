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

#include "MockRlc.hpp"
#include <L2Packet.hpp>
#include <L2Header.hpp>
#include <InetPacketPayload.hpp>

MockRlc::MockRlc(MacId id) :macId(id) {}

L2Packet * MockRlc::requestSegment(unsigned int num_bits, const MacId &mac_id) {
    auto packet = new L2Packet();    

    auto unicastHeader = new L2HeaderPP();
    unicastHeader->is_pkt_start = true;
    unicastHeader->is_pkt_end = true;
    unicastHeader->dest_id = mac_id;

    auto payload = new InetPacketPayload();
    payload->size = 500;
    payload->offset = 0;

    packet->addMessage(unicastHeader, payload);

    return packet;
}

void MockRlc::receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority) {};
void MockRlc::receiveFromLower(L2Packet* packet) {}
void MockRlc::receiveInjectionFromLower(L2Packet* packet, PacketPriority priority) {}
bool MockRlc::isThereMoreData(const MacId& mac_id) const {
    return false;
}
