//
// Created by Sebastian Lindner on 31.08.20.
//

#include "SelectiveRepeatArq.hpp"
#include "PacketUtils.hpp"
#include <cstdint>
#include <stdexcept>

using namespace TUHH_INTAIRNET_ARQ;

SelectiveRepeatArq::SelectiveRepeatArq(uint8_t resend_timeout, uint8_t window_size)
        : resend_timeout(resend_timeout), window_size(window_size) {
}

bool SelectiveRepeatArq::isInRtxState() const {
    return false;
}

B SelectiveRepeatArq::getBufferStatus() {
    throw std::logic_error("not implemented");
}

SelectiveRepeatArqProcess *SelectiveRepeatArq::getArqProcess(MacId address) {
    auto it = arqProcesses.find(address);
    if (it != arqProcesses.end()) {
        return it->second;
    }
    return nullptr;
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

bool SelectiveRepeatArq::hasRtxSegment(MacId address, B size) {
    auto process = getArqProcess(address);
    if (!process) {
        return false;
    }
    return process->hasRtxSegment(size);
}

L2Packet * SelectiveRepeatArq::getRtxSegment(MacId address, B size) {
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
