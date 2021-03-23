#include "Server/cserver.h"
#include "src/includes/cdatabase.h"
#include "Server/sha256.h"

#include <QDebug>

CServer::CServer()
{
    qDebug() << "Welcome to Lockvox Server !" << Qt::endl;
    QDir test;
    if(!test.exists("storage/log"))
    {
        if(!test.mkpath("storage/log"))
        {
            qDebug() << "[Log error] Can't create log directory" << Qt::endl;
        }
    }

    log_level = 0;
    current = QDateTime::currentDateTime();
    QString path = "storage/log/Server_log_" +  current.toString("dd_MMMM_yyyy_hh_mm_ss") + ".txt";
    log_file.setFileName(path);


    if(!log_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "[Log Error] Can't create log file" << Qt::endl;
    }
    else
    {
        log.setDevice(&log_file);
        writeToLog("Starting server...", 0);

        // Gestion du serveur TCP
        serveur = new QTcpServer(this);
        if (!serveur->listen(QHostAddress::Any, 50885)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50885
        {
            // Si le serveur n'a pas été démarré correctement
            writeToLog(serveur->errorString(), 2);
        }
        else
        {
            // Si le serveur a été démarré correctement
            writeToLog("Running on port :" + QString::number(serveur->serverPort()), 0);
            connect(serveur, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
        }

        m_db = new CDatabase();
        set_channels(m_db->parseChannel());
        set_clients(m_db->parseClient());
    }
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

            writeToLog("User [" + get_clientList()[i]->get_uuid().toString() + "(" + get_clientList()[i]->get_pseudo() + ")] disconnected", 0);

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
        writeToLog("Received data from unknown client", 1);
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
    CPacket tmp(*data, get_clientList()[uid]); //Check if valid packet, if not, may be a splitted packet
    if(tmp.GetType() == NULL | tmp.GetAction() == NULL)
    {
        QByteArray * buffer = get_clientBuffer(uid);
        if(data->contains("\"mainObj\": {"))
        {
            //New packet
            if(!buffer->isEmpty())
            {
                writeToLog("[" + get_clientList()[uid]->get_uuid().toString() + "(" + get_clientList()[uid]->get_pseudo() +
                           ")] New request held by multiple packet arrived while user buffer isn't empty, clearing it",1);
                writeToLog("A packet and therefore a request must have been lost nor a bad packet was received before",1);
                buffer->clear();
            }
            buffer->append(*data);
        }
        else
        {
            if(buffer->isEmpty())
            {
                //That's meen it's a bad packet, report to log
                writeToLog("Unable to deserialize received packet :\n" + *data + "\nRequest Aborted", 1);
            }
            else
            {
                buffer->append(*data);
                CPacket tmp1(*buffer, get_clientList()[uid]);

                //We check if the packet is complete, otherwise we wait for the buffer to fill up
                if(tmp1.GetType() != NULL & tmp1.GetAction() != NULL)
                {
                    processIncomingData(get_clientList()[uid], *buffer);
                    buffer->clear();
                }
            }
        }
    }
    else
    {
        processIncomingData(get_clientList()[uid], *data);
    }
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

void CServer::sendToAllExecptClient(QByteArray out, CClient *client)
{
    foreach(CClient * c, m_clients){
        if(c->get_uuid() == client->get_uuid())
            break;
        c->get_socket()->write(out);
        c->get_socket()->waitForBytesWritten();
    }
}

void CServer::AddBannedUser(CClient * client)
{
    m_banned_users.push_back(client);
    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] has been banned", 0);
}

void CServer::RemoveBannedUser(CClient* client)
{
    if(!m_banned_users.removeOne(client))
    {
        writeToLog("Trying to unban user [" + client->get_uuid().toString() + "] who is not banned", 1);
        return;
    }
    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] is not banned anymore", 0);
}

QList<CClient*> CServer::GetBannedUserList()
{
    return m_banned_users;
}

void CServer::processIncomingData(CClient *sender, QByteArray data) //Process received data
{
        if(!sender)
        {
            writeToLog("Received data from unknown client", 1);
            return;
        }

        CPacket* packet = new CPacket(data, sender);

        if(packet->GetAction() == NULL | packet->GetType() == NULL)
        {
            writeToLog("Unable to deserialize received packet :\n" + packet->GetByteArray() + "\nRequest Aborted", 1);
            return;
        }

        if(packet->GetAction().toInt() == -1 | packet->GetType().toInt() == -1)
        {
            qDebug() << "Send server info to client\n";
            CPacket * objServer = new CPacket("-1","-1");
            objServer->Serialize(this);
            sender->get_socket()->write(objServer->GetByteArray());
            sender->get_socket()->waitForBytesWritten();
            return;
        }

        //Récupération du type
        switch (packet->GetType().toInt())
        {
            case 0: //SERV
            {
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

                       if(get_clientById(client->get_uuid())->get_isOnline() == false)
                       {
                           get_clientById(client->get_uuid())->set_isOnline(true);
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
                            if(get_clientList()[i]->get_uuid() == client->get_uuid())
                            {
                                get_clientById(client->get_uuid())->set_isOnline(false);
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

                        writeToLog("User [" + sender->get_uuid().toString() + "(" + sender->get_pseudo() + ")] change username to [" + client->get_pseudo() + "]", 0);
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

                        foreach(CClient * c , m_clients)
                        {
                            if(client->get_uuid() == c->get_uuid())
                            {
                                writeToLog("User [" + c->get_uuid().toString() + "(" + c->get_pseudo() + ")] Has been kicked from server" ,0);
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
                        CPacket* ans = new CPacket("0", "7");
                        CPacket newUser("0","0");
                        //Hachage du password
                        std::string hashed = info[1].toStdString();
                        hashed = sha256(hashed);
                        bool valid = false;

                        QString * err = new QString;
                        CClient * tmp_client = m_db->parseClient(info[0].toStdString(), err);
                        if(tmp_client) //Si l'utilisateur existe
                        {
                            QString * err1 = new QString;
                            if(hashed == m_db->getHash(info[0].toStdString(), err1))  //Si le mdp correspond à l'utilisateur
                            {
                                foreach(CClient *c, m_clients) //Vérification de connexion déjà existante
                                {
                                    if(tmp_client->get_uuid() == c->get_uuid() && c->get_isAuthenticate()) //utilisateur déjà co
                                    {
                                        ans->Serialize_auth(NULL, 2);
                                        writeToLog("User [" + c->get_uuid().toString() + "(" + c->get_pseudo() + ")] Already connected", 1);
                                    }
                                    if(tmp_client->get_uuid() == c->get_uuid() && !c->get_isAuthenticate()) //Si c'est valide
                                    {

                                        writeToLog("User [" + c->get_uuid().toString()  + "(" + c->get_pseudo() + ")] connected from [" + sender->get_socket()->peerAddress().toString() + "]",0);
                                        //mettre l'utilisateur authentifié
                                        c->set_isOnline(true);
                                        c->set_isAuthenticate(true);
                                        c->set_socket(sender->get_socket());
                                        //Lui envoyer ses infos
                                        ans->Serialize_auth(c, 0);


                                        newUser.Serialize_newClient(c);

                                        valid = true;
                                    }
                                }
                                delete(err1);
                            }
                            else    //Bad password
                            {
                                if(hashed == "false")
                                {
                                    ans->Serialize_auth(NULL, 1);
                                    writeToLog("Error with database when fetching password hash",2);
                                    writeToLog(*err1, 3);
                                    delete(err1);
                                }
                                else
                                {
                                    writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Bad password for [" + tmp_client->get_uuid().toString()  + "(" + tmp_client->get_pseudo() + ")]", 1);
                                    ans->Serialize_auth(NULL, 3);
                                }
                            }
                            delete(err);
                        }
                        else
                        {
                            if(*err == "no_client")
                            {
                                ans->Serialize_auth(NULL, 1);
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Trying to connect to a non-existing account", 1);
                            }
                            else
                            {
                                ans->Serialize_auth(NULL, 1);
                                writeToLog("Error with database when fetching client",2);
                                writeToLog(*err,3);
                                delete(err);
                            }
                        }

                        //qDebug() << "Send Auth answer to client\n";
                        sender->get_socket()->write(ans->GetByteArray());   //On lui envoie ses info
                        sender->get_socket()->waitForBytesWritten();

                        //If authentification suceed - Send Server Object to the client
                        if(valid)
                        {
                            /*qDebug() << "Send server info to client\n";
                            CPacket * objServer = new CPacket();
                            sender->get_socket()->write(objServer->Serialize(this));
                            sender->get_socket()->waitForBytesWritten();*/
                            //sendToAllExecptClient(newUser.GetByteArray(), sender);
                        }


                        break;
                    }

                    case 8 :
                    {
                        //Get Information from request
                        packet->Deserialize_regReq();

                        //Check informations
                        if(packet->get_RegisterInfo().email.isNull() | packet->get_RegisterInfo().password.isNull() | packet->get_RegisterInfo().name.isNull())
                        {
                            writeToLog("Received incomplete register information from [" + sender->get_socket()->peerAddress().toString() + "]", 1);
                            //Send error to client
                            //TODO
                            break;
                        }

                        QUuid uuid = QUuid::createUuid();

                        QString error = m_db->newUser(uuid.toString(QUuid::WithoutBraces).toStdString(), packet->get_RegisterInfo().name.toStdString(), packet->get_RegisterInfo().email.toStdString(),packet->get_RegisterInfo().password.toStdString());

                        if(error == "mailerror")
                        {
                            //Mail already exist
                            writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Trying to create account with existing mail address" ,1);
                            CPacket ans("0", "8");
                            ans.Serialize_regAns(0);

                            //Send register answer
                            sender->get_socket()->write(ans.GetByteArray());
                            sender->get_socket()->waitForBytesWritten();
                            break;
                        }
                        else
                        {
                            if(error != "success")
                            {
                                writeToLog("Error in creating new user", 3);
                                writeToLog(error,3);
                                //Send error to client
                                //TODO
                                break;
                            }
                            else
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Created new account [" + uuid.toString() + "(" + packet->get_RegisterInfo().name + ")] Successfully", 0);
                                CClient * client = new CClient(uuid, packet->get_RegisterInfo().name, sender->get_socket(), -1, true, "" );
                                addClient(client);

                                //Send answer to the client
                                CPacket ans("0", "8");
                                ans.Serialize_regAns(1);

                                //Serialize here the client with the corresponding information
                                ans.Serialize_myClient(client);

                                //Send register answer
                                sender->get_socket()->write(ans.GetByteArray());
                                sender->get_socket()->waitForBytesWritten();

                                sender->get_socket()->write(Serialize());
                                sender->get_socket()->waitForBytesWritten();

                                CPacket newUser("0","0");
                                newUser.Serialize_newClient(client);
                                sendToAll(newUser.GetByteArray());
                            }
                        }
                        break;
                    }

                    default:
                    {
                        writeToLog("Received invalid server action :\n" + packet->GetByteArray() +"\nIgnored", 1);
                        break;
                    }
                }
                break;
            }

            case 1: //CHAN
            {
                switch (packet->GetAction().toInt())
                {
                    case 0:
                    {
                        //JOIN CHANNEL
                        packet->Deserialize_ID();
                        QUuid tmp = packet->get_IdClient();
                        CClient * client = get_clientById(tmp);
                        CChannel * channel = get_channelById(packet->get_IdChannel());

                        if(channel && client)
                        {
                            client->set_idChannel(channel->get_id());
                            channel->addUser(client);
                            CPacket ans("1","0");
                            ans.Serialize();
                            ans.Serialize_ID(channel->get_id(),client->get_uuid());
                            sendToAll(ans.GetByteArray());
                            writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] has joined channel id ["+ QString::number(packet->get_IdChannel()) + "]", 0);
                        }
                        else
                        {
                            if(!client)
                            {
                                writeToLog("A non-existing client is trying to join a channel ! packet UUID [" + tmp.toString() + "]", 2);
                                //Send error packet
                                //TODO
                            }
                            if(!channel)
                            {
                                writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] is trying to join a non-existing channel id [" + QString::number(packet->get_IdChannel()) + "]", 1);
                                //Send error packet
                                //TODO
                            }
                        }
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
                            channel->delUser(client->get_uuid());
                            CPacket ans("1","1");
                            ans.Serialize();
                            ans.Serialize_ID(channel->get_id(),client->get_uuid());
                            sendToAll(ans.GetByteArray());
                        }
                        else
                        {
                            if(!client)
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Try to exit a channel, but Client isn't in the list ???",2);
                                break;
                            }
                            if(!channel)
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Try to exit a non-existing channel ???",2);
                                break;
                            }
                        }
                        break;
                    }

                    case 2:
                    {
                        //Text channel message
                        CMessage msg = packet->Deserialize_Message();

                        //Verify if it's a non private message
                        if(msg.get_isPrivate() != false)
                        {
                            writeToLog("[" + sender->get_socket()->peerAddress().toString() + "Received a private message for a text channel ? Ignored.", 1);
                            //Send error packet to client
                            //TODO
                            break;
                        }
                        msg.toXML();
                        string filename = sha256(msg.toString().toStdString());

                        QDir test;
                        QString path = "storage/public/" +msg.get_to();

                        if(!test.exists(path))
                        {
                            if(!test.mkpath(path))
                            {
                                writeToLog("Can't create path [" + path + "]",2);
                                //Send error to client
                                //TODO
                                break;
                            }
                        }
                        path = "storage/public/" + msg.get_to() + "/" + QString::fromStdString(filename) + ".xml";
                        if(QFile::exists(path))
                        {
                            writeToLog("[" + sender->get_uuid().toString() + "(" + sender->get_pseudo() + ")] is spamming copy paste message in channel [" + msg.get_to() + "]",1);
                            //Send error packet to client
                            //TODO
                            break;
                        }

                        QFile file(path);

                        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            writeToLog("Can't save received message from [" + msg.get_from() + "] to [" + msg.get_to() + "]",2);
                            //Send error packet to client
                            //TODO
                            break;
                        }

                        QTextStream stream(&file);
                        stream << msg.toString();
                        file.close();

                        path = "storage/public/" +  msg.get_to()  + "/" + "index.json";

                        if(QFile::exists(path))
                        {
                            QList<QString> filename_list;
                            filename_list = readChannelIndex(path);
                            if(filename_list.isEmpty())
                            {
                                writeToLog("Error while reading channel index for channel [" + msg.get_to() + "]",2);
                                //Send error packet to client
                                //TODO
                                break;
                            }
                            else
                            {
                                if(filename_list.first() == "no_index")
                                {
                                    writeToLog("No index in index.json from channel [" + msg.get_to() + "] Try to create one",1);
                                    if(!createChannelIndex(filename,path))
                                    {
                                        writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",2);
                                        //Send error packet to  client
                                        //TODO
                                        break;
                                    }
                                    else
                                    {
                                        writeToLog("Index created successfully for channel [" + msg.get_to() + "]",0);
                                    }
                                }
                                else
                                {
                                    filename_list.append(QString::fromStdString(filename));
                                    if(!insertChannelIndex(path,filename_list))
                                    {
                                        writeToLog("Error while inserting in channel index for channel [" + msg.get_to() + "]",2);
                                        //Send error packet to client
                                        //TODO
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(!createChannelIndex(filename,path))
                            {
                                writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",2);
                                //Send error packet to  client
                                //TODO
                                break;
                            }
                        }
                        //Send to all
                        //TODO
                        break;
                    }

                    case 3:
                    {
                        //Request message list
                        QList<QString> info = packet->deserialize_messageRequest();
                        QList<CMessage> messages_list = createMessageList(info.at(1), false, info.at(2).toInt(), sender->get_uuid(), info.at(3).toInt());

                        if(messages_list.last().get_from() == "allIsSync")
                        {
                            //Tell to client there is no older message
                            //TODO
                            break;
                        }
                        if(messages_list.last().get_from() == "no_index")
                        {
                            //Tell the client there is no message saved
                            //TODO
                            break;
                        }

                        CPacket reqAns("1","2");

                        reqAns.Serialize_MessageList(messages_list);
                        sendToClient(reqAns.GetByteArray(), sender);
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
                    {
                        writeToLog("Received invalid channel action :\n" + packet->GetByteArray() +"\nIgnored", 1);
                        break;
                    }
                }
                break;
            }

            case 2: //USER
            {
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

                    case 6:
                    {
                        //Request message list
                        QList<QString> info = packet->deserialize_messageRequest();
                        QList<CMessage> messages_list = createMessageList(info.at(1), true, info.at(2).toInt(), sender->get_uuid(), info.at(3).toInt());

                        if(messages_list.last().get_from() == "allIsSync")
                        {
                            //Tell to client there is no older message
                            //TODO
                            break;
                        }
                        if(messages_list.last().get_from() == "no_index")
                        {
                            //Tell the client there is no message saved
                            //TODO
                            break;
                        }

                        CPacket reqAns("2","6");

                        reqAns.Serialize_MessageList(messages_list);
                        sendToClient(reqAns.GetByteArray(), sender);
                        break;
                    }

                    default:
                    {
                        writeToLog("Received invalid user action :\n" + packet->GetByteArray() +"\nIgnored", 1);
                        break;
                    }
                }
                break;
            }

            default:
            {
                writeToLog("Received invalid request type :\n" + packet->GetByteArray() +"\nIgnored", 1);
                break;
            }
        }
        return;
 }

