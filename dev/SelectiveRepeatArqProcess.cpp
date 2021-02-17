//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include "PacketUtils.hpp"

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacId remoteAddress, uint8_t resend_timeout,
                                                     uint8_t window_size) : remoteAddress(remoteAddress) {
    this->resend_timeout = resend_timeout;
    this->window_size = window_size;
}

vector<PacketFragment> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<PacketFragment> result;
    for (auto const &segment: list_toPassUp) {
        result.push_back(segment);
    }
    list_toPassUp.clear();
    return result;
}

void SelectiveRepeatArqProcess::processAck(PacketFragment segment) {
    L2HeaderUnicast *header;// = (L2HeaderUnicast *) segment->getBaseHeader();
    SequenceNumber nextExpected = header->getSeqnoNextExpected();
    vector<SequenceNumber> srej = header->getSrejList();
    if (seqno_nextExpected.isHigherThan(nextExpected, window_size)) {
        // This ack must be old as it acks an old sequence number.
        return;
    }

    for (auto it = list_sentUnacked.begin(); it != list_sentUnacked.end(); it++) {

        L2HeaderUnicast *header;// = (L2HeaderUnicast *) (*it)->getBaseHeader();
        auto unackedSeqNo = header->getSeqno();
        bool isInSrej = false;
        // if in srej, put in rtx list and delete
        for (auto it2 = srej.begin(); it2 != srej.end(); it2++) {
            if (unackedSeqNo == *it2) {
                isInSrej = true;
            }
        }

        if (isInSrej) {
            list_rtx.push_back(*it);
            list_sentUnacked.erase(it);
            continue;
        } else if (unackedSeqNo.isLowerThan(nextExpected, window_size)) {
            list_sentUnacked.erase(it);
            continue;
        }

    }

    // set next expected to new value
    // delete all segments which are implicitly acked
}

void SelectiveRepeatArqProcess::processLowerLayerSegment(PacketFragment segment) {
    processAck(segment);
    L2HeaderUnicast *header;// = (L2HeaderUnicast *) segment->getBaseHeader();
    SequenceNumber seqNo = header->getSeqno();
    if (seqno_nextExpected.isLowerThan(seqNo, window_size)) {
        seqno_nextExpected = SequenceNumber(seqNo.get() + 1);
    }

    if (seqNo == seqno_lastPassedUp + 1) {
        list_toPassUp.push_back(segment);
        seqno_lastPassedUp.increment();

        for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
            L2HeaderUnicast *header;// = (L2HeaderUnicast *) (*it)->getBaseHeader();
            seqNo = header->getSeqno();
            if (seqNo == seqno_lastPassedUp + 1) {
                list_toPassUp.push_back(*it);
                seqno_lastPassedUp.increment();
                list_rcvdOutOfSeq.erase(it);
            }
        }
    } else {
        list_rcvdOutOfSeq.push_back(segment);
        list_rcvdOutOfSeq.sort([this](PacketFragment s1, PacketFragment s2) {
            L2HeaderUnicast *header1 = (L2HeaderUnicast *) s1.first;
            L2HeaderUnicast *header2 = (L2HeaderUnicast *) s2.first;
            auto seqNo1 = header1->getSeqno();
            auto seqNo2 = header2->getSeqno();
            return seqNo1.isLowerThan(seqNo2, window_size);
        });
    }
}

bool SelectiveRepeatArqProcess::hasRtxSegment(B size) {
    return !list_rtx.empty();
}

bool SelectiveRepeatArqProcess::wasReceivedOutOfOrder(SequenceNumber seqNo) {
    for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
        L2HeaderUnicast *header;// = (L2HeaderUnicast *) (*it)->getBaseHeader();
        SequenceNumber outOfOrderSeqNo = header->getSeqno();
        if (outOfOrderSeqNo == seqNo) {
            return true;
        }
    }
    return false;
}

L2Packet *SelectiveRepeatArqProcess::getRtxSegment(B size) {
    PacketFragment segment = list_rtx.front();
    this->list_rtx.pop_front();
    auto packet = new L2Packet();
    packet->addPayload(segment.first, segment.second);
    return packet;
}

void SelectiveRepeatArqProcess::processUpperLayerSegment(PacketFragment segment) {
    L2HeaderUnicast *header;// = (L2HeaderUnicast *) segment->getBaseHeader();
    header->setSeqno(seqno_nextToSend);
    header->setSeqnoNextExpected(seqno_nextExpected);
    header->setSrejList(getSrejList());
    seqno_nextToSend.increment();
    list_sentUnacked.push_back(segment);
}

vector<SequenceNumber> SelectiveRepeatArqProcess::getSrejList() {
    vector<SequenceNumber> list;
    SequenceNumber seqNo(seqno_lastPassedUp + 1);

    while (seqNo.isLowerThan(seqno_nextExpected - 1, window_size)) {
        if (wasReceivedOutOfOrder(seqNo)) {
            seqNo.increment();
            continue;
        }
        list.push_back(seqNo);
        seqNo.increment();
    }
    return list;
}