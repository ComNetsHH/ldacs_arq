//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_SEQUENCENUMBER_HPP
#define TUHH_INTAIRNET_ARQ_SEQUENCENUMBER_HPP

#include <cstdint>

namespace TUHH_INTAIRNET_ARQ {
	class SequenceNumber {
		public:
			SequenceNumber();
			SequenceNumber(uint8_t sequence_number);
			SequenceNumber(const SequenceNumber& other);

			bool operator==(const SequenceNumber& other) const;
            bool operator>(const SequenceNumber& other) const;
            bool operator<(const SequenceNumber& other) const;
			
			uint8_t get() const;
			
			void increment();
			
		protected:
			uint8_t sequence_number;
	};
	
	static const uint8_t SEQNO_MAX = 255;
	static const uint8_t SEQNO_UNSET = 0;
	static const uint8_t SEQNO_FIRST = 1;
	static const uint8_t NUM_BYTES = 1;
}


#endif //TUHH_INTAIRNET_ARQ_SEQUENCENUMBER_HPP
