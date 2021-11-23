//
// Created by Sebastian Lindner on 31.08.20.
//

#include "SelectiveRepeatArq.hpp"
#include "PacketUtils.hpp"
#include "IRlc.hpp"
#include "IMac.hpp"
#include <cstdint>
#include <stdexcept>

using namespace TUHH_INTAIRNET_ARQ;

SelectiveRepeatArq::SelectiveRepeatArq(MacId address, uint8_t resend_timeout, uint8_t window_size, int maxTx, double per)
        : address(address), resend_timeout(resend_timeout), window_size(window_size), maxTx(maxTx), per(per) {
}

bool SelectiveRepeatArq::isInRtxState() const {
    return false;
}

unsigned int SelectiveRepeatArq::getBufferStatus() {
    throw std::logic_error("not implemented");
}

SelectiveRepeatArqProcess *SelectiveRepeatArq::getArqProcess(MacId remoteAddress) {
    auto it = arqProcesses.find(address);
    if (it != arqProcesses.end()) {
        return it->second;
    }
    auto process = new SelectiveRepeatArqProcess(this, address, remoteAddress, maxTx);

    if(emitCallback) {
        process->registerEmitEventCallback(emitCallback);
    }

    if(copyL2PayloadCallback) {
        process->registerCopyL2PayloadCallback(copyL2PayloadCallback);
    }

    if(deleteL2Callback) {
        process->registerDeleteL2Callback(deleteL2Callback);
    }

    if(deleteL2PayloadCallback) {
        process->registerDeleteL2PayloadCallback(deleteL2PayloadCallback);
    }
    arqProcesses.insert(make_pair(address, process));
    return process;
}

vector<PacketFragment> SelectiveRepeatArq::getInOrderSegments() {
    vector<PacketFragment> segments;
    for (auto it = arqProcesses.begin(); it != arqProcesses.end(); it++) {
        auto process = it->second;
        auto processSegments = process->getInOrderSegments();
        for (auto const &inOrderSegment: processSegments) {
            segments.push_back(inOrderSegment);
        }
    }

    return segments;
}

bool SelectiveRepeatArq::hasRtxSegment(MacId address, unsigned int size) {
    auto process = getArqProcess(address);
    if (!process) {
        return false;
    }
    return process->hasRtxSegment(size);
}

L2Packet * SelectiveRepeatArq::getRtxSegment(MacId address, unsigned int size) {
    auto process = getArqProcess(address);
    if (!process) {
        return nullptr;
    }
    return process->getRtxSegment(size);
}

int SelectiveRepeatArq::getNumProcesses() {
    return arqProcesses.size();
}

void SelectiveRepeatArq::notifyAboutNewLink(const MacId& id) {
    auto process = getArqProcess(id);
    if(process != nullptr) {
        return;
        throw std::runtime_error("Link must be removed first");
    }
    process = new SelectiveRepeatArqProcess(this, address, id, maxTx);
    if(copyL2PayloadCallback) {
        process->registerCopyL2PayloadCallback(copyL2PayloadCallback);
    }
    arqProcesses.insert(make_pair(id, process));
}

void SelectiveRepeatArq::notifyAboutRemovedLink(const MacId& id) {
    auto it = arqProcesses.find(id);
    if (it != arqProcesses.end()) {
        return;
    }
    arqProcesses.erase(it);
    delete it->second;
}

void SelectiveRepeatArq::notifyOutgoing(unsigned int num_bits, const MacId& mac_id) {
    debug("SelectiveRepeatArq::notifyOutgoing " + std::to_string(num_bits));
    auto process = this->getArqProcess(mac_id);

    IMac* mac = getLowerLayer();
    mac->notifyOutgoing(num_bits + process->getRtxSize(), mac_id);
}

void SelectiveRepeatArq::reportRtxData(MacId dest) {
    IRlc* rlc = getUpperLayer();
    IMac* mac = getLowerLayer();

    if(!rlc || !mac) {
        return;
    }


    unsigned int data = rlc->getQueuedDataSize(dest);
    auto process = this->getArqProcess(dest);
    data += process->getRtxSize();
    mac->notifyOutgoing(data, dest);
}

