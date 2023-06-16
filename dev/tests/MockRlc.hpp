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
