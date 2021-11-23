//
// Created by Konrad Fuger on 24.10.21.
//

#include "MockRlc.hpp"
#include <L2Packet.hpp>
#include <L2Header.hpp>
#include <InetPacketPayload.hpp>

MockRlc::MockRlc(MacId id) :macId(id) {}

L2Packet * MockRlc::requestSegment(unsigned int num_bits, const MacId &mac_id) {
    auto packet = new L2Packet();
    auto baseHeader = new L2HeaderBase(macId, 0,0,0,0);
    packet->addMessage(baseHeader, nullptr);

    auto unicastHeader = new L2HeaderUnicast(L2Header::FrameType::unicast);
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
