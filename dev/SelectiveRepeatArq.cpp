//
// Created by Sebastian Lindner on 31.08.20.
//

#include "SelectiveRepeatArq.hpp"
#include <cstdint>
#include <stdexcept>

TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::SelectiveRepeatArq(uint8_t resend_timeout, uint8_t window_size)
	: resend_timeout(resend_timeout), window_size(window_size), list_sentUnacked(), list_rtx(), list_toAck(), sending_buffer(), list_rcvdOutOfSeq(), received_segments(), received_acks(), sent_segments(), sent_and_acked_segments(), sent_acks() {
	
}

bool TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::isInRtxState() const {
	return !this->list_rtx.empty();
}

void TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::receiveFromUpperLayer(const TUHH_INTAIRNET_ARQ::L2Segment& segment) {
	throw std::logic_error("not implemented");
}

void TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::receiveFromLowerLayer(const TUHH_INTAIRNET_ARQ::L2Segment& segment) {
	throw std::logic_error("not implemented");
}

void TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::processData(const TUHH_INTAIRNET_ARQ::L2Segment& segment) {
	throw std::logic_error("not implemented");
}

void TUHH_INTAIRNET_ARQ::SelectiveRepeatArq::processAck(const TUHH_INTAIRNET_ARQ::L2Segment& segment) {
	throw std::logic_error("not implemented");
}
