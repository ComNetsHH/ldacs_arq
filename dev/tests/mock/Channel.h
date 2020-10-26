//
// Created by Konrad Fuger on 26.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_CHANNEL_H
#define TUHH_INTAIRNET_ARQ_CHANNEL_H

#include "../../L2Segment.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class Channel {
public:
    void send(L2Segment* segment);
};

#endif //TUHH_INTAIRNET_ARQ_CHANNEL_H
