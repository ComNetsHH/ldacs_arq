//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include "PacketUtils.hpp"
#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacId remoteAddress, uint8_t resend_timeout,
                                                     uint8_t window_size) : remoteAddress(remoteAddress) {
    this->resend_timeout = resend_timeout;
    this->window_size = window_size;
    //this->seqno_nextToSend = TUHH_INTAIRNET_MCSOTDMA::SequenceNumber(SEQNO_FIRST);
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
    //cout << "### MacId: " << (int) remoteAddress.getId() << " received ACK ###" << endl;
    //cout << "Unacked: " << list_sentUnacked.size() << endl;
    //cout << "Rtx: " << list_rtx.size() << endl;
    //cout << "OOO: " << list_rcvdOutOfSeq.size() << endl;
    //cout << "PO: " << list_toPassUp.size() << endl;
    //cout << "Srej: " << srej.size() << endl;

    auto header = (L2HeaderUnicast *) segment.first;
    SequenceNumber nextExpected = SequenceNumber(header->getSeqnoNextExpected());
    vector<SequenceNumber> srej = PacketUtils::getSrejList(header);
    if (seqno_nextExpected.isHigherThan(nextExpected, window_size)) {
        // This ack must be old as it acks an old sequence number.
        // return;
    }

    for(int i = 0; i< srej.size(); i++) {
        emit("arq_srej", (double)srej[i].get());
    }

    for (auto it = list_sentUnacked.begin(); it != list_sentUnacked.end(); it++) {
        L2HeaderUnicast *header = (L2HeaderUnicast *) it->first;
        auto unackedSeqNo = SequenceNumber(header->getSeqno().get());
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
    auto header = (L2HeaderUnicast *) segment.first;
    SequenceNumber seqNo = SequenceNumber(header->getSeqno());
    emit("arq_seq_no_received", (double)seqNo.get());

    // cout << (int)remoteAddress.getId() << " Received SeqNo " << (int)seqNo.get() << endl;

    if (seqno_nextExpected.isLowerThan(seqNo, window_size)) {
        seqno_nextExpected = SequenceNumber(seqNo.get() + 1);
    }

    if (seqNo == seqno_lastPassedUp + 1) {
        list_toPassUp.push_back(segment);
        seqno_lastPassedUp.increment();

        // cout << "LPU " << (int) seqno_lastPassedUp.get() << endl;


        for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
            L2HeaderUnicast *header = (L2HeaderUnicast *) it->first;
            seqNo = SequenceNumber(header->getSeqno());
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

bool SelectiveRepeatArqProcess::hasRtxSegment(unsigned int size) {
    return !list_rtx.empty();
}

bool SelectiveRepeatArqProcess::wasReceivedOutOfOrder(SequenceNumber seqNo) {
    for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
        auto header = (L2HeaderUnicast *) (*it).first;
        SequenceNumber outOfOrderSeqNo = SequenceNumber(header->getSeqno());
        if (outOfOrderSeqNo == seqNo) {
            return true;
        }
    }
    return false;
}

unsigned int SelectiveRepeatArqProcess::getRtxSize() {
    unsigned int result = 0;
    for (auto it = list_rtx.begin(); it != list_rtx.end(); it++) {
        auto header = (L2HeaderUnicast *) (*it).first;
        auto payload = (*it).second;
        result += header->getBits();
        if(payload != nullptr) {
            result += payload->getBits();
        }
    }
    return result;
}

L2Packet *SelectiveRepeatArqProcess::getRtxSegment(unsigned int size) {
    auto packet = new L2Packet();
    L2HeaderBase* base_header = new L2HeaderBase(remoteAddress, 0, 0, 0, 0);
    packet->addMessage(base_header, nullptr);

    while(packet->getBits() < size && list_rtx.size() > 0) {
        PacketFragment original = list_rtx.front();
        PacketFragment segment = PacketUtils::copyFragment(original);
        this->list_rtx.pop_front();
        this->list_sentUnacked.push_back(original);
        packet->addMessage(segment.first, segment.second);
        emit("arq_seq_no_sent", (double)((L2HeaderUnicast*)segment.first)->getSeqno().get());
    }
    return packet;
}

void SelectiveRepeatArqProcess::processUpperLayerSegment(PacketFragment segment) {
    auto header = (L2HeaderUnicast *) segment.first;
    header->setSeqno(SequenceNumber(seqno_next_to_send++));
    header->setSeqnoNextExpected(SequenceNumber(seqno_nextExpected));
    PacketUtils::setSrejList(header, getSrejList());
    seqno_nextToSend.increment();
    list_sentUnacked.push_back(PacketUtils::copyFragment(segment));

}

vector<SequenceNumber> SelectiveRepeatArqProcess::getSrejList() {
    vector<SequenceNumber> list;
    SequenceNumber seqNo(seqno_lastPassedUp + 1);

    while (seqNo.isLowerThan(seqno_nextExpected - 1, window_size)) {
        if (wasReceivedOutOfOrder(seqNo)) {
            seqNo.increment();
            continue;
        }
        list.push_back(SequenceNumber(seqNo));
        seqNo.increment();
    }
    return list;
}

unsigned int SelectiveRepeatArqProcess::getNumReceivedOutOfSequence() {
    return this->list_rcvdOutOfSeq.size();
}

unsigned int SelectiveRepeatArqProcess::getNumRtx() {
    return this->list_rtx.size();
}

unsigned int SelectiveRepeatArqProcess::getNumUnacked() {
    return this->list_sentUnacked.size();
}

MacId SelectiveRepeatArqProcess::getMacId() {
    return this->remoteAddress;
}