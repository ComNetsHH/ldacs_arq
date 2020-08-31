//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP
#define TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP

#include "L2SegmentHeader.hpp"

namespace TUHH_INTAIRNET_ARQ {
	class Payload;
	
	class L2Segment {
		public:
			L2Segment();
		
		protected:
			L2SegmentHeader header;
			Payload* payload;
	};
}


#endif //TUHH_INTAIRNET_ARQ_L2SEGMENT_HPP
