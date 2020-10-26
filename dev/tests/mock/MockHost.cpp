//
// Created by Konrad Fuger on 26.10.20.
//

#include "MockHost.h"

MockMac *MockHost::getMac() {
    return this->mac;
}

MockRlc MockHost::getRlc() {
    return this->rlc;
}

MacAddress MockHost::getAddress() {
    return this->address;
}

MockHost::MockHost(MacAddress address, Channel *channel) :
        arq(100, 100) {
    this->address = address;
    this->channel = channel;
//    this->mac = MockMac();
//    this->rlc = MockRlc();
}

void MockHost::send() {
    mac->send();
}

