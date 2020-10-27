//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacAddress remoteAddress) {
    this->remoteAddress = remoteAddress;
}

vector<L2Segment*> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<L2Segment*> result;
    for (auto const& segment: list_toPassUp) {
        result.push_back(segment);
    }
    list_toPassUp.clear();
    return result;
}

void SelectiveRepeatArqProcess::processAck(L2Segment *segment) {
    L2SegmentHeader *header = segment->getHeader();
    SequenceNumber nextExpected = header->getSeqnoNextExpected();
    if(seqno_nextExpected.get() >= nextExpected.get()) {
        // This ack must be old as it acks an old sequence number.
        return;
    }
    // set next expected to new value
    // delete all segments which are implizitly acked
}

void SelectiveRepeatArqProcess::processLowerLayerSegment(L2Segment *segment) {
    processAck(segment);
    SequenceNumber seqNo = segment->getHeader()->getSeqno();
    if(seqno_nextExpected.get() <= seqNo.get()) {
        seqno_nextExpected = SequenceNumber(seqNo.get() +1);
    }
    list_toAck.push_back(segment);
    received_segments.push_back(seqNo);

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
        list_rcvdOutOfSeq.sort([](L2Segment *s1, L2Segment *s2) {
            auto h1 = s1->getHeader()->getSeqno();
            auto h2 = s2->getHeader()->getSeqno();
            return h1 < h2;
        });
    }
}

bool SelectiveRepeatArqProcess::hasRtxSegment(B size) {
    return !list_rtx.empty();
}

L2Segment* SelectiveRepeatArqProcess::getRtxSegment(B size) {
    L2Segment *segment = list_rtx.front();
    this->list_rtx.pop_front();
    return segment;
}

void SelectiveRepeatArqProcess::processUpperLayerSegment(L2Segment *segment) {
    L2SegmentHeader* header = segment->getHeader();
    header->setSeqno(seqno_nextToSend);
    seqno_nextToSend.increment();
    list_sentUnacked.push_back(segment);
    //segment->getHeader()->setSeqno(SequenceNumber(2));
}