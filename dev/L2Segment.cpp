//
// Created by Sebastian Lindner on 31.08.20.
//

#include "L2Segment.hpp"

TUHH_INTAIRNET_ARQ::L2Segment::L2Segment() : header(), payload(nullptr) {}

TUHH_INTAIRNET_ARQ::L2Segment::L2Segment(const TUHH_INTAIRNET_ARQ::L2Segment& other) : header(other.header), payload(other.payload) {}

TUHH_INTAIRNET_ARQ::L2Segment TUHH_INTAIRNET_ARQ::L2Segment::copy() const {
	return L2Segment(*this);
}

TUHH_INTAIRNET_ARQ::L2SegmentHeader& TUHH_INTAIRNET_ARQ::L2Segment::getHeader() {
	return this->header;
}

TUHH_INTAIRNET_ARQ::L2Segment::Payload* TUHH_INTAIRNET_ARQ::L2Segment::getPayload() {
	return this->payload;
}
