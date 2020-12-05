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
}
CServer::CServer(int mode)
{


    m_receive_data = new QByteArray();
    switch(mode)
    {
        case 1 :
            qDebug() << "START APPLICATION AS SERVER" << Qt::endl;

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


        break;
        case 2 :
            qDebug() << "START APPLICATION AS CLIENT" << Qt::endl;
            serveur = NULL;
            m_socket = new QTcpSocket();

            m_socket->abort();
            m_socket->connectToHost("127.0.0.1", 50885);

            connect(m_socket, SIGNAL(readyRead()), this, SLOT(cReceiveData()));

    }


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
    //QByteArray out = this->Serialize();
    //QThread::sleep(5);
    //sendToAll(out);
    //qDebug() << "packet send";
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

                qDebug() << "packet send to " << client->get_pseudo();
            }
    }
}

void CServer::sendToServer(QByteArray ba){
    qDebug() << "Data has been send to Server ";
    m_socket->write(ba);
}

void CServer::sendToServer(){

    QByteArray buffer;
    buffer = m_audio_in->get_device()->read(8192);
    qDebug() << "Send to server - " << buffer << Qt::endl;
    m_socket->write(buffer);
    buffer.clear();
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

void CServer::sReceiveData(){


    // On détermine quel client envoie le message (recherche du QTcpSocket du client)
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) // Si par hasard on n'a pas trouvé le client à l'origine du signal, on arrête la méthode
          return;

    qDebug() << "Receive msg \n";

    QByteArray *data = new QByteArray();
    data->append(socket->readAll());

    qDebug() << "Resend data to client";
    socket->write(*data);
}

void CServer::cReceiveData(){
    qDebug() << "Receive data from server" ;

    //Either JSON


    //Either Audio
    QByteArray audioBlock; //holds received audio
    QString controlString; //unused for now, holds received control string
    m_audio_out->writeData(m_socket->readAll());

    //m_receive_data->append(m_socket->readAll());
    //qDebug() << *m_receive_data;










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


























