#include "Server/cserver.h"
#include "src/includes/cdatabase.h"


#include <QDebug>

CServer::CServer()
{
    qDebug() << "Starting LockVox Server - \n";

    // Gestion du serveur TCP
    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50885
    {
        // Si le serveur n'a pas été démarré correctement
        qDebug()<< "Le serveur n'a pas pu être démarré. Raison :" << serveur->errorString();
    }
    else
    {
        // Si le serveur a été démarré correctement
        qDebug() << "Le serveur a été démarré sur le port " << QString::number(serveur->serverPort())  << "Des clients peuvent maintenant se connecter.";
        connect(serveur, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }

    m_db = new CDatabase();
    m_db->execMain();

    set_channels(m_db->parseChannel());
    set_clients(m_db->parseClient());

}

//Getters
CDatabase * CServer::get_database(){
    return m_db;
}

//Setters
void CServer::set_database(CDatabase * db){
    m_db = db;
}

int CServer::whichClient(QTcpSocket * s){
    //Looking for the client's index with the socket 's'
    for(int i = 0; i < get_clientList().size(); i++){
        if(get_clientList()[i]->get_socket() == s){
            return i;
        }
    }
    //No client find
    return -1;
}

void CServer::onNewConnection()
{
    CClient * newClient = new CClient();
    newClient->set_id(0);
    newClient->set_pseudo("taga");
    newClient->set_socket(serveur->nextPendingConnection());

    addClient(newClient);


    connect(newClient->get_socket(), SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    connect(newClient->get_socket(), SIGNAL(disconnected()), this, SLOT(onDisconnectClient()));

    //Send server object
    CPacket * packet = new CPacket();
    newClient->get_socket()->write(packet->Serialize(this));

}

void CServer::onDisconnectClient()
{
    //Looking for sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
        return;

    //Compare to clients list
    for( int i = 0 ; i  < get_clientList().size(); i++){
        if(get_clientList()[i]->get_socket() == socket){
            free(get_clientList()[i]);
            get_clientList().removeAt(i);
            qDebug() << "Client " << i << " has disconnect" << Qt::endl;
        }
    }
    //send msg to everybody to say someone disconnect (id client)
}

void CServer::onReceiveData(){

    //Process receiving data -
    //Get sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) //Couldn't find sender
          return;
    qDebug() << "Receive msg \n";

    //Get data
    QByteArray *data = new QByteArray();
    data->append(socket->readAll());

    int uid = whichClient(socket);
    if(uid == -1){
        return;
    }

    //Process data
    processIncomingData(get_clientList()[uid], *data);
}


void CServer::sendToChannel(const QString &message, int id_channel)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //preparation of the packet
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    //send msg to all client of the channel
    for( int i = 0; i < get_channelList()[id_channel]->get_clients().size(); i++)
    {
        get_channelList()[id_channel]->get_clients()[i]->get_socket()->write(packet);
    }
}

void CServer::sendToClient(const QString &message, CClient * client)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //preparation of the packet
    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    //send msg to the client
    client->get_socket()->write(packet);
}

void CServer::sendToAll(QByteArray out)
{
    if(get_clientList().isEmpty() == true)
    {
        qDebug() << "there is no client ! ";
    }
    else{
        foreach(CClient * client, get_clientList())
            {
                qDebug() << out;
                client->get_socket()->write(out);
                client->get_socket()->flush();

                qDebug() << "packet send to " << client->get_pseudo();
            }
    }
}

void CServer::sendToClient(QByteArray out, CClient * client){
    client->get_socket()->write(out);
    client->get_socket()->waitForBytesWritten();
}


void CServer::SendObjectsToClient()
{
    //TODO
    CPacket * packet = new CPacket("0","1");
    packet->Serialize(this);
    sendToAll(packet->GetByteArray());
}

