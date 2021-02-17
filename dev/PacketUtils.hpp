//
// Created by Konrad Fuger on 17.02.21.
//

#ifndef TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP
#define TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP

#include "L2Packet.hpp"
#include "MacId.hpp"
#include "PacketFragment.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;


class PacketUtils {
public:
    static MacId getSrcAddress(L2Packet* packet);

    static L2HeaderBase* getBaseHeader(L2Packet* packet);

    static vector<PacketFragment> getUnicastFragments(L2Packet* packet);
};


#endif //TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP