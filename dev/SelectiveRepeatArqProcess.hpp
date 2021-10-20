//
// Created by Konrad Fuger on 12.10.20.
//

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP

#include "TYPES.h"
#include "PacketFragment.hpp"
#include "SequenceNumber.hpp"
#include "IOmnetPluggable.hpp"
#include "L2Packet.hpp"
#include "MacId.hpp"
#include <list>
#include <queue>

using namespace std;
using namespace TUHH_INTAIRNET_MCSOTDMA;

namespace TUHH_INTAIRNET_ARQ {
    class SelectiveRepeatArqProcess: public IOmnetPluggable {
    protected:
        /** Mac address of the communication of this process **/
        MacId remoteAddress;

        uint8_t resend_timeout;

        uint8_t window_size;

        /** The sequence number that was last passed up to the upper layer. */
        SequenceNumber seqno_lastPassedUp = SequenceNumber(SEQNO_UNSET);

        /** The sequence number that will be assigned to the next newly sent segment. */
        SequenceNumber seqno_nextToSend = SequenceNumber(SEQNO_FIRST);
        int seqno_next_to_send  = 1;

        /** The sequence number that is expected to arrive next. */
        SequenceNumber seqno_nextExpected = SequenceNumber(SEQNO_FIRST);

        /** UNUSED: The sequence number of the last-acknowledged segment. */
        //SequenceNumber seqno_lastAcked = SequenceNumber(SEQNO_FIRST);

        /** List of segments that were sent and were not acknowledged yet. */
        std::list<PacketFragment> list_sentUnacked;

        /** List of segments that should be passed up to the higher layer */
        list<PacketFragment> list_toPassUp;

        /** List of segments that should be retransmitted. */
        std::list<PacketFragment> list_rtx;

        /** List of received out-of-order segments. */
        std::list<PacketFragment> list_rcvdOutOfSeq;

        /** For logging purposes. */
        std::vector<SequenceNumber> received_segments, received_acks, sent_segments, sent_and_acked_segments, sent_acks;
        bool should_send_now = false;

        void processAck(PacketFragment segment);

        bool wasReceivedOutOfOrder(SequenceNumber seqNo);

        /** Packet error rate imposed on the arq, INLY FOR DEBUGGING, REMOVE ME! */
        double per = 0;

    public:
        /** Standard constructor **/
        SelectiveRepeatArqProcess(MacId remoteAddress, double per = 0.0, uint8_t resend_timeout = 0, uint8_t window_size = SEQNO_MAX / 2);

        /** If a ARQ process has no internal state anymore, it can be deleted **/
        bool isStale();

        /** Add a segment from lower layer**/
        void processLowerLayerSegment(PacketFragment segment);

        /** Add a segment received from Rlc to fill its header fields **/
        void processUpperLayerSegment(PacketFragment segment);

        /** Receive all in order Segments **/
        vector<PacketFragment> getInOrderSegments();

        vector<SequenceNumber> getSrejList();

        bool hasRtxSegment(unsigned int size);
        L2Packet* getRtxSegment(unsigned int size);

        unsigned int getRtxSize();

        unsigned int getNumReceivedOutOfSequence();
        unsigned int getNumRtx();
        unsigned int getNumUnacked();

        MacId getMacId();


    };

}

#endif //TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP
