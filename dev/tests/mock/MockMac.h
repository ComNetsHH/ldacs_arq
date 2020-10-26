//
// Created by Konrad Fuger on 26.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_MOCKMAC_H
#define TUHH_INTAIRNET_ARQ_MOCKMAC_H

#include "Channel.h"
#include "../../L2Segment.hpp"
#include "MockHost.h"

using namespace TUHH_INTAIRNET_ARQ;

class MockHost;

class MockMac {
public:
    void send();

protected:
    MockHost *parent;
    Channel *channel;
};

#endif //TUHH_INTAIRNET_ARQ_MOCKMAC_H