L2Packet* SelectiveRepeatArq::requestSegment(unsigned int num_bits, const MacId& mac_id) {
    if(mac_id != SYMBOLIC_LINK_ID_BROADCAST) {
        emit("arq_bits_requested_from_lower", (double)num_bits);
    }
    debug("SelectiveRepeatArq::requestSegment " + std::to_string(num_bits));
    if(hasRtxSegment(mac_id, num_bits)) {
        auto rtxPacket =  getRtxSegment(mac_id, num_bits);
        emit("arq_bits_sent_down", (double)rtxPacket->getBits());
        emit("arq_num_rtx", (double)(++this->numRtx));

        return rtxPacket;
    }
    IRlc* rlc = getUpperLayer();
    L2Packet* segment = rlc->requestSegment(num_bits, mac_id);
    auto fragments = PacketUtils::getUnicastFragments(segment);
    auto process = getArqProcess(mac_id);

    for(int i=0; i< fragments.size(); i++) {
        process->processUpperLayerSegment(fragments[i]);
        auto header = (L2HeaderUnicast*)fragments[i].first;
        emit("arq_seq_no_sent", (double)header->getSeqno().get());
    }
    emit("arq_bits_sent_down", (double)segment->getBits());
    emitStatistics();

    return segment;
}

bool SelectiveRepeatArq::shouldLinkBeArqProtected(const MacId& mac_id) const {
    return false;
}

void SelectiveRepeatArq::injectIntoUpper(L2Packet* packet) {
    debug("SelectiveRepeatArq::injectIntoUpper");
    IRlc* rlc = getUpperLayer();
    return rlc->receiveInjectionFromLower(packet);
}

void SelectiveRepeatArq::receiveFromLower(L2Packet* packet) {
    float unif = (float) rand()/RAND_MAX;

    if(unif < per) {
        return;
    }

    debug("SelectiveRepeatArq::receiveFromLower");
    MacId src = packet->getOrigin();
    auto process = getArqProcess(src);
    auto fragments = PacketUtils::getUnicastFragments(packet);

    for(int i = 0; i< fragments.size(); i++) {
        process->processLowerLayerSegment(fragments[i]);
    }

    auto inOrderFragments = process->getInOrderSegments();
    if(inOrderFragments.size() > 0) {
        auto completePacket = new L2Packet();
        L2HeaderBase* base_header = new L2HeaderBase(process->getMacId(), 0, 0, 0, 0);
        completePacket->addMessage(base_header, nullptr);

        for(int i = 0; i< inOrderFragments.size(); i++) {
            auto header = (L2HeaderUnicast*)inOrderFragments[i].first;

            emit("arq_seqno_passed_up", (double)header->getSeqno().get());
            completePacket->addMessage(inOrderFragments[i].first, inOrderFragments[i].second);

        }

        IRlc* rlc = getUpperLayer();
        emit("arq_bits_sent_up", (double)completePacket->getBits());
        return rlc->receiveFromLower(completePacket);
    }
    emitStatistics();
}

void SelectiveRepeatArq::processIncomingHeader(L2Packet* incoming_packet) {
    return;
}

void SelectiveRepeatArq::onEvent(double time) {

}

void SelectiveRepeatArq::emitStatistics() {
    double received_out_of_sequence = 0;
    double rtx = 0;
    double unacked = 0;

    for (auto it = arqProcesses.begin(); it != arqProcesses.end(); it++) {
        auto process = it->second;
        received_out_of_sequence += process->getNumReceivedOutOfSequence();
        rtx += process->getNumRtx();
        unacked += process->getNumUnacked();
    }
    emit("arq_received_out_of_sequence",received_out_of_sequence);
    emit("arq_rtx_list", rtx);
    emit("arq_sent_unacked", unacked);
}

