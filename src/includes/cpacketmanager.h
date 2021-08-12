#ifndef CPACKETMANAGER_H
#define CPACKETMANAGER_H

#include "src/includes/cpacket.h"
#include "cclient.h"

enum PacketPriority {
    LOW, MEDIUM, HIGH, CRITICAL
};
enum PacketClass;
enum Type;
enum ServerAction;
enum ChannelAction;
enum UserAction;
enum MessageAction;
enum ConfAction;

class CPacket;

class CPacketManager {


    public:

        CPacketManager();
        ~CPacketManager();

        bool verify();
        int assignId();                                             //!< Find out available ID
        int findPeerPacket(CPacket p);
        void catchError();

        //Process type of the packet
        void processRequest(CPacket p);
        void processAnswer();
        //Process action according to the type of the packet
        void processAction(ServerAction action, CPacket packet);
        void processAction(ChannelAction action, CPacket packet);
        void processAction(UserAction action, CPacket packet);
        void processAction(MessageAction action, CPacket packet);
        void processAction(ConfAction action, CPacket packet);


        int removePending(int id);
        void resendPending(int id);

        void sendPacket(CClient * receiver, PacketClass c, Type type, int action);
        void sendPacket(CPacket packet, CClient * sender, CClient *receiver);


signals:


    public slots:

        void receivePacket(CPacket packet);
        void writePacket(CPacket packet);
        void writePacket(CClient * receiver, PacketClass c, Type type, int action);
        
        //INFO
        //NOTIFY
        //

        //For tasks manager- TODO
        void trigger();
    private:

    QList<CPacket> m_pendingPackets;                             //!< List of pending packets which require an answer. 
    QList<CPacket> m_outgoingPackets;                            //!< List of packet who must be send
    QList<CPacket> m_receivedPackets;                            //!< When receive a packet, it is add to this list for further processing. 

    CClient * m_self;

    int m_counter;
};

#endif //CPACKETMANAGER_H
