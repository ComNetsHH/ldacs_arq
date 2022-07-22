//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include "SelectiveRepeatArq.hpp"
#include "PacketUtils.hpp"
#include <InetPacketPayload.hpp>
#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace TUHH_INTAIRNET_MCSOTDMA;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(SelectiveRepeatArq* parent, MacId address, MacId remoteAddress, int maxTx, uint8_t resend_timeout,
                                                     uint8_t window_size) : arq(parent), address(address), remoteAddress(remoteAddress), maxTx(maxTx) {
    this->resend_timeout = resend_timeout;
    this->window_size = window_size;
    //this->seqno_nextToSend = TUHH_INTAIRNET_MCSOTDMA::SequenceNumber(SEQNO_FIRST);
}

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacId address, MacId remoteAddress, int maxTx, uint8_t resend_timeout,
                                                     uint8_t window_size) : address(address), remoteAddress(remoteAddress), maxTx(maxTx) {
    this->resend_timeout = resend_timeout;
    this->window_size = window_size;
    //this->seqno_nextToSend = TUHH_INTAIRNET_MCSOTDMA::SequenceNumber(SEQNO_FIRST);
}

vector<PacketFragment> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<PacketFragment> result;
    for (auto const &segment: list_toPassUp) {
        result.push_back(segment);
        SequenceNumber seqNo = ((L2HeaderPP *) segment.first)->getSeqno();
        emit("arq_seq_no_passed_up", (double)seqNo.get());
    }
    list_toPassUp.clear();
    return result;
}

void SelectiveRepeatArqProcess::processAck(PacketFragment segment) {
    auto header = (L2HeaderPP *) segment.first;
    SequenceNumber nextExpected = SequenceNumber(header->getSeqnoNextExpected());
    vector<SequenceNumber> srej = PacketUtils::getSrejList(header);

    if (seqno_nextExpected.isHigherThan(nextExpected, window_size)) {
        // This ack must be old as it acks an old sequence number.
        //return;
    }

    for(int i = 0; i< srej.size(); i++) {
        emit("arq_srej", (double)srej[i].get());
    }

    auto it = list_sentUnacked.begin();
    auto end = list_sentUnacked.end();

    while (it != end) {
        L2HeaderPP *header = (L2HeaderPP *) it->first;
        auto unackedSeqNo = SequenceNumber(header->getSeqno().get());

        bool isInSrej = false;
        // if in srej, put in rtx list and delete
        for (auto it2 = srej.begin(); it2 != srej.end(); it2++) {
            if (unackedSeqNo == *it2) {
                isInSrej = true;
            }
        }

        if (isInSrej) {
            // PacketFragment copy = PacketUtils::copyFragment(*it, [this](L2Packet::Payload* payload) {return this->deepCopy(payload);});
            list_rtx.push_back(*it);
            list_sentUnacked.erase(it++);
            if(arq) {
                arq->reportRtxData(header->getDestId());
            }
            continue;
        } else if (unackedSeqNo.isLowerThan(nextExpected, window_size)) {
            deletePayload(it->second);
            list_sentUnacked.erase(it++);
            continue;
        }else {
            it++;
        }
    }

}

void SelectiveRepeatArqProcess::processLowerLayerSegment(PacketFragment segment) {
    auto header = (L2HeaderPP *) segment.first;
    SequenceNumber seqNo = SequenceNumber(header->getSeqno());
    processAck(segment);
    emit("arq_seq_no_received", (double)seqNo.get());

    if (seqno_nextExpected.isLowerThan(seqNo, window_size)) {
        seqno_nextExpected = seqNo.next();
    }

    if (seqNo == seqno_lastPassedUp.next()) {
        list_toPassUp.push_back(segment);
        seqno_lastPassedUp.increment();

        auto it = list_rcvdOutOfSeq.begin();
        auto end = list_rcvdOutOfSeq.end();

        while(it != end) {
            L2HeaderPP *header = (L2HeaderPP *) it->first;
            seqNo = SequenceNumber(header->getSeqno());

            emit("arq_out_of_sequence_list", (double)seqNo.get());

            if (seqNo == seqno_lastPassedUp.next()) {
                list_toPassUp.push_back(*it);
                seqno_lastPassedUp.increment();
                list_rcvdOutOfSeq.erase(it++);
            } else {
                it++;
            }
        }
    } else if(seqNo.isLowerThan(seqno_lastPassedUp, window_size)){
        return;
    } else {
        list_rcvdOutOfSeq.push_back(segment);
        list_rcvdOutOfSeq.sort([this](PacketFragment s1, PacketFragment s2) {
            L2HeaderPP *header1 = (L2HeaderPP *) s1.first;
            L2HeaderPP *header2 = (L2HeaderPP *) s2.first;
            auto seqNo1 = header1->getSeqno();
            auto seqNo2 = header2->getSeqno();
            return seqNo1.isLowerThan(seqNo2, window_size);
        });
    }
    state = ArqState::received_last;
}

