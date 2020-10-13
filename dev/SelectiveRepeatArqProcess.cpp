//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacAddress destAddress) {
    this->destAddress = destAddress;
}

vector<L2Segment> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<L2Segment> result;
    return result;
}

void SelectiveRepeatArqProcess::addSegment(const L2Segment &segment) {
    // TODO: Implementation
}