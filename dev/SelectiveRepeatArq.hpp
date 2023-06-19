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

#ifndef TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP
#define TUHH_INTAIRNET_ARQ_SELECTIVEREPEATARQ_HPP

#include <list>
#include <cstdint>
#include <queue>
#include <map>
#include "SequenceNumber.hpp"
#include "IArq.hpp"
#include "IOmnetPluggable.hpp"
#include "SelectiveRepeatArqProcess.hpp"


using namespace std;

namespace TUHH_INTAIRNET_ARQ {
    class SelectiveRepeatArq: public IArq, public IOmnetPluggable {
    public:
        /** Standard constructor with resend_timeout and window_size params **/
        SelectiveRepeatArq(MacId address, uint8_t resend_timeout, uint8_t window_size, int maxTx = 4, double per = 0);

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

        /** Decides wether a link should be protected default yes **/
        bool shouldLinkBeArqProtected(const MacId& mac_id);

        /** used by any process to trigger a new reporting of more data **/
        void reportRtxData(MacId dest);

        /* handle notification about new link */
        void notifyAboutNewLink(const MacId& id) override;

        /* handle notification about removed link */
        void notifyAboutRemovedLink(const MacId& id) override;

    protected:
        /* current number of rtx */
        int numRtx = 0;

        /** Max number of transmissions. */
        int maxTx = 0;

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
