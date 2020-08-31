//
// Created by Sebastian Lindner on 31.08.20.
//

#include <assert.h>
#include "SequenceNumber.hpp"

TUHH_INTAIRNET_ARQ::SequenceNumber::SequenceNumber(uint8_t sequence_number) : sequence_number(sequence_number) {
	assert(SEQNO_MAX >= sequence_number);
}

TUHH_INTAIRNET_ARQ::SequenceNumber::SequenceNumber() : sequence_number(SEQNO_UNSET) {}

TUHH_INTAIRNET_ARQ::SequenceNumber::SequenceNumber(const TUHH_INTAIRNET_ARQ::SequenceNumber& other) : SequenceNumber(other.sequence_number) {}

uint8_t TUHH_INTAIRNET_ARQ::SequenceNumber::get() const {
	return this->sequence_number;
}

void TUHH_INTAIRNET_ARQ::SequenceNumber::increment() {
	this->sequence_number = (uint8_t) ((this->sequence_number + 1) % SEQNO_MAX);
	// Skip over UNSET.
	if (this->get() == SEQNO_UNSET)
		this->increment();
}