bool SelectiveRepeatArqProcess::hasRtxSegment(unsigned int size) {
    return !list_rtx.empty();
}

bool SelectiveRepeatArqProcess::wasReceivedOutOfOrder(SequenceNumber seqNo) {
    for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
        auto header = (L2HeaderPP *) (*it).first;
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
        auto header = (L2HeaderPP *) (*it).first;
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
    
    while(packet->getBits() < size && list_rtx.size() > 0) {
        PacketFragment original = list_rtx.front();
        PacketFragment segment = copyFragment(original);
        auto header = (L2HeaderPP*)(segment.first);
        SequenceNumber seqNo = header->getSeqno();
        txCount[(int)(seqNo.get())]++;
        this->list_rtx.pop_front();
        if(!isUnacked(seqNo) && maxTx >= txCount[(int)(seqNo.get())]) {
            this->list_sentUnacked.push_back(segment);
        }

        // TODO: add current srej
        packet->addMessage(original.first, original.second);
        emit("arq_seq_no_sent", (double)((L2HeaderPP*)original.first)->getSeqno().get());
    }
    state = ArqState::sent_last;
    return packet;
}

void SelectiveRepeatArqProcess::processUpperLayerSegment(PacketFragment segment) {
    auto header = (L2HeaderPP *) segment.first;
    header->setSeqno(SequenceNumber(seqno_nextToSend));
    txCount[(int)(seqno_nextToSend.get())] = 1;
    seqno_nextToSend.increment();
    header->setSeqnoNextExpected(SequenceNumber(seqno_nextExpected));
    auto srej_list = getSrejList();
    PacketUtils::setSrejList(header, srej_list);

    // handle max srej amount
    if(state == ArqState::received_last) {
        for(int i = 0; i< srej_list.size(); i++) {
            auto seqNo = srej_list[i];
            int idx = (int)(seqNo.get());
            if(srejCount.find(idx) == srejCount.end()) {
                srejCount[idx] = 0;
            }
            srejCount[idx]++;
            if(srejCount[idx] > maxTx) {
                srejCount[idx] = 0;
                seqno_lastPassedUp.increment();
                auto it = list_rcvdOutOfSeq.begin();
                auto end = list_rcvdOutOfSeq.end();

                while(it != end) {
                    L2HeaderPP *header = (L2HeaderPP *) it->first;
                    seqNo = SequenceNumber(header->getSeqno());

                    emit("arq_out_of_sequence_list", (double)seqNo.get());

                    if (seqNo == seqno_lastPassedUp.next()) {
                        list_toPassUp.push_back(*it);
                        seqno_lastPassedUp.increment();
                        list_rcvdOutOfSeq.erase(it++);
                    } else {
                        it++;
                    }
                }

            }
        }
    }

    PacketFragment copy = copyFragment(segment);
    if(!isUnacked(header->getSeqno())) {
        list_sentUnacked.push_back(copy);
    }
    state = ArqState::sent_last;
}

vector<SequenceNumber> SelectiveRepeatArqProcess::getSrejList() {
    vector<SequenceNumber> list;
    SequenceNumber seqNo(seqno_lastPassedUp.next());

    // unsure if prev works
    while (seqNo.isLowerThan(seqno_nextExpected, window_size)) {
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

bool SelectiveRepeatArqProcess::isUnacked(SequenceNumber seqNo) {
    for (auto it = list_sentUnacked.begin(); it != list_sentUnacked.end(); it++) {
        L2HeaderPP *header = (L2HeaderPP *) it->first;
        auto unackedSeqNo = SequenceNumber(header->getSeqno().get());

        if(seqNo.get() == unackedSeqNo.get()) {
            return true;
        }
    }

    return false;
}

unsigned int SelectiveRepeatArqProcess::getNumUnacked() {
    return this->list_sentUnacked.size();
}

MacId SelectiveRepeatArqProcess::getMacId() {
    return this->remoteAddress;
}

PacketFragment SelectiveRepeatArqProcess::copyFragment(PacketFragment fragment) {
    auto header = fragment.first->copy();
    auto payload = (InetPacketPayload*)fragment.second;
    if(payload != nullptr) {
        payload = (InetPacketPayload*)(deepCopy(fragment.second));
    }
    return make_pair(header, payload);
}