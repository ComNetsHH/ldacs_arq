//
// Created by Sebastian Lindner on 31.08.20.
//

#include "SelectiveRepeatArq.hpp"
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

SelectiveRepeatArqProcess *SelectiveRepeatArq::getArqProcess(MacAddress address) {
    auto it = arqProcesses.find(address);
    if (it != arqProcesses.end()) {
        return it->second;
    }
    return nullptr;
}

void SelectiveRepeatArq::receiveFromLowerLayer(L2Segment *segment) {
    auto header = segment->getHeader();
    MacAddress srcAddress = header->getSrcAddress();
    auto process = getArqProcess(srcAddress);
    if (!process) {
        process = new SelectiveRepeatArqProcess(srcAddress);
        arqProcesses.insert(make_pair(srcAddress, process));
    }

    process->processLowerLayerSegment(segment);
}

vector<L2Segment *> SelectiveRepeatArq::getInOrderSegments() {
    vector<L2Segment *> segments;
    for (auto it = arqProcesses.begin(); it != arqProcesses.end(); it++) {
        auto process = it->second;
        auto processSegments = process->getInOrderSegments();
        for (auto const &inOrderSegment: processSegments) {
            segments.push_back(inOrderSegment);
        }
    }

    return segments;
}

bool SelectiveRepeatArq::hasRtxSegment(MacAddress address, B size) {
    auto process = getArqProcess(address);
    if (!process) {
        return false;
    }
    return process->hasRtxSegment(size);
}

L2Segment * SelectiveRepeatArq::getRtxSegment(MacAddress address, B size) {
    auto process = getArqProcess(address);
    if (!process) {
        return nullptr;
    }
    return process->getRtxSegment(size);
}

int SelectiveRepeatArq::getNumProcesses() {
    return arqProcesses.size();
}
