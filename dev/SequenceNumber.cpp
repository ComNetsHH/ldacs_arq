//
// Created by Sebastian Lindner on 31.08.20.
//

#include <assert.h>
#include "SequenceNumber.hpp"

using namespace TUHH_INTAIRNET_ARQ;

SequenceNumber::SequenceNumber(uint8_t sequence_number) : sequence_number(sequence_number) {
    assert(SEQNO_MAX >= sequence_number);
}

SequenceNumber::SequenceNumber() : sequence_number(SEQNO_UNSET) {}

SequenceNumber::SequenceNumber(const TUHH_INTAIRNET_ARQ::SequenceNumber &other) : SequenceNumber(
        other.sequence_number) {}

uint8_t SequenceNumber::get() const {
    return this->sequence_number;
}

void SequenceNumber::increment() {
    this->sequence_number = (uint8_t) ((this->sequence_number + 1) % SEQNO_MAX);
    // Skip over UNSET.
    if (this->get() == SEQNO_UNSET)
        this->increment();
}

bool SequenceNumber::operator==(const SequenceNumber &other) const {
    return other.get() == this->get();
}

SequenceNumber SequenceNumber::operator+(uint8_t increment) {
    assert(increment > 0);
    uint8_t raw = this->get();

    if (SEQNO_MAX - raw > increment) {
        return SequenceNumber(raw + increment);
    } else {
        uint8_t wrapAroundDistance = SEQNO_MAX - raw;
        return SequenceNumber(SEQNO_FIRST + (increment - wrapAroundDistance) -1);
    }
}

SequenceNumber SequenceNumber::operator-(uint8_t decrement) {
    assert(decrement > 0);
    uint8_t raw = this->get();

    if (raw > decrement) {
        return SequenceNumber(raw - decrement);
    } else {
        uint8_t wrapAroundDistance = decrement - raw;
        return SequenceNumber(SEQNO_MAX - wrapAroundDistance);
    }
}

bool SequenceNumber::operator>(const SequenceNumber &other) const {
    throw "Deprecated, Sequencenumbers dont have a strict notion of bigger,smaller";
}

bool SequenceNumber::operator<(const SequenceNumber &other) const {
    throw "Deprecated, Sequencenumbers dont have a strict notion of bigger,smaller";
}

bool SequenceNumber::isHigherThan(SequenceNumber other, uint8_t windowSize) {
    uint8_t raw = this->get();
    uint8_t otherRaw = other.get();

    // A sequence number A is higher than an other one  B if
    // I) The raw value of A is higher than the raw value of B but the distance is smaller than the window Size
    if (raw > otherRaw) {
        uint8_t dist = raw - otherRaw;
        if (dist < windowSize) {
            return true;
        }
    }

    // 2) The raw value of A is lower than the raw value of B there was a wrap around
    if (otherRaw > raw) {
        uint8_t dist = (SEQNO_MAX - otherRaw) + (raw - SEQNO_FIRST);

        if (dist < windowSize) {
            return true;
        }
    }
    return false;
}

bool SequenceNumber::isLowerThan(SequenceNumber other, uint8_t windowSize) {
    return other.isHigherThan(*this, windowSize);
}

bool SequenceNumber::isLowerThanEqual(SequenceNumber other, uint8_t windowSize) {
    return isLowerThan(other, windowSize) || *this == other;
}
