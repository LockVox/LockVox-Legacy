#include "Server/cserver.h"
#include "src/includes/cdatabase.h"
#include "Server/sha256.h"

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
    set_channels(m_db->parseChannel());
    set_clients(m_db->parseClient());

}

//Getters
CDatabase * CServer::get_database()
{
    return m_db;
}

//Setters
void CServer::set_database(CDatabase * db)
{
    m_db = db;
}

int CServer::whichClient(QTcpSocket * s)
{
    //Looking for the client's index with the socket 's'
    for(int i = 0; i < get_clientList().size(); i++)
    {
        if(get_clientList()[i]->get_socket() == s)
        {
            return i;
        }
    }
    //No client find
    return -1;
}

void CServer::onNewConnection()
{
    CClient * newClient = new CClient();
    newClient->set_socket(serveur->nextPendingConnection());

    connect(newClient->get_socket(), SIGNAL(readyRead()), this, SLOT(onReceiveData()));
    connect(newClient->get_socket(), SIGNAL(disconnected()), this, SLOT(onDisconnectClient()));
}

void CServer::onDisconnectClient()
{
    //Looking for sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
    {
        return;
    }

    //Compare to clients list
    for( int i = 0 ; i  < get_clientList().size(); i++)
    {
        if(get_clientList()[i]->get_socket() == socket)
        {
            get_clientList()[i]->set_isOnline(false);
            get_clientList()[i]->set_isAuthenticate(false);

            //Say to everyone
            CPacket request("0","1");
            request.Serialize_newClient(get_clientList()[i]);
            sendToAll(request.GetByteArray());
        }
    }
    //send msg to everybody to say someone disconnect (id client)
}

