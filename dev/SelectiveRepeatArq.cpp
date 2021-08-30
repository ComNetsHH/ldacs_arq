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

int tmp_idx = 0;

SelectiveRepeatArq::SelectiveRepeatArq(uint8_t resend_timeout, uint8_t window_size)
        : resend_timeout(resend_timeout), window_size(window_size) {
}

bool SelectiveRepeatArq::isInRtxState() const {
    return false;
}

unsigned int SelectiveRepeatArq::getBufferStatus() {
    throw std::logic_error("not implemented");
}

SelectiveRepeatArqProcess *SelectiveRepeatArq::getArqProcess(MacId address) {
    auto it = arqProcesses.find(address);
    if (it != arqProcesses.end()) {
        return it->second;
    }
    auto process = new SelectiveRepeatArqProcess(address);
    arqProcesses.insert(make_pair(address, process));
    return process;
}

void SelectiveRepeatArq::receiveFromLowerLayer(L2Packet *packet) {
    MacId srcAddress = PacketUtils::getSrcAddress(packet);
    auto process = getArqProcess(srcAddress);
    if (!process) {
        process = new SelectiveRepeatArqProcess(srcAddress);
        arqProcesses.insert(make_pair(srcAddress, process));
    }

    auto fragments = PacketUtils::getUnicastFragments(packet);

    for(int i = 0; i< fragments.size(); i++) {
        process->processLowerLayerSegment(fragments[i]);
    }
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
    process = new SelectiveRepeatArqProcess(id);
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
    // TODO add rtx data
    IMac* mac = getLowerLayer();
    mac->notifyOutgoing(num_bits, mac_id);
}

L2Packet* SelectiveRepeatArq::requestSegment(unsigned int num_bits, const MacId& mac_id) {
    debug("SelectiveRepeatArq::requestSegment " + std::to_string(num_bits));
    if(hasRtxSegment(mac_id, num_bits)) {
        return getRtxSegment(mac_id, num_bits);
    }
    IRlc* rlc = getUpperLayer();
    L2Packet* segment = rlc->requestSegment(num_bits, mac_id);
    auto fragments = PacketUtils::getUnicastFragments(segment);
    auto process = getArqProcess(mac_id);

    for(int i=0; i< fragments.size(); i++) {
        process->processUpperLayerSegment(fragments[i]);
        SequenceNumber seqNo = ((L2HeaderUnicast*)(fragments[i].first))->getSeqno();
        //cout << ("SelectiveRepeatArq::requestSegment: SeqNo" + std::to_string((int)seqNo.get())) << endl;
    }
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
    if(++tmp_idx == 10) {
        return;
    }


    debug("SelectiveRepeatArq::receiveFromLower");
    MacId dest = packet->getDestination();
    auto process = getArqProcess(dest);
    auto fragments = PacketUtils::getUnicastFragments(packet);

    for(int i = 0; i< fragments.size(); i++) {
        process->processLowerLayerSegment(fragments[i]);
    }

    auto inOrderFragments = process->getInOrderSegments();

    if(inOrderFragments.size() > 0) {
        L2Packet* completePacket = packet->copy();
        for(int i = 0; i< inOrderFragments.size(); i++) {
            completePacket->addMessage(inOrderFragments[i].first, inOrderFragments[i].second);
        }

        IRlc* rlc = getUpperLayer();
        return rlc->receiveFromLower(packet);
    }
}

void SelectiveRepeatArq::processIncomingHeader(L2Packet* incoming_packet) {
    return;
}

void SelectiveRepeatArq::onEvent(double time) {

}

