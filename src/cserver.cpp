#include "src/includes/cserver.h"

#include <QDebug>
CServer::CServer()
{
    // Gestion du serveur TCP
    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        // Si le serveur n'a pas été démarré correctement
        qDebug()<< "Le serveur n'a pas pu être démarré. Raison :" << serveur->errorString();
    }
    else
    {
        // Si le serveur a été démarré correctement
        qDebug() << "Le serveur a été démarré sur le port " << QString::number(serveur->serverPort())  << "Des clients peuvent maintenant se connecter.";
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    tailleMessage = 0;


    //UDP Server initialization




}

//Getters
CDatabase * CServer::get_database(){
    return m_db;
}

QList<CChannel *> CServer::get_channelList(){
    return m_channels;
}

QList<CClient *> CServer::get_clientList(){
    return m_clients;
}

QList<CChannel> CServer::get_staticChannelList(){
    QList<CChannel> list;
    foreach(CChannel *c, m_channels)
        list.append(*c);
    return list;
}

QList<CClient> CServer::get_staticClientList(){
    QList<CClient> list;
    foreach(CClient *c, m_clients)
        list.append(*c);
    return list;
}

QTcpSocket * CServer::get_socket(){
    return m_socket;
}


//Setters
void CServer::set_database(CDatabase * db){
    m_db = db;
}

void CServer::set_clients(QList<CClient> clients){
    foreach(CClient c, clients)
    {
        m_clients.append(&c);

    }
}

void CServer::set_channels(QList<CChannel> channels){

}

void CServer::set_channel(CChannel channel, int index){
    m_channels[index] = &channel;
}


void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}

void CServer::freeChannels(){
    foreach(CChannel * c, m_channels)
    {
       free(c);
    }
}


void CServer::nouvelleConnexion()
{

    //
    CClient * newClient = new CClient();
    newClient->set_socket(serveur->nextPendingConnection());

    m_clients.append(newClient);

    connect(newClient->get_socket(), SIGNAL(readyRead()), this, SLOT(sReceiveData()));
    connect(newClient->get_socket(), SIGNAL(disconnected()), this, SLOT(deconnexionClient()));

    qDebug() << "New client";
    ///////////////////////////////////////////////////////////////////////////////////////

    QByteArray out = this->Serialize();

    QThread::sleep(5);
    sendToAll(out);
    qDebug() << "packet send";
    ///////////////////////////////////////////////////////////////////////////////////////
}