void CServer::onReceiveData(){

    //Process receiving data -
    //Get sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0) //Couldn't find sender
    {
          return;
    }

    //Get data
    QByteArray *data = new QByteArray();
    data->append(socket->readAll());

    int uid = whichClient(socket);
    if(uid == -1)
    {
        //Process data
        CClient * c = new CClient();
        c->set_socket(socket);
        processIncomingData(c, *data);
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
      foreach(CClient * client, get_clientList())
      {
            if(client->get_socket() != NULL)
            {
                    client->get_socket()->write(out);
            }
      }
}

void CServer::sendToClient(QByteArray out, CClient * client)
{
    client->get_socket()->write(out);
    client->get_socket()->waitForBytesWritten();
}


void CServer::SendObjectsToClient()
{
    //TODO
    //ça sert a quoi au vue de la fct juste au dessus ?
    CPacket * packet = new CPacket("0","1");
    packet->Serialize(this);
    sendToAll(packet->GetByteArray());
}

void CServer::AddBannedUser(CClient * client)
{
    m_banned_users.push_back(client);
}

void CServer::RemoveBannedUser(CClient* client)
{
   if(!m_banned_users.removeOne(client))
       qDebug() << "User : '" << client->get_pseudo() << "' is not banned" << Qt::endl;
}

QList<CClient*> CServer::GetBannedUserList()
{
    return m_banned_users;
}

void CServer::processIncomingData(CClient *sender, QByteArray data){    //Treats data received
        if(!sender)
            return;

        CPacket* packet = new CPacket(data, sender);
        packet->Deserialize();

        //Récupération du type
        switch (packet->GetType().toInt())
        {
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
                       {
                           get_clientList()[client->get_id()]->set_isOnline(false);
                       }

                       //Update info -
                       CPacket ans("0","0");
                       ans.Serialize_newClient(client);
                       sendToAll(ans.GetByteArray());

                       break;
                    }

                    case 1:
                    {
                        //SERV DISCONNECT
                        //Update online users
                        CClient * client = packet->Deserialize_newClient();

                        for(int i = 0; i < get_clientList().size(); i++)
                        {
                            if(get_clientList()[i]->get_id() == client->get_id())
                            {
                                get_clientList()[client->get_id()]->set_isOnline(false);
                            }
                        }

                        //User is not online anymore
                        CPacket ans("1","0");
                        ans.Serialize_newClient(client);

                        //Send Update
                        sendToAll(packet->GetByteArray());

                       free(client);

                       break;
                   }

                   case 2:
                   {
                        //PSEUDO UPDATE
                        CClient * client = packet->Deserialize_newClient();

                        //Apply changement
                        sender->set_pseudo(client->get_pseudo());

                        //Send update
                        CPacket ans("2","0");
                        ans.Serialize_newClient(sender);
                        sendToAll(ans.GetByteArray());

                        free(client);

                        break;
                    }

                    case 3:
                    {
                        //BIO UPDATE
                        CClient * client = packet->Deserialize_newClient();

                        //Apply changement
                        sender->set_description(client->get_description());

                        //Send update
                        CPacket ans("2","0");
                        ans.Serialize_newClient(sender);
                        sendToAll(ans.GetByteArray());
                        break;
                    }

                    case 4:
                    {
                        //BAN USER
                        //Il faudra check si le sender a l'autorisation
                        //TODO

                        break;
                    }

                    case 5:
                    {
                        //BAN IP
                        //Rajouter système de gestion du temps
                        //TODO
                        break;
                    }

                    case 6:
                    {
                        //Kick user
                        CClient * client = packet->Deserialize_newClient();

                        for(auto c : m_clients)
                        {
                            if(client->get_id() == c->get_id())
                            {
                                sendToAll(packet->GetByteArray());
                                m_clients.removeOne(c);
                                c->get_socket()->close();
                            }
                        }
                        break;
                    }

                    case 7:
                    {
                        //Demande d'authentification
                        QList<QString> info = packet->Deserialize_auth();

                        qDebug() << "Authentification request  - " << info[0] << " " << info[1];
                        CPacket* ans = new CPacket("0", "7");

                        //Hachage du password
                        std::string hashed = info[1].toStdString();
                        hashed = sha256(hashed);
                        bool valid = false;

                        if(hashed == m_db->getHash(info[1].toStdString()))  //Si le mdp correspond à l'utilisateur
                        {
                            CClient * tmp_client = m_db->parseClient(info[0].toStdString());
                            if(tmp_client) //Si l'utilisateur existe pas
                            {
                                for(auto c : m_clients) //Vérification de connexion déjà existante
                                {
                                    if(tmp_client->get_id() == c->get_id() && c->get_isAuthenticate()) //utilisateur déjà co
                                    {
                                        ans->Serialize_auth(NULL, 2);
                                    }
                                    if(tmp_client->get_id() == c->get_id() && !c->get_isAuthenticate()) //Si c'est valide
                                    {
                                        //mettre l'utilisateur authentifié
                                        c->set_isAuthenticate(true);
                                        c->set_socket(sender->get_socket());
                                        //Lui envoyer ses infos
                                        ans->Serialize_auth(c, 0);
                                        valid = true;
                                    }
                                }
                            }
                            else
                            {
                                ans->Serialize_auth(NULL, 1);
                            }
                        }
                        else    //Bad password
                        {
                            ans->Serialize_auth(NULL, 3);
                        }

                        sender->get_socket()->write(ans->GetByteArray());   //On lui envoie ses info
                        sender->get_socket()->waitForBytesWritten();
                        Sleep(100);

                        //If authentification suceed - Send Server Object to the client
                        if(valid)
                        {
                            //CPacket * packet = new CPacket();
                            sender->get_socket()->write(packet->Serialize(this));
                            sender->get_socket()->waitForBytesWritten();
                        }
                        break;
                }

                case 8 :
                {

                }

                default:
                    qDebug() << "Error invalid action" << Qt::endl;
            }
            break;

            case 1: //CHAN
                switch (packet->GetAction().toInt())
                {
                case 0:
                {
                    //JOIN CHANNEL
                    packet->Deserialize_ID();

                    CClient * client = get_clientById(packet->get_IdClient());
                    CChannel * channel = get_channelById(packet->get_IdChannel());


                    if(channel && client)
                    {
                        client->set_idChannel(channel->get_id());
                        channel->addUser(client);
                    }

                    CPacket ans("1","0");
                    ans.Serialize();
                    ans.Serialize_ID(channel->get_id(),client->get_id());

                    sendToAll(ans.GetByteArray());

                    break;
                }

                case 1:
                {
                    //QUIT CHAN
                    packet->Deserialize_ID();

                    CClient * client = get_clientById(packet->get_IdClient());
                    CChannel * channel = get_channelById(packet->get_IdChannel());

                    if(channel && client)
                    {
                        client->set_idChannel(-1);
                        channel->delUser(client->get_id());
                    }

                    CPacket ans("1","1");
                    ans.Serialize();
                    ans.Serialize_ID(channel->get_id(),client->get_id());

                    sendToAll(ans.GetByteArray());

                    break;
                }

                case 5:
                {
                    //Create chan voc
                    CChannel * c = packet->Deserialize_newChannel();
                    addChannel(c);

                    CPacket ans("1","5");
                    ans.Serialize_newChannel(c);
                    sendToAll(ans.GetByteArray());

                    break;
                }

                case 6:
                {
                    //Delete chan voc
                    CChannel * c = packet->Deserialize_newChannel();
                    CChannel * toDelChannel = get_channelById(c->get_id());
                    DelChannel(toDelChannel);


                    CPacket ans("1","6");
                    ans.Serialize_newChannel(c);
                    sendToAll(ans.GetByteArray());

                    free(c);
                    break;
                }

                case 7:
                {
                    //Rename chan voc
                    CChannel * c = packet->Deserialize_newChannel();

                    CChannel * channel = get_channelById(c->get_id());
                    channel->set_name(c->get_name());

                    CPacket ans("1","7");
                    ans.Serialize_newChannel(channel);
                    sendToAll(ans.GetByteArray());
                    free(c);

                    break;
                }

                case 8:
                {
                    //Modif max user (voc)
                    CChannel * c = packet->Deserialize_newChannel();

                    CChannel * channel = get_channelById(c->get_id());
                    channel->set_maxUsers(c->get_maxUsers());

                    CPacket ans("1","8");
                    ans.Serialize_newChannel(channel);
                    sendToAll(ans.GetByteArray());
                    free(c);

                    break;
                }

                case 9:
                {
                    //kick user voc
                    //TODO

                    break;
                }

                case 10:
                {
                    //Mute user voc (server side)
                    //TODO

                    break;
                }

                case 11:
                {
                    //Create chan text --------> Qxmpp
                    //TODO

                    break;
                }

                case 12:
                {
                    //Delete cahn text
                    //TODO

                    break;
                }

                case 13:
                {
                    //Rename chan text
                    //TODO
                    break;
                }

                default:
                    qDebug() << "Error invalid action" << Qt::endl;
            }
            break;

            case 2: //USER
                switch (packet->GetAction().toInt())
                {
                case 0:
                {
                    //Mute (user side) ?????
                    //TODO

                    break;
                }

                case 1:
                {
                    //Add friend --> later
                    //TODO

                    break;
                }

                case 2:
                {
                    //Del friend
                    //TODO

                    break;
                }

                case 3:
                {
                    //Send msg to friend
                    //TODO

                    break;
                }

                case 4:
                {
                    //Modif pseudo (update bdd)
                    //TODO

                    break;
                }

                case 5:
                {
                    //Change right
                    //TODO

                    break;
                }

                default:
                    qDebug() << "Error invalid action" << Qt::endl;
            }
            break;

        default:
            qDebug() << "Error invalid type" << Qt::endl;
        }
        return;
 }

