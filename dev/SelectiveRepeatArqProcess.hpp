//
// Created by Konrad Fuger on 12.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP

#include "TYPES.h"
#include "SequenceNumber.hpp"
#include "L2Segment.hpp"
#include <list>
#include <queue>

using namespace std;

namespace TUHH_INTAIRNET_ARQ {
    class SelectiveRepeatArqProcess {
    protected:
        MacAddress remoteAddress;
        /** The sequence number that was last passed up to the upper layer. */
        SequenceNumber seqno_lastPassedUp = SequenceNumber(SEQNO_UNSET);
        /** The sequence number that will be assigned to the next newly sent segment. */
        SequenceNumber seqno_nextToSend = SequenceNumber(SEQNO_FIRST);
        /** The sequence number that is expected to arrive next. */
        SequenceNumber seqno_nextExpected = SequenceNumber(SEQNO_FIRST);
        /** The sequence number of the last-acknowledged segment. */
        SequenceNumber seqno_lastAcked = SequenceNumber(SEQNO_FIRST);
        /** List of segments that were sent and were not acknowledged yet. */
        std::list<L2Segment*> list_sentUnacked;
        /** List of segments that were sent and were not acknowledged yet. */
        list<L2Segment*> list_toPassUp;
        /** List of segments that should be retransmitted. */
        std::list<L2Segment*> list_rtx;
        /** List of segments that should be acknowledged. */
        std::list<L2Segment*> list_toAck;
        /** Queue of segments to send. */
        std::queue<L2Segment*> sending_buffer;
        /** List of received out-of-order segments. */
        std::list<L2Segment*> list_rcvdOutOfSeq;
        // out of sequence ACKs shouldn't be necessary?
        /** For logging purposes. */
        std::vector<SequenceNumber> received_segments, received_acks, sent_segments, sent_and_acked_segments, sent_acks;
        bool should_send_now = false;

        void processAck(L2Segment *segment);

    public:
        SelectiveRepeatArqProcess(MacAddress remoteAddress);

        /** If a ARQ process has no internal state anymore, it can be deleted **/
        bool isStale();

        /** Add a segment from lower layer**/
        void processLowerLayerSegment(L2Segment *segment);

        /** Add a segment received from Rlc to fill its header fields **/
        void processUpperLayerSegment(L2Segment *segment);

        /** Receive all in order Segments **/
        vector<L2Segment*> getInOrderSegments();

        bool hasRtxSegment(B size);
        L2Segment* getRtxSegment(B size);

    };

}

#endif //TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP
