#include "Client/cserver.h"


CServer::CServer()
{

            qDebug() << "Starting LockVox client ! Welcome !" << Qt::endl;
            m_socket = new QTcpSocket();

            m_socket->abort();
            m_socket->connectToHost("127.0.0.1", 50885);

            connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReceiveData()));

}


//Getters
QTcpSocket * CServer::get_socket(){
    return m_socket;
}

//Setters
void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}

//Envoie de l'audio au server grâce à un QByteArray
void CServer::sendToServer(QByteArray ba){
    //qDebug() << "Data has been send to Server ";
    m_socket->write(ba);
}

void CServer::sendToServer(){

}


void CServer::onReceiveData(){

    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
          return;

    qDebug() << "Receive msg \n";

    QByteArray *data = new QByteArray();
    data->append(socket->readAll());

    //Process data
    processIncomingData(*data);

}

void CServer::processIncomingData(QByteArray data){
    CPacket * packet = new CPacket(data,NULL);

    packet->Deserialize();

    if(packet->GetAction().toInt() == -1 && packet->GetType().toInt() == -1){
       qDebug() << "Receive Server obj\n";
       Deserialize(data);
       emit(changeState(1));
    }


    //Récupération du type
    switch (packet->GetType().toInt()) {
    case 0:{ //SERV
            switch (packet->GetAction().toInt())
            {
            case 0:
            {
                //New User is now online
                CClient * client = new CClient();
                client = packet->Deserialize_newClient();

                for(int i = 0; i < get_clientList().size(); i++){
                    if(get_clientList()[i]->get_id() == client->get_id()){
                        get_clientList()[client->get_id()]->set_isOnline(false);
                    }
                }

                free(client);

                break;
            }
            case 1:
            {
                //User is now offline
                CClient * client = packet->Deserialize_newClient();

                for(int i = 0; i < get_clientList().size(); i++){
                    if(get_clientList()[i]->get_id() == client->get_id()){
                        get_clientList()[client->get_id()]->set_isOnline(false);
                    }
                }
                free(client);

                break;
            }
            case 2:
            {
                //PSEUDO UPDATE
                CClient * c = packet->Deserialize_newClient();
                CClient * client = get_clientById(c->get_id());
                client->set_pseudo(c->get_pseudo());
                break;
            }
            case 3:
            {
                //BIO UPDATE
                //Comme au dessus
                //get_clientList()[sender->get_id()]->set_pseudo(packet->GetData()["description"].toString());
                QJsonObject infos;
                //infos.insert("id", sender->get_id());
                infos.insert("description", packet->GetData()["description"]);
                QJsonDocument doc(infos);
                QByteArray ret;
                //ret.push_back(packet->GetType());
                //ret.push_back(packet->GetAction());
                //sendToAll(ret);
                break;
            }
            case 4:
                //BAN USER
                break;
            case 5:{
                //BAN IP
                //Rajouter système de gestion du temps
                break;
                }
            case 6: {
                //Kick user

                break;
                }
        break;
        }
    case 1: //CHAN
        switch (packet->GetAction().toInt())
        {
        case 0: {
            //CONNECT CHAN
            packet->Deserialize_ID();

            CClient * client = get_clientById(packet->get_IdClient());
            CChannel * channel = get_channelById(packet->get_IdChannel());

            if(channel && client){
                client->set_idChannel(channel->get_id());
                channel->addUser(client);
            }
            break;
        }
        case 1: {
            //QUIT CHAN
            packet->Deserialize_ID();

            CClient * client = get_clientById(packet->get_IdClient());
            CChannel * channel = get_channelById(packet->get_IdChannel());


            if(channel && client){
                client->set_idChannel(-1);
                channel->delUser(client->get_id());
            }
            break;
        }
        case 5: {
            //Create chan voc
            CChannel * c = packet->Deserialize_newChannel();
            addChannel(c);

            break;
        }
        case 6: {
            //Delete chan voc
            CChannel * c = packet->Deserialize_newChannel();
            CChannel * toDelChannel = get_channelById(c->get_id());
            DelChannel(toDelChannel);
            break;
        }
        case 7: {
            //Rename chan voc
            CChannel * c = packet->Deserialize_newChannel();

            CChannel * channel = get_channelById(c->get_id());
            channel->set_name(c->get_name());
            break;
        }
        case 8: {
            //Modif max user (voc)

            break;
        }
        case 9: {
            //kick user voc

            break;
        }
        case 10: {
            //Mute user voc (server side)

            break;
        }
        case 11:
            //Create chan text --------> Qxmpp
            break;
        case 12:
            //Delete cahn text
            break;
        case 13:
            //Rename chan text
            break;
        default:
            qDebug() << "Error invalid action" << Qt::endl;
        }
        break;
    case 2: //USER
        switch (packet->GetAction().toInt())
        {
        case 0:
            //Mute (user side) ?????
            break;
        case 1:
            //Add friend --> later
            break;
        case 2:
            //Del friend
            break;
        case 3:
            //Send msg to friend
            break;
        case 4:
            //Modif pseudo (update bdd)
            break;
        case 5:
            //Change right
            break;
        default:
            qDebug() << "Error invalid action" << Qt::endl;
        }
        break;
    default:
        qDebug() << "That action isn't listed : " << packet->GetType() << " " <<packet->GetAction() << Qt::endl;
    }
    return;
}


}