QByteArray CServer::Serialize()
{

    QJsonObject obj;

    QJsonArray cArray, sArray;

    QJsonObject mainObj;

    mainObj.insert("type", "-1");
    mainObj.insert("action", "-1");

    obj["mainObj"] = mainObj;

    foreach(CChannel * c, get_channelList())
    {
        cArray.append(c->serializeToObj());
    }

    foreach(CClient * c, get_clientList())
    {
       sArray.append(c->serializeToObj());
    }

    obj["channels"] = cArray;
    obj["clients"] = sArray;

    QJsonDocument jsonDoc(obj);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();
}

void CServer::Deserialize(QByteArray in)
{
    QJsonDocument jsonDoc = QJsonDocument::fromJson(in);
    QJsonObject obj = jsonDoc.object();

    QJsonArray sArray = obj["clients"].toArray();
    QJsonArray cArray = obj["channels"].toArray();

    deserializeChannel(cArray);
    deserializeClients(sArray);
}


QByteArray CServer::SerializeChannels()
{
    QJsonArray jsonArray;
    foreach(CChannel * c, get_channelList())
    {
        jsonArray.append(c->serializeToObj());
    }

    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();
}

QByteArray CServer::SerializeClients()
{
    QJsonArray jsonArray;
    foreach(CClient * c, get_clientList())
    {
        jsonArray.append(c->serializeToObj());
    }

    QJsonDocument jsonDoc(jsonArray);
    qDebug() << jsonDoc;

    return jsonDoc.toJson();
}

