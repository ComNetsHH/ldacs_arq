//
// Created by Konrad Fuger on 24.10.21.
//

#ifndef TUHH_INTAIRNET_ARQ_MOCKRLC_HPP
#define TUHH_INTAIRNET_ARQ_MOCKRLC_HPP

#include <IRlc.hpp>

class MockRlc: IRlc {
protected:
    MacId macId;
public:
    MockRlc(MacId id);
    L2Packet * requestSegment(unsigned int num_bits, const MacId &mac_id) override;
    void receiveFromUpper(L3Packet* data, MacId dest, PacketPriority priority = PRIORITY_DEFAULT) override;
    void receiveFromLower(L2Packet* packet) override;
    void receiveInjectionFromLower(L2Packet* packet, PacketPriority priority = PRIORITY_LINK_MANAGEMENT) override;
    bool isThereMoreData(const MacId& mac_id) const override;
    unsigned int getQueuedDataSize(MacId dest) override {return 0;};
};


#endif //TUHH_INTAIRNET_ARQ_MOCKRLC_HPP