QByteArray CServer::Serialize(){

    QJsonObject obj;
    QJsonArray cArray, sArray;

    foreach(CChannel * c, get_channelList()){
        cArray.append(c->serializeToObj());
    }
    //foreach(CClient * c, get_clientList()){
       // sArray.append(c->serializeToObj());
    //}
    obj["channels"] = cArray;
    //obj["clients"] = sArray;



    QJsonDocument jsonDoc(obj);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();

}

void CServer::Deserialize(QByteArray in){

    QJsonDocument jsonDoc = QJsonDocument::fromJson(in);
    QJsonObject obj = jsonDoc.object();

    QJsonArray sArray = obj["clients"].toArray();
    QJsonArray cArray = obj["channels"].toArray();

    deserializeChannel(cArray);
    deserializeClients(sArray);

}

QByteArray CServer::SerializeChannels(){
    QJsonArray jsonArray;
    foreach(CChannel * c, get_channelList()){
        jsonArray.append(c->serializeToObj());
    }
    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();

}

QByteArray CServer::SerializeClients(){
    QJsonArray jsonArray;
    foreach(CClient * c, get_clientList()){
        jsonArray.append(c->serializeToObj());
    }
    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();
}

void CServer::DeserializeChannels(QByteArray in){

        //Deserialize byte array into a json document
        QJsonDocument jsonDoc = QJsonDocument::fromJson(in);

        if(jsonDoc.isNull()){
            qDebug() <<"JSON doc is invalid (null)" << Qt::endl;
        }

        QJsonArray jsonArray = jsonDoc.array();

        //Get each element of the array
        foreach( const QJsonValue & value, jsonArray){

            //Convert it to an json object then to a channel
            QJsonObject obj = value.toObject();
            CChannel * newChannel = deserializeToChannel(obj);
            //qDebug() << "Channel : " << newChannel->get_id()<< newChannel->get_name()<< Qt::endl;

            //check if the channel already exist or not
            bool exist = false;
            //if the channel exist, we reload it with new value
            foreach(CChannel * c, get_channelList()){
                if(c->get_id() == newChannel->get_id()){
                    exist = true;
                    c->set_all(newChannel);
                }
            }
            //if the channel doesnt exist, we add it to the list of channel
            if(get_channelList().isEmpty() || exist == false){
                qDebug() << "That channel doesnt exist, gonna create it " << Qt::endl;
                addChannel(newChannel);
            }
        }

        //Print content of the actual list of channel - check
        /*
        foreach(CChannel * c, get_channelList()){
         qDebug() << "Channel :\nName: " << c->get_name()<< Qt::endl;
         qDebug() << "ID: " << c->get_id()<< Qt::endl;
         qDebug() << "MaxUsers: " << c->get_maxUsers()<< Qt::endl;
         qDebug() << "NbClients: " << c->get_nbClients()<< Qt::endl;
        }*/

}

CChannel * CServer::deserializeToChannel(QJsonObject json_obj){
    CChannel * channel = new CChannel();

    channel->deserialize(json_obj);

    return channel;
}

CClient * CServer::deserializeToClient(QJsonObject json_obj){
    CClient * client = new CClient();
    client->deserialize(json_obj);
    return client;
}

void CServer::deserializeChannel(QJsonArray & json_array){

    foreach( const QJsonValue & value, json_array){

        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CChannel * newChannel = deserializeToChannel(obj);


        //check if the channel already exist or not
        bool exist = false;
        foreach(CChannel * c, get_channelList()){
            if(c->get_id() == newChannel->get_id())
                 exist = true;
        }

        if(exist == false){
            addChannel(newChannel);
        }
    }
}

void CServer::deserializeClients(QJsonArray & json_array){

    foreach( const QJsonValue & value, json_array){
        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CClient * newClient = deserializeToClient(obj);

        //check if the channel already exist or not
        bool exist = false;
        foreach(CClient * c, get_clientList()){
            if(c->get_id() == newClient->get_id())
                 exist = true;
        }

        if(exist == false)
            addClient(newClient);

    }
}




























