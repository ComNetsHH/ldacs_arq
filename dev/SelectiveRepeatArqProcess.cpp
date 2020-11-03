//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacAddress remoteAddress, uint8_t resend_timeout,
                                                     uint8_t window_size) {
    this->remoteAddress = remoteAddress;
    this->resend_timeout = resend_timeout;
    this->window_size = window_size;
}

vector<L2Segment *> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<L2Segment *> result;
    for (auto const &segment: list_toPassUp) {
        result.push_back(segment);
    }
    list_toPassUp.clear();
    return result;
}

void SelectiveRepeatArqProcess::processAck(L2Segment *segment) {
    L2SegmentHeader *header = segment->getHeader();
    SequenceNumber nextExpected = header->getSeqnoNextExpected();
    if (seqno_nextExpected.get() >= nextExpected.get()) {
        // This ack must be old as it acks an old sequence number.
        return;
    }
    // set next expected to new value
    // delete all segments which are implicitly acked
}

void SelectiveRepeatArqProcess::processLowerLayerSegment(L2Segment *segment) {
    processAck(segment);
    SequenceNumber seqNo = segment->getHeader()->getSeqno();
    if (seqno_nextExpected.get() <= seqNo.get()) {
        seqno_nextExpected = SequenceNumber(seqNo.get() + 1);
    }
    list_toAck.push_back(segment);
    //received_segments.push_back(seqNo);

    if (seqNo.get() == seqno_lastPassedUp.get() + 1) {
        list_toPassUp.push_back(segment);
        seqno_lastPassedUp.increment();

        for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
            seqNo = (*it)->getHeader()->getSeqno();
            if (seqNo.get() == seqno_lastPassedUp.get() + 1) {
                list_toPassUp.push_back(*it);
                seqno_lastPassedUp.increment();
                list_rcvdOutOfSeq.erase(it);
            }
        }
    } else {
        list_rcvdOutOfSeq.push_back(segment);
        list_rcvdOutOfSeq.sort([this](L2Segment *s1, L2Segment *s2) {
            auto h1 = s1->getHeader()->getSeqno();
            auto h2 = s2->getHeader()->getSeqno();
            return h1.isLowerThan(h2, window_size);
        });
    }
}

bool SelectiveRepeatArqProcess::hasRtxSegment(B size) {
    return !list_rtx.empty();
}

bool SelectiveRepeatArqProcess::wasReceivedOutOfOrder(SequenceNumber seqNo) {
    for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
        SequenceNumber outOfOrderSeqNo = (*it)->getHeader()->getSeqno();
        if (outOfOrderSeqNo == seqNo) {
            return true;
        }
    }
    return false;
}

L2Segment *SelectiveRepeatArqProcess::getRtxSegment(B size) {
    L2Segment *segment = list_rtx.front();
    this->list_rtx.pop_front();
    return segment;
}

void SelectiveRepeatArqProcess::processUpperLayerSegment(L2Segment *segment) {
    L2SegmentHeader *header = segment->getHeader();
    header->setSeqno(seqno_nextToSend);
    header->setSeqnoNextExpected(seqno_nextExpected);
    header->setSrejList(getSrejList());
    seqno_nextToSend.increment();
    list_sentUnacked.push_back(segment);
}

vector<SequenceNumber> SelectiveRepeatArqProcess::getSrejList() {
    vector<SequenceNumber> list;

    //SequenceNumber seqNo(seqno_nextExpected.get() - window_size);
    SequenceNumber seqNo(1);

    while (seqNo.get() < seqno_nextExpected.get() - 1) {
        if (seqNo.get() <= seqno_lastPassedUp.get()) {
            seqNo.increment();
            continue;
        }
        if (wasReceivedOutOfOrder(seqNo)) {
            seqNo.increment();
            continue;
        }
        list.push_back(seqNo);
        seqNo.increment();
    }

    // start from nextExpected - window
    // loop through all seqNo
    // if not received, add to list
    return list;
}