void CServer::DeserializeChannels(QByteArray in)
{
        //Deserialize byte array into a json document
        QJsonDocument jsonDoc = QJsonDocument::fromJson(in);

        if(jsonDoc.isNull())
        {
            qDebug() <<"JSON doc is invalid (null)" << Qt::endl;
        }

        QJsonArray jsonArray = jsonDoc.array();

        //Get each element of the array
        foreach( const QJsonValue & value, jsonArray)
        {
            //Convert it to an json object then to a channel
            QJsonObject obj = value.toObject();
            CChannel * newChannel = deserializeToChannel(obj);
            //qDebug() << "Channel : " << newChannel->get_id()<< newChannel->get_name()<< Qt::endl;

            //check if the channel already exist or not
            bool exist = false;

            //if the channel exist, we reload it with new value
            foreach(CChannel * c, get_channelList())
            {
                if(c->get_id() == newChannel->get_id())
                {
                    exist = true;
                    c->set_all(newChannel);
                }
            }

            //if the channel doesnt exist, we add it to the list of channel
            if(get_channelList().isEmpty() || exist == false)
            {
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

CChannel * CServer::deserializeToChannel(QJsonObject json_obj)
{
    CChannel * channel = new CChannel();

    channel->deserialize(json_obj);

    return channel;
}

CClient * CServer::deserializeToClient(QJsonObject json_obj)
{
    CClient * client = new CClient();
    client->deserialize(json_obj);
    return client;
}

void CServer::deserializeChannel(QJsonArray & json_array)
{

    foreach( const QJsonValue & value, json_array)
    {
        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CChannel * newChannel = deserializeToChannel(obj);

        //check if the channel already exist or not
        bool exist = false;
        foreach(CChannel * c, get_channelList())
        {
            if(c->get_id() == newChannel->get_id())
            {
                 exist = true;
            }
        }

        if(exist == false)
        {
            addChannel(newChannel);
        }
    }
}

void CServer::deserializeClients(QJsonArray & json_array)
{
    foreach( const QJsonValue & value, json_array)
    {
        //Convert it to an json object then to a channel
        QJsonObject obj = value.toObject();
        CClient * newClient = deserializeToClient(obj);

        //check if the channel already exist or not
        bool exist = false;
        foreach(CClient * c, get_clientList())
        {
            if(c->get_id() == newClient->get_id())
            {
                 exist = true; 
            }
        }

        if(exist == false)
        {
            addClient(newClient);
        }
    }
}



























