// The L-Band Digital Aeronautical Communications System (LDACS) Automatic Repeat Request (ARQ) implements the ARQ protocol for the LDACS Air-Air Medium Access Control simulator.
// Copyright (C) 2023  Sebastian Lindner, Konrad Fuger, Musab Ahmed Eltayeb Ahmed, Andreas Timm-Giel, Institute of Communication Networks, Hamburg University of Technology, Hamburg, Germany

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQPROCESSTEST_HPP

#include "TYPES.h"
#include "PacketFragment.hpp"
#include "SequenceNumber.hpp"
#include "IOmnetPluggable.hpp"
#include "L2Packet.hpp"
#include "MacId.hpp"
#include <list>
#include <map>
#include <queue>

using namespace std;
using namespace TUHH_INTAIRNET_MCSOTDMA;

namespace TUHH_INTAIRNET_ARQ {
    // Forward declare SelectiveRpeatArq
    class SelectiveRepeatArq;

    class SelectiveRepeatArqProcess: public IOmnetPluggable {
    protected:
        /** encodes wether the Arq has last sent or received **/
        enum ArqState {
            undefined,
            sent_last,
            received_last
        };

        /** Mac address of the communication of this process **/
        MacId remoteAddress;

        /** Reference to the parent process **/
        SelectiveRepeatArq * arq = nullptr;

        /** My MacId **/
        MacId address;

        uint8_t resend_timeout;

        uint8_t window_size;

        int maxTx = 3;

        ArqState state = ArqState::undefined;

        /** Remembers the number of tx attempts for each seqno **/
        std::map<int, int> txCount;

        /** Remembers the number of srej notifications for each seqno **/
        map<int, int> srejCount;

        /** The sequence number that was last passed up to the upper layer. */
        SequenceNumber seqno_lastPassedUp = SequenceNumber(SEQNO_UNSET);

        /** The sequence number that will be assigned to the next newly sent segment. */
        SequenceNumber seqno_nextToSend = SequenceNumber(SEQNO_FIRST);

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

        PacketFragment copyFragment(PacketFragment fragment);

        bool isUnacked(SequenceNumber seqNo);

    public:
        /** Standard constructor **/
        SelectiveRepeatArqProcess(SelectiveRepeatArq* parent, MacId address, MacId remoteAddress, int maxTx, uint8_t resend_timeout = 0, uint8_t window_size = SEQNO_MAX / 2);
        SelectiveRepeatArqProcess(MacId address, MacId remoteAddress, int maxTx = 0, uint8_t resend_timeout = 0, uint8_t window_size = SEQNO_MAX / 2);

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
