//
// Created by Sebastian Lindner on 31.08.20.
//

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP

#include <list>
#include <cstdint>
#include <queue>
#include <map>
#include "SequenceNumber.hpp"
#include "L2Segment.hpp"
#include "TYPES.h"
#include "SelectiveRepeatArqProcess.hpp"


using namespace std;

namespace TUHH_INTAIRNET_ARQ {
    class SelectiveRepeatArq {
    public:
        /** Standard constructor with resend_timeout and window_size params **/
        SelectiveRepeatArq(uint8_t resend_timeout, uint8_t window_size);

        /** Method for the MAC layer to query the buffer status. Will refer to RLC for answer **/
        B getBufferStatus();

        /** Handle a segment from MAC layer **/
        void receiveFromLowerLayer(L2Segment *segment);

        /** Returns true if any of the ArqProcesses has data to retransmit **/
        bool isInRtxState() const;

        SelectiveRepeatArqProcess *getArqProcess(MacAddress address);

        int getNumProcesses();

        /** function to query for a rtx segment **/
        bool hasRtxSegment(MacAddress address, B size);

        /** Function to request a segment for a given MacAddress which is currenlty scheduled to be sent to **/
        L2Segment* getRtxSegment(MacAddress address, B size);

        /** Get Segments to be passed up **/
        vector<L2Segment*> getInOrderSegments();
    protected:
        /** Time until an unacknowledged segment is scheduled for retransmission. */
        uint8_t resend_timeout;

        /** Window size. */
        uint64_t window_size;

        /** ArqProcesses for each communication partner **/
        map<MacAddress, SelectiveRepeatArqProcess *> arqProcesses;

        /** Clean all stale state **/
        void cleanUp();
    };
}


#endif //TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
