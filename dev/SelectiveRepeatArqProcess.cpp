//
// Created by Konrad Fuger on 12.10.20.
//

#include "SelectiveRepeatArqProcess.hpp"
#include <iostream>

using namespace TUHH_INTAIRNET_ARQ;
using namespace std;

SelectiveRepeatArqProcess::SelectiveRepeatArqProcess(MacAddress destAddress) {
    this->destAddress = destAddress;
}

vector<L2Segment> SelectiveRepeatArqProcess::getInOrderSegments() {
    vector<L2Segment> result;
    for (auto const& segment: list_toPassUp) {
        result.push_back(segment);
    }
    list_toPassUp.clear();
    return result;
}

void SelectiveRepeatArqProcess::addSegment(const L2Segment &segment) {
    SequenceNumber seqNo = segment.getHeader().getSeqno();

    if (seqNo.get() == seqno_lastPassedUp.get() + 1) {
        list_toPassUp.push_back(segment);
        seqno_lastPassedUp.increment();

        for (auto it = list_rcvdOutOfSeq.begin(); it != list_rcvdOutOfSeq.end(); it++) {
            seqNo = (*it).getHeader().getSeqno();
            if (seqNo.get() == seqno_lastPassedUp.get() + 1) {
                list_toPassUp.push_back(*it);
                seqno_lastPassedUp.increment();
                list_rcvdOutOfSeq.erase(it);
            }
        }
    } else {
        list_rcvdOutOfSeq.push_back(segment);
        list_rcvdOutOfSeq.sort([](const L2Segment &s1, const L2Segment &s2) {
            auto h1 = s1.getHeader().getSeqno();
            auto h2 = s2.getHeader().getSeqno();
            return h1 < h2;
        });
    }
}