QByteArray CServer::Serialize()
{
    QByteArray* ba = new QByteArray();
    return *ba;

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
    return jsonDoc.toJson();
}

void CServer::DeserializeChannels(QByteArray in)
{
        //Deserialize byte array into a json document
        QJsonDocument jsonDoc = QJsonDocument::fromJson(in);

        if(jsonDoc.isNull())
        {
            writeToLog("JSON doc is invalid in channel deserialization",1);
        }

        QJsonArray jsonArray = jsonDoc.array();

        //Get each element of the array
        foreach( const QJsonValue & value, jsonArray)
        {
            //Convert it to an json object then to a channel
            QJsonObject obj = value.toObject();
            CChannel * newChannel = deserializeToChannel(obj);

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

            //if the channel doesnt exist.
            if(get_channelList().isEmpty() || exist == false)
            {
                writeToLog("Requesting a non-exisiting channel.",1);
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
            if(c->get_uuid() == newClient->get_uuid())
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

// ///////////////////////////////////////////////////////// //
//     Create an index.json for an empty channel and add     //
//               the first message of it                     //
//         filename should be the exacte file name           //
//                  of the message file                      //
//         path_to_index should be something like :          //
//         storage/[public|private]/[id]/index.json          //
// ///////////////////////////////////////////////////////// //
bool CServer::createChannelIndex(string filename, QString path_to_index)
{
    QFile index(path_to_index);
    if(!index.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        writeToLog("Can't open index of channel",2);
        return false;
    }

    QJsonArray array;
    QJsonObject obj;
    obj.insert("id","0");
    obj.insert("filename",QString::fromStdString(filename));
    array.append(obj);

    QJsonObject main;
    main["index"] = array;

    QJsonDocument jsonIndex;
    jsonIndex.setObject(main);

    QTextStream streamIndex(&index);
    streamIndex << jsonIndex.toJson();
    index.close();
    return true;
}

// ///////////////////////////////////////////////////////// //
//      returns the list of names of all messages files      //
//             contained in the given index                  //
//         path_to_index should be something like :          //
//         storage/[public|private]/[id]/index.json          //
// ///////////////////////////////////////////////////////// //
QList<QString> CServer::readChannelIndex(QString path_to_index)
{
    QFile indexJson(path_to_index);
    QList<QString> null;

    if(!indexJson.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        writeToLog("Can't open index of channel",2);
        return null;
    }

    QByteArray json = indexJson.readAll();
    indexJson.close();
    QJsonDocument jsonIndex = QJsonDocument::fromJson(json);
    QJsonObject jsonObj = jsonIndex.object();

    if(jsonObj.contains("index"))
    {
        QJsonArray array = jsonObj.value("index").toArray();
        int index = 0;
        QList<QString> filename_list;

        while(!array.isEmpty())
        {
            QJsonObject tmp = array.first().toObject();
            if(tmp.value("id").toInt() == index)
            {
                filename_list.append(tmp.value("filename").toString());
            }
            else
            {
                writeToLog("Error in index.json not containing correct message index",2);
                return null;
            }
            array.removeFirst();
            index++;
        }
        return filename_list;
    }
    else
    {
        writeToLog(("Error while reading index.json, no field \"index\" found"), 2);
        return null;
    }
}

// ///////////////////////////////////////////////////////// //
//     Update index.json when inserting new message to it    //
//         path_to_index should be something like :          //
//         storage/[public|private]/[id]/index.json          //
//    filename_list is the list of all filename of message   //
// stored into for the given channel, can be exctracted with //
//                     the above function                    //
// ///////////////////////////////////////////////////////// //
bool CServer::insertChannelIndex(QString path_to_index, QList<QString> filename_list)
{
    QFile indexJson(path_to_index);
    if(!indexJson.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        writeToLog("Can't open index of channel",2);
        return false;
    }

    QJsonArray array;
    int index = 0;

    while(!filename_list.isEmpty())
    {
        QJsonObject obj;
        obj.insert("id", index);
        obj.insert("filename", filename_list.first());
        array.append(obj);
        filename_list.removeFirst();
        index++;
    }

    QJsonObject main;
    main["index"] = array;

    QJsonDocument jsonIndex;
    jsonIndex.setObject(main);

    QTextStream streamIndex(&indexJson);
    streamIndex << jsonIndex.toJson();
    indexJson.close();
    return true;
}

// ///////////////////////////////////////////////////////// //
// Create a QList of CMessage stored localy using index.json //
//      id refers to id of channel or user for storage       //
//     isPrivate tells if it's a private message or not      //
//  nb_msg_to_sync to tell how much message you want to sync //
//               should be -1 to retrieve all                //
//             sender refere tu uuid of sender,              //
//              used to gather private message               //
//        start_index is used to retrieve older message      //
//       should be 0 if no message has already been sync     //
// ///////////////////////////////////////////////////////// //
QList<CMessage> CServer::createMessageList(QString id, bool isPrivate, int nb_msg_to_sync, QUuid sender, int start_index)
{
    QString default_path, path;
    QList<CMessage> message_list;
    int index;

    if(isPrivate)
    {
        default_path = "storage/private/" + id + "/";
    }
    else
    {
        default_path = "storage/public/" + sender.toString(QUuid::WithoutBraces) + "/" + id + "/";
    }

    QList<QString> filename_list = readChannelIndex(default_path + "index.json");

    if(filename_list.isEmpty() | filename_list.last() == "no_index")
    {
        CMessage noIndex("noIndex","null","null",false);
        message_list.append(noIndex);
        return message_list;
    }

    if(start_index == 0)
    {
        index = nb_msg_to_sync;
    }
    else
    {
        if(start_index >= filename_list.size())
        {
            CMessage allIsSync("allIsSync","null","null",false);
            message_list.append(allIsSync);
            return message_list;
        }
        else
        {
            if((start_index + nb_msg_to_sync) >= filename_list.size())
            {
                index = filename_list.size() - 1;
            }
            else
            {
                index = start_index + nb_msg_to_sync;
            }
        }
    }

    if((nb_msg_to_sync == -1) | (nb_msg_to_sync >= filename_list.size() && start_index == 0))
    {
        foreach(QString filename, filename_list)
        {
            path = default_path + filename + ".xml";
            if(QFile::exists(path))
            {
                QFile file(path);
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    writeToLog("Can't open message file [" + filename + "]",2);
                }
                else
                {
                    CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                    message_list.append(tmp);
                }
            }
            else
            {
                writeToLog("Message [" + filename + "] may have been deleted and not removed from index [" + id + "]",2);
            }
        }
        return message_list;
    }
    else
    {
        for(int i = index; i >= start_index; i--)
        {
            path = default_path + filename_list[filename_list.size() - i] + ".xml";
            if(QFile::exists(path))
            {
                QFile file(path);
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    writeToLog("Can't open message file [" + filename_list[filename_list.size() - i] + "]",2);
                }
                else
                {
                    CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                    message_list.append(tmp);
                }
            }
            else
            {
                writeToLog("Message [" + filename_list[filename_list.size() - i] + "] may have been deleted and not removed from index [" + id + "]",2);
            }
        }
        return message_list;
    }
}

// ///////////////////////////////////////////////////////// //
//       Write the given error into the server log file      //
//            error should be the given error                //
//                  level should be :                        //
//                    0 : Standard                           //
//                  1 : Server Warning                       //
//                   2 : Server Error                        //
//                  3 : Database Error                       //
// ///////////////////////////////////////////////////////// //
void CServer::writeToLog(QString error, int level)
{
    if(level >= log_level)
    {
        QString prefix;
        current = QDateTime::currentDateTime();
        switch (level)
        {
            case 0:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server]";
                break;
            }
            case 1:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server Warning]";
                break;
            }
            case 2:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server Error]";
                break;
            }
            case 3:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Database Error]";
                break;
            }
            default:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Unknown Error]";
                break;
            }
        }
        log << prefix << error << Qt::endl;
    }
}
