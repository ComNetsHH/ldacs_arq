//
// Created by Sebastian Lindner on 31.08.20.
//

#include "L2SegmentHeader.hpp"

using namespace TUHH_INTAIRNET_MCSOTDMA;

TUHH_INTAIRNET_ARQ::L2SegmentHeader::L2SegmentHeader() {

}

TUHH_INTAIRNET_ARQ::L2SegmentHeader::L2SegmentHeader(SequenceNumber seqno) {
    this->seqno = seqno;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSeqno(SequenceNumber seqno) {
	this->seqno = seqno;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSeqnoNextExpected(SequenceNumber seqno) {
	this->seqno_next_expected = seqno;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSrejList(std::vector<SequenceNumber> seqnos) {
	this->srej_list = seqnos;
	this->srej_list_length = (uint8_t) seqnos.size();
}

SequenceNumber TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSeqno() const {
	return this->seqno;
}

SequenceNumber TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSeqnoNextExpected() const {
	return this->seqno_next_expected;
}

uint8_t TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSrejListLength() const {
	return this->srej_list_length;
}

std::vector<SequenceNumber> TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSrejList() const {
	return this->srej_list;
}

uint8_t TUHH_INTAIRNET_ARQ::L2SegmentHeader::getPollSlots() const {
	return this->poll;
}

uint64_t TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSize() const {
	// 1B * (seqno + seqno_next_expected + poll + srej_list)
	return 3 + this->srej_list_length;
}

MacAddress TUHH_INTAIRNET_ARQ::L2SegmentHeader::getSrcAddress() {
    return this->srcAddress;
}

MacAddress TUHH_INTAIRNET_ARQ::L2SegmentHeader::getDestAddress() {
    return this->destAddress;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setSrcAddress(MacAddress address) {
    this->srcAddress = address;
}

void TUHH_INTAIRNET_ARQ::L2SegmentHeader::setDestAddress(MacAddress address) {
    this->destAddress = address;
}










