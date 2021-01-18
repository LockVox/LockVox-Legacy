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

}

void CServer::deserializeClients(QJsonArray & json_array){

    foreach( const QJsonValue & value, json_array){

        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CClient * newClient = deserializeToClient(obj);
        //qDebug() << "Channel : " << newChannel->get_id()<< newChannel->get_name()<< Qt::endl;

        //check if the channel already exist or not
        bool exist = false;
        //if the channel exist, we reload it with new value
        foreach(CClient * c, get_clientList()){
            if(c->get_id() == newClient->get_id()){
                 exist = true;
                c->set_all(newClient);
            }
        }

        //if the channel doesnt exist, we add it to the list of channel
        if(get_channelList().isEmpty() || exist == false){
            qDebug() << "That channel doesnt exist, gonna create it " << Qt::endl;
        }

    }
}


























