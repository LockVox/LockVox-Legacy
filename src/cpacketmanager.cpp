#include "src/includes/cpacketmanager.h"

CPacketManager::CPacketManager(){
    m_counter = 0;
}

CPacketManager::~CPacketManager(){


}

bool CPacketManager::verify()
{
    return true;
}

int CPacketManager::assignId()
{
    return 0;
}

int CPacketManager::findPeerPacket(CPacket packet)
{
    /*foreach(CPacket p , m_pendingPackets){
        if(p.GetId() == packet.GetId()){
            return
        }
    }*/
    return -1;
}

void CPacketManager::processRequest(CPacket packet)
{
    switch(packet.getType()){
        case _SERVER:
            processAction(ServerAction(packet.getAction()), packet);
            break;
        case _CHANNELS:
             processAction(ChannelAction(packet.getAction()), packet);
            break;
        case _USERS:
            processAction(UserAction(packet.getAction()), packet);
            break;
        case _MESSAGES:
            processAction(MessageAction(packet.getAction()), packet);
            break;
        case _CONF:
            processAction(ConfAction(packet.getAction()), packet);
        break;
        default:
            break;
    }
}

void CPacketManager::processAction(ServerAction action, CPacket packet)
{
    switch(action){
        case AUTH:
        {
            sAuthentication info_auth = packet.deserialize_auth();
            emit m_self->auth(info_auth);
            break;
        }
        case REG:
        {
            sRegister info_reg = packet.deserialize_reg();
            emit m_self->reg(info_reg);
            break;
        }
        default:
        break;
    }
}

void CPacketManager::processAction(ChannelAction action, CPacket packet)
{

}

void CPacketManager::processAction(UserAction action, CPacket packet)
{
    switch(action){
        case UPDATE_USERNAME:
            emit m_self->user_updateParams(ClientParams::USERNAME, packet.getSender(), packet.deserialize_param());
        break;
        case UPDATE_DESCRIPTION:
            emit m_self->user_updateParams(ClientParams::DESCRIPTION, packet.getSender(), packet.deserialize_param());
        break;
        case UPDATE_MAIL:
            emit m_self->user_updateParams(ClientParams::MAIL, packet.getSender(), packet.deserialize_param());
        break;
        case UPDATE_PASSWORD:
           // emit m_self->user_updateParams(ClientParams::PASSWORD, packet.getSender(), packet.deserialize_param());
        break;
        default:

        break;
    }
}



void CPacketManager::sendPacket(CPacket packet, CClient * sender, CClient *receiver)
{
   packet.SetId(m_counter);
   m_counter++;

   packet.setSender(sender);
   packet.setReceiver(receiver);
   packet.setCookie(sender->getSessionCookie()->getCookie());

   //m_outgoingPackets.push_back(packet);
   packet.getReceiver()->get_socket()->write(packet.GetByteArray());
}

void CPacketManager::receivePacket(CPacket packet){

    packet.Deserialize();

    m_receivedPackets.push_back(packet);

    if(packet.getReq() == ANS){

    }

    if(packet.getReq() == REQ){
        processRequest(packet);
    }

    /*
    if(packet.getClass() == NOTIFY){
        //adeserialize and apply changes
        emit pNotify(packet); 
    }
    else if(packet.getClass() == INFO){  
        if(packet.GetRequest() == REQ){
            emit  pReqInfo(packet);
        }
        if(packet.GetRequest() == ANS){
            emit  pAnsInfo(packet);
            removePending(packet.GetId());
        }
    }
    else if(packet.getClass() == ACTION){
        if(packet.GetRequest() == REQ){
            emit  pReqAction(packet);
        }
        if(packet.GetRequest() == ANS){
            emit  pAction(packet);
            removePending(packet.GetId());
        }
    }
    else {
        //Default? 
    }*/
}




/*
PacketManager : 

- Store sended request until answer has been received

- Manage packet class, type and action

- Automatically serialize objects.

-

-

-







*/