void CServer::processIncomingData(CClient *sender, QByteArray data){    //Treats data received
        if(!sender)
            return;

        CPacket* packet = new CPacket(data, sender);
        packet->Deserialize();

        //Récupération du type
        switch (packet->GetType().toInt()) {
        case 0: //SERV
            switch (packet->GetAction().toInt())
            {
            case 0:
            {   
                //SERV CONNECT
                //Check Auth -

                //If Auth is Ok
                CClient * client = new CClient();
                client = packet->Deserialize_newClient();
                addClient(client);

                if(get_clientList()[client->get_id()]->get_isOnline() == true)
                    get_clientList()[client->get_id()]->set_isOnline(false);

                //Update info -
                CPacket ans("0","0");
                ans.Serialize_newClient(client);
                sendToAll(ans.GetByteArray());

                //TODO -

                break;
            }
            case 1:
            {
                //SERV DISCONNECT
                //Update online users
                CClient * client = packet->Deserialize_newClient();

                for(int i = 0; i < get_clientList().size(); i++){
                    if(get_clientList()[i]->get_id() == client->get_id()){
                        get_clientList()[client->get_id()]->set_isOnline(false);
                    }
                }
                //User is not online anymore

                 CPacket ans("0","1");
                 ans.Serialize();
                 ans.Serialize_newClient(client);

                 //Send Update
                 sendToAll(packet->GetByteArray());

                 free(client);


                 //TODO -



                break;
            }
            case 2:
            {
                //PSEUDO UPDATE

                //Modify pseudo
                get_clientList()[sender->get_id()]->set_pseudo(packet->GetData()["pseudo"].toString());

                //Send update to the other client

                QJsonObject infos;
                infos.insert("id", sender->get_id());
                infos.insert("pseudo", packet->GetData()["pseudo"]);
                QJsonDocument doc(infos);
                QByteArray ret;                 //Réponse
                //ret.push_back(packet->GetType());
                //ret.push_back(packet->GetAction());
                sendToAll(ret);
                free(packet);
                break;
            }
            case 3:
            {
                //BIO UPDATE
                //Comme au dessus
                get_clientList()[sender->get_id()]->set_pseudo(packet->GetData()["description"].toString());
                QJsonObject infos;
                infos.insert("id", sender->get_id());
                infos.insert("description", packet->GetData()["description"]);
                QJsonDocument doc(infos);
                QByteArray ret;
                //ret.push_back(packet->GetType());
                //ret.push_back(packet->GetAction());
                sendToAll(ret);
                break;
            }
            case 4:
                //BAN USER
                break;
            case 5:
                //BAN IP
                //Rajouter système de gestion du temps
            case 6: {
                //Kick user
                /*t_user_chan[packet->GetData()["id"].toInt()][m_clients[packet->GetData()["id"].toInt()]->GetUserID()] = -1;
                QJsonObject info;
                info.insert("reason", packet->GetData()["reason"]);
                CPacket ret;
                ret.SetType(0);
                ret.SetAction(6);
                m_clients[packet->GetData()["id"].toInt()]->get_socket()->write(ret.Serialize());
                for(int i = 0 ; i < m_clients.length() ; i++)
                    if(m_clients[i]->GetUserID() == packet->GetData()["id"].toInt())
                    {
                        m_clients.removeAt(i);
                    }
                sender->get_socket()->close();
                */
                break;
            }
            default:
                qDebug() << "Error invalid action" << Qt::endl;
            }
            break;

        case 1: //CHAN
            switch (packet->GetAction().toInt())
            {
            case 0: {

                //JOIN CHANNEL
                packet->Deserialize_ID();

                CClient * client = get_clientById(packet->get_IdClient());
                CChannel * channel = get_channelById(packet->get_IdChannel());


                if(channel && client){
                    client->set_idChannel(channel->get_id());
                    channel->addUser(client);
                }

                CPacket ans("1","0");
                ans.Serialize();
                ans.Serialize_ID(channel->get_id(),client->get_id());

                sendToAll(ans.GetByteArray());

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

                CPacket ans("1","1");
                ans.Serialize();
                ans.Serialize_ID(channel->get_id(),client->get_id());

                sendToAll(ans.GetByteArray());

                break;
            }
            case 5: {
                //Create chan voc
                CChannel * newChan = new CChannel(packet->GetData()["name"].toString(), get_channelList().size());
                addChannel(newChan);
                break;
            }
            case 6: {
                //Delete chan voc
                for(int i = 0 ; i < get_channelList().size() ; i++)
                    if(get_channelList()[i]->get_id() == packet->GetData()["id"].toInt())
                        DelChannel(i);
                break;
            }
            case 7: {
                //Rename chan voc
                for(int i = 0 ; i < get_channelList().size() ; i++)
                    if(get_channelList()[i]->get_id() == packet->GetData()["id"].toInt())
                        get_channelList()[i]->set_name(packet->GetData()["name"].toString());
                break;
            }
            case 8: {
                //Modif max user (voc)
                for(int i = 0 ; i < get_channelList().size() ; i++)
                    if(get_channelList()[i]->get_id() == packet->GetData()["id"].toInt())
                        get_channelList()[i]->set_maxUsers(packet->GetData()["maxuser"].toInt());
                break;
            }
            case 9: {
                //kick user voc
                /*if(t_user_right[sender->GetUserID()][2])    //Si l'utilisateur a le droit de kick
                {
                    m_clients[packet->GetData()["id"].toInt()]->get_socket()->close();  //On ferme la connection
                    m_clients.removeAt(packet->GetData()["id"].toInt());        //On met à jour la liste des utilisateur
                    QByteArray ret;
                    ret.push_back(packet->GetType());
                    ret.push_back(packet->GetAction());
                    ret.push_back(data);
                    sendToAll(ret);
                }*/
                break;
            }
            case 10: {
                //Mute user voc (server side)
                //m_clients[packet->GetData()["id"].toInt()]->SetMute(!m_clients[packet->GetData()["id"].toInt()]->GetMuted());
                //CPacket res;
                //res.SetType(0);
                //res.SetAction(10);

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
                get_clientList()[packet->GetData()["id"].toInt()]->set_pseudo(packet->GetData()["pseudo"].toString());
                //sendToAll(packet->Serialize());
                break;
            case 5:
                //Change right
                break;
            default:
                qDebug() << "Error invalid action" << Qt::endl;
            }
            break;
        default:
            qDebug() << "Error invalid type" << Qt::endl;
        }
        return;
 }



QByteArray CServer::Serialize(){

    QJsonObject obj;

    QJsonArray cArray, sArray;

    QJsonObject mainObj;

    mainObj.insert("type", "-1");
    mainObj.insert("action", "-1");

    obj["mainObj"] = mainObj;


    foreach(CChannel * c, get_channelList()){
        cArray.append(c->serializeToObj());
    }
    foreach(CClient * c, get_clientList()){
       sArray.append(c->serializeToObj());
    }
    obj["channels"] = cArray;
    obj["clients"] = sArray;



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



























