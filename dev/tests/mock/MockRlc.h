//
// Created by Konrad Fuger on 26.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_MOCKRLC_H
#define TUHH_INTAIRNET_ARQ_MOCKRLC_H

#include "../../IRlc.h"


class MockRlc: IRlc {
public:
    L2Segment getSegment(B size, MacAddress address) override;
};

#endif //TUHH_INTAIRNET_ARQ_MOCKRLC_H
