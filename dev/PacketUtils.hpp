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

    static PacketFragment copyFragment(PacketFragment fragment);

    static vector<SequenceNumber> getSrejList(L2HeaderUnicast * header);

    static void setSrejList(L2HeaderUnicast *header, vector<SequenceNumber> srej);

    static int diff(SequenceNumber onbe, SequenceNumber other);
};


#endif //TUHH_INTAIRNET_ARQ_PACKETUTILS_HPP
