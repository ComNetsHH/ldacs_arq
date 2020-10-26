//
// Created by Konrad Fuger on 26.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_MOCKHOST_H
#define TUHH_INTAIRNET_ARQ_MOCKHOST_H

#include "MockMac.h"
#include "MockRlc.h"
#include "../../TYPES.h"
#include "../../SelectiveRepeatArq.hpp"

using namespace TUHH_INTAIRNET_ARQ;

class MockMac;

class MockHost {
protected:
    MockMac *mac;
    MockRlc rlc;
    SelectiveRepeatArq arq;
    Channel* channel;
    MacAddress address;

public:
    MockHost(MacAddress address, Channel* channel);
    MockMac* getMac();
    MockRlc getRlc();
    SelectiveRepeatArq getArq();
    MacAddress getAddress();
    Channel* getChannel();
    void send();
};

#endif //TUHH_INTAIRNET_ARQ_MOCKHOST_H
