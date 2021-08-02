#ifndef CPACKET_H
#define CPACKET_H

#include "cpacket.h"

enum PacketPriority {
    LOW, MEDIUM, HIGH, CRITICAL
}

class CPacketManager {

    CPacketManager(); 
    ~CPacketManager(); 

    public:
        verify();
        assignId();                                             //!< Find out available ID

        removePending(int id);
        resendPending(int id); 


    signals: 
        pNotify(CClient * client, )
    slots:
        receivePacket(CPacket packet); 
        writePacket(CPacket packet); 

        writePacket(CClient * receiver, PacketClass class, Type type, action)
        
        //INFO

        //NOTIFY

        //


        //For tasks manager- TODO
        trigger();
    private:

    QList<CPacket> m_pendingPackets;                             //!< List of pending packets which require an answer. 
    QList<CPacket> m_outgoingPackets;                            //!< List of packet who must be send
    QList<CPacket> m_receivedPackets;                            //!< When receive a packet, it is add to this list for further processing. 

}

#endif //CPACKET_H