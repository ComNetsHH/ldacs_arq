//
// Created by Konrad Fuger on 17.02.21.
//

#ifndef TUHH_INTAIRNET_ARQ_PACKETFRAGMENT_HPP
#define TUHH_INTAIRNET_ARQ_PACKETFRAGMENT_HPP

#include "L2Packet.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;
using namespace std;

typedef pair<L2Header*, L2Packet::Payload *> PacketFragment;

#endif //TUHH_INTAIRNET_ARQ_PACKETFRAGMENT_HPP
