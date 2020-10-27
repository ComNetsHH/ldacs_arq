//
// Created by Konrad Fuger on 26.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_IRLC_H
#define TUHH_INTAIRNET_ARQ_IRLC_H

#include "L2Segment.hpp"
#include "TYPES.h"

using namespace TUHH_INTAIRNET_ARQ;

class IRlc  {
public:
    virtual L2Segment getSegment(B size, MacAddress address ) = 0;
};

#endif //TUHH_INTAIRNET_ARQ_IRLC_H
