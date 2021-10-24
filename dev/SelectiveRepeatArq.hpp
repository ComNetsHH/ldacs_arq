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
#include "IArq.hpp"
#include "IOmnetPluggable.hpp"
#include "TYPES.h"
#include "SelectiveRepeatArqProcess.hpp"


using namespace std;

namespace TUHH_INTAIRNET_ARQ {
    class SelectiveRepeatArq: public IArq, public IOmnetPluggable {
    public:
        /** Standard constructor with resend_timeout and window_size params **/
        SelectiveRepeatArq(MacId address, uint8_t resend_timeout, uint8_t window_size, double per = 0);

        /** bringing back per here to run sims without touching any control packets **/
        double per = 0;

        /** Method for the MAC layer to query the buffer status. Will refer to RLC for answer **/
        unsigned int getBufferStatus();

        /** Returns true if any of the ArqProcesses has data to retransmit **/
        bool isInRtxState() const;

        SelectiveRepeatArqProcess *getArqProcess(MacId address);

        int getNumProcesses();

        /** function to query for a rtx segment **/
        bool hasRtxSegment(MacId address, unsigned int size);

        /** Function to request a segment for a given MacAddress which is currenlty scheduled to be sent to **/
        L2Packet* getRtxSegment(MacId address, unsigned int size);

        /** Get Segments to be passed up **/
        vector<PacketFragment> getInOrderSegments();

        /** TODO: **/
        bool shouldLinkBeArqProtected(const MacId& mac_id);


        void notifyAboutNewLink(const MacId& id) override;
        void notifyAboutRemovedLink(const MacId& id) override;
    protected:
        /** Number of retransmissions. */
        int numRtx = 0;

        /** Time until an unacknowledged segment is scheduled for retransmission. */
        uint8_t resend_timeout;

        /** Window size. */
        uint64_t window_size;

        /** My MacId **/
        MacId address;

        /** ArqProcesses for each communication partner **/
        map<MacId, SelectiveRepeatArqProcess *> arqProcesses;

        /** Clean all stale state **/
        void cleanUp();

        /** emit all kinds of statictics **/
        void emitStatistics();

    public:
        void notifyOutgoing(unsigned int num_bits, const MacId& mac_id) override;

        L2Packet* requestSegment(unsigned int num_bits, const MacId& mac_id) override;

        bool shouldLinkBeArqProtected(const MacId& mac_id) const override;

        void injectIntoUpper(L2Packet* packet) override;

        void receiveFromLower(L2Packet* packet) override;

        void onEvent(double time) override;

    protected:
        void processIncomingHeader(L2Packet* incoming_packet) override;
    };
}


#endif //TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