void CServer::deconnexionClient()
{
    //Looking for sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
        return;

    //Compare to clients list
    for( int i = 0 ; i  < m_clients.size(); i++){
        if(m_clients[i]->get_socket() == socket){
            free(m_clients[i]);
            m_clients.removeAt(i);
            qDebug() << "Client " << i << " has disconnect" << Qt::endl;
        }
    }

    //send msg to everybody to say someone disconnect (id client)

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
    for( int i = 0; i < m_channels[id_channel]->get_clients().size(); i++)
    {
        m_channels[id_channel]->get_clients()[i]->get_socket()->write(packet);
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

void CServer::envoyerATous(const QString &message)
{
    // Préparation du paquet
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0; // On écrit 0 au début du paquet pour réserver la place pour écrire la taille
    out << message; // On ajoute le message à la suite
    out.device()->seek(0); // On se replace au début du paquet
    out << (quint16) (paquet.size() - sizeof(quint16)); // On écrase le 0 qu'on avait réservé par la longueur du message


    // Envoi du paquet préparé à tous les clients connectés au serveur
    for (int i = 0; i < clients.size(); i++)
    {
        clients[i]->write(paquet);
    }
}


void CServer::sendToAll(QByteArray out)
{

    if(m_clients.isEmpty() == true)
    {
        qDebug() << "there is no client ! ";
    }
    else{
        foreach(CClient * client, m_clients)
            {
                qDebug() << out;
                client->get_socket()->write(out);
                client->get_socket()->flush();

                qDebug() << "oula3";
                qDebug() << "packet send to " << client->get_pseudo();
            }
    }
}

void CServer::addChannel(CChannel * tmp){
    m_channels.push_back(tmp);
}

void CServer::addClient(CClient * client){
    m_clients.push_back(client);
}

//param : none
//Send CServer, CChannel and CClient to clients.
//A flag is used for each type of object
//1: CServer  / 2: CChannel / 3: CClient
void CServer::SendObjectsToClient()
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    //First - Send Server Object -
    //Flag 1 - Send Server







    //Second - Send Channels
    out << "2";                                 //Flag 2 - Send Channel

    //Third - Send clients

    out << 3;                                   //Flag 3 - Send Client

}

//Comment récupérer les données ??
void CServer::sReceiveData(CClient sender, QByteArray data){
    sender.set_pseudo("abc");
    data = {0};
    switch (data.at(0)) {
        case 0: //SERV
        switch (data.at(1))
        {
            case 0:
                //SERV CONNECT
                //SEND INFO SERVER
                break;
            case 1:
                //SERV DISCONNECT
                //Update online users
                break;
            case 2:
                //PSEUDO UPDATE
                //Update en ram
                break;
            case 3:
                //BIO UPDATE
                //Comme au dessus
            case 4:
                //BAN USER
                break;
            case 5:
                //BAN IP
                //Rajouter système de gestion du temps
            case 6:
                //Kick user
                //
                break;
        default:
            qDebug() << "Error invalid action" << Qt::endl;
        }
        break;

        case 1: //CHAN
        switch (data.at(1))
        {
        case 0:
            //CONNECT CHAN
            break;
        case 1:
            //DISCONNECT CHAN
            break;
        case 2:
            //Envoi message
            //BDD + envoi à tlm
            break;
        case 3:
            //Modif message
            break;
        case 4:
            //Del message
            break;
        case 5:
            //Create chan voc
            break;
        case 6:
            //Delete chan voc
            break;
        case 7:
            //Rename chan voc
            break;
        case 8:
            //Modif max user (voc)
            break;
        case 9:
            //kick user voc
            break;
        case 10:
            //Mute user voc (server side)
            break;
        case 11:
            //Create chan text
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
        switch (data.at(1))
        {
        case 0:
            //Mute (user side)
            break;
        case 1:
            //Add friend
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
            qDebug() << "Error invalid type" << Qt::endl;
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



QList<CChannel*> CServer::GetRightChannels(int p_rightid)
{
    QList<CChannel*> res;
    for(int i = 0; i < m_channels.size(); i++)
        if(t_chan_right[i][p_rightid] == true)
        {
            res.append(m_channels.at(i));
        }
    return res;
}

QList<CClient*> CServer::GetRightClient(int p_rightid)
{
    QList<CClient*> res;
    for(int i =0 ; i < m_clients.size() ; i++)
        if(t_user_right[i][p_rightid] == true)
            res.append(m_clients.at(i));
    return res;
}

QList<crole*> CServer::GetUserRoles(int p_userid)
{
    QList<crole*> res;
    for(int i = 0; i < m_roles.size(); i++)
        if(t_user_right[p_userid][i] == true)
        {
            res.append(m_roles.at(i));
        }
    return res;

}

QList<CChannel*> CServer::GetUserChannels(int p_userid)
{
    QList<CChannel*> res;
    for(int i = 0; i < m_channels.size(); i++)
        if(t_user_chan[p_userid][i] != -1) //si il appartient au chan
        {
            res.append(m_channels.at(i));
        }
    return res;
}

QList<CClient*> CServer::GetChannelClients(int p_chanid, bool p_online)
{
    QList<CClient*> res;
    for(int i = 0 ; i < m_clients.size() ; i++)
        if((p_online && t_user_chan[i][p_chanid] == 1) || (!p_online && (t_user_chan[i][p_chanid] == 0 || t_user_chan[i][p_chanid] == 1))) // Si online seulement les co, sinon tous ceux qui appartiennent
        {
            res.append(m_clients.at(i));
        }
    return res;
}

QList<crole*> CServer::GetChannelRoles(int p_chanid)
{
    QList<crole*> res;
    for(int i = 0; i < m_roles.size() ; i++)
        if(t_chan_right[p_chanid][i] == true)
            res.append(m_roles.at(i));
    return res;
}





















