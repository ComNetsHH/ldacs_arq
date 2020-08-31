//
// Created by Sebastian Lindner on 31.08.20.
//

#include "L2SegmentHeader.hpp"

TUHH_INTAIRNET_ARQ::L2SegmentHeader::L2SegmentHeader() : seqno(), seqno_next_expected(), srej_list_length(0), poll(0) {}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSeqno(TUHH_INTAIRNET_ARQ::SequenceNumber seqno) {
	this->seqno = seqno;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::SetSeqnoNextExpected(TUHH_INTAIRNET_ARQ::SequenceNumber seqno) {
	this->seqno_next_expected = seqno;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSrejList(std::vector<TUHH_INTAIRNET_ARQ::SequenceNumber> seqnos) {
	this->srej_list = seqnos;
	this->srej_list_length = (uint8_t) seqnos.size();
}

TUHH_INTAIRNET_ARQ::SequenceNumber TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSeqno() const {
	return this->seqno;
}

TUHH_INTAIRNET_ARQ::SequenceNumber TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSeqnoNextExpected() const {
	return this->seqno_next_expected;
}

uint8_t TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSrejListLength() const {
	return this->srej_list_length;
}

std::vector<TUHH_INTAIRNET_ARQ::SequenceNumber> TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSrejList() const {
	return this->srej_list;
}

uint8_t TUHH_INTAIRNET_ARQ::L2SegmentHeader::getPollSlots() const {
	return this->poll;
}









