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
			
			void setSeqno(SequenceNumber seqno);
			
			void SetSeqnoNextExpected(SequenceNumber seqno);
			
			void setSrejList(std::vector<SequenceNumber> seqnos);
			
			SequenceNumber getSeqno() const;
			
			SequenceNumber getSeqnoNextExpected() const;
			
			uint8_t getSrejListLength() const;
			
			std::vector<SequenceNumber> getSrejList() const;
			
			uint8_t getPollSlots() const;
			
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
