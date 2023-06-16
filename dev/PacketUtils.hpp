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

#ifndef TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP
#define TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP

#include "L2Packet.hpp"
#include "MacId.hpp"
#include "PacketFragment.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;


class PacketUtils {
public:
    static MacId getSrcAddress(L2Packet* packet);    

    static vector<PacketFragment> getUnicastFragments(L2Packet* packet);

    static vector<SequenceNumber> getSrejList(L2HeaderPP * header);

    static void setSrejList(L2HeaderPP *header, vector<SequenceNumber> srej);

    static int diff(SequenceNumber one, SequenceNumber other, uint8_t windowSize = 100);

    //static PacketFragment copyFragment(PacketFragment fragment, function<L2Packet::Payload *(L2Packet::Payload *)> copyFkt);
};


#endif //TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP
