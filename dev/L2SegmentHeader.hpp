//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_L2SEGMENTHEADER_HPP
#define TUHH_INTAIRNET_ARQ_L2SEGMENTHEADER_HPP


#include <cstdint>
#include <vector>
#include "SequenceNumber.hpp"

namespace TUHH_INTAIRNET_ARQ {
	class L2SegmentHeader {
		public:
			L2SegmentHeader();

			/**
			 *
			 * @param seqno This segment's sequence number.
			 */
			void setSeqno(SequenceNumber seqno);
			
			/**
			 *
			 * @param seqno Sequence number of next segment expected at the sender of this segment.
			 */
			void setSeqnoNextExpected(SequenceNumber seqno);
			
			/**
			 *
			 * @param seqnos List of sequence numbers that are deemed missing at the sender of this segment.
			 */
			void setSrejList(std::vector<SequenceNumber> seqnos);
			
			/**
			 *
			 * @return This segment's sequence number.
			 */
			SequenceNumber getSeqno() const;
			
			/**
			 *
			 * @return Sequence number of next segment expected at this segment's sender.
			 */
			SequenceNumber getSeqnoNextExpected() const;
			
			/**
			 *
			 * @return Number of items in the selective rejection list.
			 */
			uint8_t getSrejListLength() const;
			
			/**
			 *
			 * @return List of sequence numbers of missing segments.
			 */
			std::vector<SequenceNumber> getSrejList() const;
			
			/**
			 *
			 * @return Number of reserved slots until an acknowledgement is expected.
			 */
			uint8_t getPollSlots() const;
			
			/**
			 *
			 * @return This header's size in bytes.
			 */
			uint64_t getSize() const;
			
		protected:
			/** This segment's sequence number. */
			SequenceNumber seqno;
			/** Sequence number next expected by the sender of this segment. */
			SequenceNumber seqno_next_expected;
			/** Length of the selective rejection list. */
			uint8_t srej_list_length;
			/** Selective rejection list. */
			std::vector<SequenceNumber> srej_list;
			/** Number of slots until an acknowledgement is expected. */
			uint8_t poll;
	};
}


#endif //TUHH_INTAIRNET_ARQ_L2SEGMENTHEADER_HPP
