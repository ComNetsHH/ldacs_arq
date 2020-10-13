//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP
#define TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP

#include "L2SegmentHeader.hpp"

namespace TUHH_INTAIRNET_ARQ {
    class L2Segment {
    public:
        class Payload {
            virtual uint64_t getSize() const = 0;
        };

        L2Segment();

        L2Segment(const L2Segment &other);

        L2Segment(L2SegmentHeader header);

        L2Segment copy() const;

        const L2SegmentHeader &getHeader() const;

        Payload *getPayload();

    protected:
        L2SegmentHeader header;
        Payload *payload;
    };
}


#endif //TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP
