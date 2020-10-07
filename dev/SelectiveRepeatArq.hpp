//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP

#include <list>
#include <cstdint>
#include <queue>
#include "SequenceNumber.hpp"
#include "L2Segment.hpp"

namespace TUHH_INTAIRNET_ARQ {
	class SelectiveRepeatArq {
		public:
			SelectiveRepeatArq(uint8_t resend_timeout, uint8_t window_size);
			
			virtual void receiveFromUpperLayer(const L2Segment& segment);
			
			virtual void receiveFromLowerLayer(const L2Segment& segment);
		
		protected:
			virtual void passToLowerLayer(const L2Segment& segment) = 0;
			
			virtual void passToUpperLayer(const L2Segment& segment) = 0;
			
			void processData(const L2Segment& segment);
			
			void processAck(const L2Segment& segment);
			
			bool isInRtxState() const;
		protected:
			/** Time until an unacknowledged segment is scheduled for retransmission. */
			uint8_t resend_timeout;
			/** Window size. */
			uint64_t window_size;
			/** The sequence number that will be assigned to the next newly sent segment. */
			SequenceNumber seqno_nextToSend = SequenceNumber(SEQNO_FIRST);
			/** The sequence number that is expected to arrive next. */
			SequenceNumber seqno_nextExpected = SequenceNumber(SEQNO_FIRST);
			/** The sequence number of the last-acknowledged segment. */
			SequenceNumber seqno_lastAcked = SequenceNumber(SEQNO_FIRST);
			/** List of segments that were sent and were not acknowledged yet. */
			std::list<L2Segment> list_sentUnacked;
			/** List of segments that should be retransmitted. */
			std::list<L2Segment> list_rtx;
			/** List of segments that should be acknowledged. */
			std::list<L2Segment> list_toAck;
			/** Queue of segments to send. */
			std::queue<L2Segment> sending_buffer;
			/** List of received out-of-order segments. */
			std::list<L2Segment> list_rcvdOutOfSeq;
			// out of sequence ACKs shouldn't be necessary?
			/** For logging purposes. */
			std::vector<SequenceNumber> received_segments, received_acks, sent_segments, sent_and_acked_segments, sent_acks;
			bool should_send_now = false;
	};
}


#endif //TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
