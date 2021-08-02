CPacketManager::CPacketManager(){


}

~CPacketManager::CPacketManager(){


}

CPacketManager::receivePacket(CPacket packet){

    packet.Deserialize();          
    m_receivedPackets.push(packet); 
    
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
    }
}


/*
PacketManager : 

- Store sended request until answer has been received

- Manage packet class, type and action

- Automatically serialize objects.

-

-

-







/*