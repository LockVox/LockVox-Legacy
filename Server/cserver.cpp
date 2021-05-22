#include "Server/includes/cserver.h"
#include "Server/includes/cdatabase.h"
#include "Server/includes/sha256.h"

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
            abort();
        }
    }

    log_level = SERVER;
    current = QDateTime::currentDateTime();
    QString path = "storage/log/Server_log_" +  current.toString("dd_MMMM_yyyy_hh_mm_ss") + ".txt";
    log_file.setFileName(path);


    if(!log_file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "[Log Error] Can't create log file" << Qt::endl;
        abort();
    }
    else
    {
        log.setDevice(&log_file);
    }
}

void CServer::start()
{
    // Gestion du serveur TCP
    writeToLog("Starting server...", SERVER);

    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) //Statuate on server sarting listening for every adresse on port given
    {
        // If server didn't start properly
        writeToLog(serveur->errorString(), SERVER_ERR);
        abort();
    }
    else
    {
        // If server started properly
        writeToLog("Running on port :" + QString::number(serveur->serverPort()), SERVER);
        connect(serveur, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    }

    m_db = new CDatabase();
    m_db->start();

    QString err = m_db->init();

    if(err != "success")
    {
        writeToLog(err,DB_ERR);
        abort();
    }

    set_channels(m_db->parseChannel());
    set_clients(m_db->parseClient());

    foreach(CClient *c, m_clients)
    {
        //For each flient, verify if they have profil pic
        QString path = "storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png";
        if(QFile::exists(path))
        {
            QImage tmp(path);
            c->set_profilePic(tmp);
        }
        else //Give them a random poril pic
        {
            int random = QRandomGenerator::global()->bounded(0,18);
            path = "storage/server/pp/pp" + QString::number(random) + ".png";
            if(QFile::exists(path))
            {
                QDir test;
                QImage tmp(path);
                c->set_profilePic(tmp);
                if(test.exists("storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces)))
                {
                    tmp.save("storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                }
                else
                {
                    test.mkpath("storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces));
                    tmp.save("storage/private/" + c->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                }
            }
        }
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

CClient * CServer::whichClient(QTcpSocket * s)
{
    //Looking for the client with the socket 's'
    foreach(CClient * client, get_clientList())
    {
        if(client->get_socket() == s)
        {
            return client;
        }
    }
    return nullptr;
}

void CServer::onNewConnection()
{
    //qDebug() << "New connection - Socket descriptor: " << socketDescriptor;


    //newClient->get_socket()->setSocketDescriptor(socketDescriptor);
    //QObject::connect(newClient->get_socket(), SIGNAL(encrypted()), this, SLOT(ready()));

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

    CClient * client = whichClient(socket);

    client->set_isOnline(false);
    client->set_isAuthenticate(false);

    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] disconnected", SERVER);

    //Say to everyone
    CPacket request("0","1");
    request.Serialize_newClient(client,false);
    sendToAll(request.GetByteArray());
    return;
}

void CServer::onReceiveData(){

    //Process receiving data -
    //Get sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket == 0) //Couldn't find sender
    {
        writeToLog("Received data from unknown client", SERVER_WARN);
        return;
    }

    //Get data
    QByteArray *data = new QByteArray();
    data->append(socket->readAll());

    CClient * client = whichClient(socket);
    if(client == nullptr)
    {
        //Process data
        CClient * c = new CClient();
        c->set_socket(socket);
        processIncomingData(c, *data);
        return;
    }

    //Process data
    CPacket tmp(*data, client); //Check if valid packet, if not, may be a splitted packet or multiple packet
    bool ifTrueProccess = true;

    if(tmp.GetType() == NULL | tmp.GetAction() == NULL)
    {
        int bracket = 0;
        QTextStream stream(data);
        QString buffer;

        while(!stream.atEnd())
        {
            if(bracket == 0 && !buffer.isEmpty())
            {
                ifTrueProccess = false;
                QByteArray array(buffer.toLocal8Bit());
                if(array !="\n")
                {
                    processIncomingData(client, array);
                }
                buffer.clear();
            }

            QString oneChar = stream.read(1);
            buffer += oneChar;
            if(oneChar == "{")
            {
                bracket++;
            }
            if(oneChar == "}")
            {
                bracket--;
            }
        }

        if(ifTrueProccess)
        {
            QByteArray * buffer = get_clientBuffer(get_clientList().indexOf(client));
            if(data->contains("\"mainObj\": {"))
            {
                //New packet
                if(!buffer->isEmpty())
                {
                    writeToLog("[" + client->get_uuid().toString() + "(" + client->get_pseudo() +
                               ")] New request held by multiple packet arrived while user buffer isn't empty, clearing it",SERVER_WARN);
                    writeToLog("A packet and therefore a request must have been lost nor a bad packet was received before",SERVER_WARN);
                    buffer->clear();
                }
                buffer->append(*data);
            }
            else
            {
                if(buffer->isEmpty())
                {
                    //That's meen it's a bad packet, report to log
                    writeToLog("Unable to deserialize received packet :\n" + *data + "\nRequest Aborted", SERVER_WARN);
                }
                else
                {
                    buffer->append(*data);
                    CPacket tmp1(*buffer, client);

                    //We check if the packet is complete, otherwise we wait for the buffer to fill up
                    if(tmp1.GetType() != NULL & tmp1.GetAction() != NULL)
                    {
                        processIncomingData(client, *buffer);
                        buffer->clear();
                    }
                }
            }
        }
    }
    else
    {
        if(ifTrueProccess & *data != "\n")
        {
            processIncomingData(client, *data);
        }
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
    foreach(CClient * c, m_clients)
    {
        if(c->get_uuid() != client->get_uuid())
        {
            if(c->get_socket() != NULL)
            {
                    c->get_socket()->write(out);
            }
        }
    }
}

void CServer::AddChannel(CChannel *channel)
{
    m_channels.push_back(channel);
    //m_audio->AddSession(*channel);
}

void CServer::AddBannedUser(CClient * client)
{
    m_banned_users.push_back(client);
    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] has been banned", SERVER);
}

void CServer::RemoveBannedUser(CClient* client)
{
    if(!m_banned_users.removeOne(client))
    {
        writeToLog("Trying to unban user [" + client->get_uuid().toString() + "] who is not banned", SERVER_WARN);
        return;
    }
    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] is not banned anymore", SERVER);
}

QList<CClient*> CServer::GetBannedUserList()
{
    return m_banned_users;
}

void CServer::processIncomingData(CClient *sender, QByteArray data) //Process received data
{
        if(!sender) //Should never happen
        {
            writeToLog("Received data from unknown client", SERVER_WARN);
            return;
        }

        CPacket* packet = new CPacket(data, sender);

        if(packet->GetAction() == NULL || packet->GetType() == NULL)//Bad packet
        {
            writeToLog("Unable to deserialize received packet :\n" + packet->GetByteArray() + "\nRequest Aborted", SERVER_WARN);
            return;
        }

        if(packet->GetAction().toInt() == -1 && packet->GetType().toInt() == -1) //Request server object
        {
            qDebug() << "Send server info to client\n";
            CPacket * objServer = new CPacket("-1","-1");
            objServer->Serialize(this);
            sender->get_socket()->write(objServer->GetByteArray());
            sender->get_socket()->waitForBytesWritten();
            return;
        }

        //Get type
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
                       CClient * client = packet->Deserialize_newClient();
                       addClient(client);

                       if(get_clientById(client->get_uuid())->get_isOnline() == false)
                       {
                           get_clientById(client->get_uuid())->set_isOnline(true);
                       }

                       //Update info -
                       CPacket ans("0","0");
                       ans.Serialize_newClient(client,false);
                       sendToAll(ans.GetByteArray());
                       break;
                    }

                    case 1:
                    {
                        //SERV DISCONNECT
                        //Update online users
                        CClient * client = packet->Deserialize_newClient();
                        if(get_clientById(client->get_uuid()) != NULL)
                        {
                            get_clientById(client->get_uuid())->set_isOnline(false);
                        }

                        //User is not online anymore
                        CPacket ans("0","1");
                        ans.Serialize_newClient(client,false);

                        //Send Update
                        sendToAll(packet->GetByteArray());

                        free(client);

                        break;
                    }

                    case 2:
                    {
                        //PSEUDO UPDATE
                        CClient * client = packet->Deserialize_myClient();
                        qDebug() << "Receive description update from " << sender->get_pseudo();

                        writeToLog("User [" + sender->get_uuid().toString() + "(" + sender->get_pseudo() + ")] change username to [" + client->get_pseudo() + "]", SERVER);
                        //Apply changement
                        sender->set_pseudo(client->get_pseudo());

                        QString res = m_db->updateUsername(sender->get_uuid().toString(QUuid::WithoutBraces).toStdString(), sender->get_pseudo().toStdString());
                        //BDD
                        qDebug() << "BDD Update description : " << res << "\n";
                        if(res=="success")
                        {
                            //Send update
                            CPacket ans("0","2");
                            ans.Serialize_newClient(sender,false);
                            sendToAll(ans.GetByteArray());
                        }
                        else
                        {
                            writeToLog(res, DB_ERR);
                            /*
                            CPacket ans("2","0");
                            CClient * errClient = new CClient();
                            errClient->set_description("Error in database");//De la merde à rework
                            ans.Serialize_newClient(errClient);
                            */
                        }

                        free(client);

                        break;
                    }

                    case 3:
                    {
                        //BIO UPDATE
                        CClient * client = packet->Deserialize_myClient();

                        qDebug() << "Receive description update from " << sender->get_pseudo();
                        //Apply changement
                        sender->set_description(client->get_description());

                        //Send update
                        QString res = m_db->updateDescription(sender->get_uuid().toString(QUuid::WithoutBraces).toStdString(), sender->get_description().toStdString());
                        //BDD
                        qDebug() << "BDD Update description : " << res << "\n";
                        if(res=="success")
                        {
                            //Send update
                            CPacket ans("0","3");
                            ans.Serialize_newClient(sender, false);
                            sendToAll(ans.GetByteArray());
                        }
                        else
                        {
                            writeToLog(res, DB_ERR);
                            /*
                            CPacket ans("2","0");
                            CClient * errClient = new CClient();
                            errClient->set_description("Error in database");//De la merde à rework
                            ans.Serialize_newClient(errClient);
                            */
                        }

                        break;
                    }

                    case 4:
                    {
                        //BAN USER
                        //Check if user have right to before...
                        //TODO

                        break;
                    }

                    case 5:
                    {
                        //BAN IP
                        //Maybe add time ?
                        //TODO
                        break;
                    }

                    case 6:
                    {
                        //Kick user
                        CClient * client = packet->Deserialize_newClient();

                        if(get_clientById(client->get_uuid()) != NULL)
                        {
                            /*
                             * We should not use .removeOne but implemente AbstractServer::delClient()
                             * Right must be implemeted and we must verify user have right to kick before implement kick
                            writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] Has been kicked from server" ,SERVER);
                            sendToAll(packet->GetByteArray());
                            m_clients.removeOne(get_clientById(client->get_uuid()));
                            client->get_socket()->close();
                            */
                        }

                        break;
                    }

                    case 7:
                    {
                        //Auth request
                        QList<QString> info = packet->Deserialize_auth();
                        CPacket* ans = new CPacket("0", "7");
                        CPacket newUser("0","0");

                        //Password hash
                        std::string hashed = sha256(info[1].toStdString());
                        bool valid = false;

                        QString * err = new QString;
                        CClient * tmp_client = m_db->parseClient(info[0].toStdString(), err);

                        if(*err == "success")
                        {
                            if(hashed == m_db->getHash(info[0].toStdString(), err))  //If password is correct
                            {
                                foreach(CClient *c, m_clients) //Check if user already connected
                                {
                                    if(tmp_client->get_uuid() == c->get_uuid() && c->get_isAuthenticate()) //For the moment if yes just don't allow second connection
                                    {
                                        writeToLog("User [" + c->get_uuid().toString() + "(" + c->get_pseudo() + ")] Already connected", SERVER_WARN);
                                        ans->Serialize_auth(NULL, 2);                 
                                        sender->get_socket()->waitForBytesWritten();
                                        sender->get_socket()->abort();
                                    }
                                    if(tmp_client->get_uuid() == c->get_uuid() && !c->get_isAuthenticate()) //If not connect him
                                    {
                                        writeToLog("User [" + c->get_uuid().toString()  + "(" + c->get_pseudo() + ")] connected from [" + sender->get_socket()->peerAddress().toString() + "]",SERVER);

                                        c->set_isOnline(true);
                                        c->set_isAuthenticate(true);
                                        c->set_socket(sender->get_socket());

                                        //Sending client info
                                        ans->Serialize_auth(c, 0);
                                        newUser.Serialize_newClient(c,false);
                                        valid = true;
                                    }
                                }
                                delete(err);
                            }
                            else
                            {
                                if(hashed == "false") //DB Error
                                {
                                    writeToLog("Error with database when fetching password hash",SERVER_ERR);
                                    writeToLog(*err, DB_ERR);
                                    ans->Serialize_auth(NULL, 1);     
                                    sender->get_socket()->waitForBytesWritten();
                                    //sender->get_socket()->abort();
                                    delete(err);
                                }
                                else //Bad password
                                {
                                    writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Bad password for [" + tmp_client->get_uuid().toString()  + "(" + tmp_client->get_pseudo() + ")]", SERVER_WARN);
                                    ans->Serialize_auth(NULL, 3);
                                    sender->get_socket()->waitForBytesWritten();
                                    //sender->get_socket()->abort();
                                    delete(err);
                                }
                            }
                        }
                        else
                        {
                            if(*err == "no_client")
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Trying to connect to a non-existing account", SERVER_WARN);
                                ans->Serialize_auth(NULL, 1);
                                sender->get_socket()->waitForBytesWritten();
                                //sender->get_socket()->abort();
                                delete(err);
                            }
                            else
                            {
                                writeToLog("Error with database when fetching client",SERVER_ERR);
                                writeToLog(*err,DB_ERR);
                                ans->Serialize_auth(NULL, 1);
                                sender->get_socket()->waitForBytesWritten();
                                //sender->get_socket()->abort();
                                delete(err);
                            }
                        }

                        //qDebug() << "Send Auth answer to client\n";

                        //We should check if lines behind are usefull...
                        sender->get_socket()->write(ans->GetByteArray());
                        sender->get_socket()->waitForBytesWritten();

                        //If authentification suceed - Send Server Object to the client
                        if(valid)
                        {
                            sendToAllExecptClient(newUser.GetByteArray(), tmp_client);
                        }
                        break;
                    }

                    case 8 :
                    {
                        //Get Information from request
                        //REGISTER
                        packet->Deserialize_regReq();

                        //Check informations
                        if(packet->get_RegisterInfo().email.isNull() | packet->get_RegisterInfo().password.isNull() | packet->get_RegisterInfo().name.isNull())
                        {
                            writeToLog("Received incomplete register information from [" + sender->get_socket()->peerAddress().toString() + "]", SERVER_WARN);
                            //Send error to client
                            //TODO
                            break;
                        }

                        QUuid uuid = QUuid::createUuid();

                        QString error = m_db->newUser(uuid.toString(QUuid::WithoutBraces).toStdString(), packet->get_RegisterInfo().name.toStdString(), packet->get_RegisterInfo().email.toStdString(),packet->get_RegisterInfo().password.toStdString());

                        if(error == "mailerror")
                        {
                            //Mail already exist
                            writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Trying to create account with existing mail address" ,SERVER_WARN);
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
                                writeToLog("Error in creating new user", SERVER_ERR);
                                writeToLog(error,DB_ERR);
                                //Send error to client
                                //TODO
                                break;
                            }
                            else
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Created new account [" + uuid.toString() + "(" + packet->get_RegisterInfo().name + ")] Successfully", SERVER);
                                CClient * client = new CClient(uuid, packet->get_RegisterInfo().name, sender->get_socket(), -1, true, "" );
                                addClient(client);

                                int random = QRandomGenerator::global()->bounded(0,18);
                                QString path = "storage/server/pp/pp" + QString::number(random) + ".png";
                                if(QFile::exists(path))
                                {
                                    QImage tmp(path);
                                    client->set_profilePic(tmp);
                                    QDir test;
                                    if(test.exists("storage/private/" + client->get_uuid().toString(QUuid::WithoutBraces)))
                                    {
                                        tmp.save("storage/private/" + client->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                                    }
                                    else
                                    {
                                        test.mkpath("storage/private/" + client->get_uuid().toString(QUuid::WithoutBraces));
                                        tmp.save("storage/private/" + client->get_uuid().toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                                    }
                                }

                                //Send answer to the client
                                CPacket ans("0", "8");
                                ans.Serialize_regAns(1);

                                //Serialize here the client with the corresponding information
                                ans.Serialize_myClient(client);

                                //Send register answer
                                sender->get_socket()->write(ans.GetByteArray());
                                sender->get_socket()->waitForBytesWritten();

                                //sender->get_socket()->write(Serialize());
                                //sender->get_socket()->waitForBytesWritten();

                                CPacket newUser("0","0");
                                newUser.Serialize_newClient(client,true);
                                sendToAllExecptClient(newUser.GetByteArray(),sender);
                            }
                        }
                        break;
                    }

                    default:
                    {
                        writeToLog("Received invalid server action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
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

                        CClient * client = get_clientById(packet->get_IdClient());
                        CChannel * channel = get_channelById(packet->get_IdChannel());

                        if(channel && client)
                        {
                            client->set_idChannel(channel->get_id());
                            channel->addUser(client);

                            CPacket ans("1","0");

                            ans.Serialize();
                            ans.Serialize_ID(channel->get_id(),client->get_uuid());

                            sendToAll(ans.GetByteArray());

                            writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] has joined channel id ["+ QString::number(packet->get_IdChannel()) + "]", SERVER);
                        }
                        else
                        {
                            if(!client)
                            {
                                writeToLog("A non-existing client is trying to join a channel ! packet UUID [" + packet->get_IdClient().toString() + "]", SERVER_ERR);
                                //Send error packet
                                //TODO
                            }
                            else
                            {
                                if(!channel)
                                {
                                    writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] is trying to join a non-existing channel id [" + QString::number(packet->get_IdChannel()) + "]", SERVER_WARN);
                                    //Send error packet
                                    //TODO
                                }
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

                            writeToLog("User [" + client->get_uuid().toString() + "(" + client->get_pseudo() + ")] has quit channel id ["+ QString::number(packet->get_IdChannel()) + "]", SERVER);
                        }
                        else
                        {
                            if(!client)
                            {
                                writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Try to exit a channel, but Client isn't in the list ???",SERVER_ERR);
                                break;
                            }
                            else
                            {
                                if(!channel)
                                {
                                    writeToLog("[" + sender->get_socket()->peerAddress().toString() + "] Try to exit a non-existing channel ???",SERVER_ERR);
                                    break;
                                }
                            }
                        }
                        break;
                    }

                    case 2:
                    {
                        //Text channel message
                        CMessage msg = packet->Deserialize_Message();

                        if(msg.get_to().toInt() <= -1 || msg.get_to().toInt() > m_channels.size())
                        {
                            writeToLog("[" + sender->get_socket()->peerAddress().toString() + " Received a message for an unknown channel ("+msg.get_to() +") ? Ignored.", SERVER_ERR);
                            return;
                        }

                        //Verify if it's a non private message
                        if(msg.get_isPrivate() != false)
                        {
                            writeToLog("[" + sender->get_socket()->peerAddress().toString() + "Received a private message for a text channel ? Ignored.", SERVER_WARN);
                            //Send error packet to client
                            CPacket err("1","3");

                            err.Serialize_MessageError(1);
                            sendToClient(err.GetByteArray(),sender);

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
                                writeToLog("Can't create path [" + path + "]",SERVER_ERR);
                                //Send error to client
                                //TODO
                                break;
                            }
                        }

                        path = "storage/public/" + msg.get_to() + "/" + QString::fromStdString(filename) + ".xml";
                        if(QFile::exists(path))
                        {
                            writeToLog("[" + sender->get_uuid().toString() + "(" + sender->get_pseudo() + ")] is spamming copy paste message in channel [" + msg.get_to() + "]",SERVER_WARN);
                            //Send error packet to client
                            CPacket err("1","3");

                            err.Serialize_MessageError(2);
                            sendToClient(err.GetByteArray(),sender);

                            break;
                        }

                        QFile file(path);

                        if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
                        {
                            writeToLog("Can't save received message from [" + msg.get_from() + "] to [" + msg.get_to() + "]",SERVER_ERR);

                            CPacket err("1","3");

                            err.Serialize_MessageError(3);
                            sendToClient(err.GetByteArray(),sender);

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
                                writeToLog("Error while reading channel index for channel [" + msg.get_to() + "]",SERVER_ERR);

                                CPacket err("1","3");

                                err.Serialize_MessageError(3);
                                sendToClient(err.GetByteArray(),sender);

                                break;
                            }
                            else
                            {
                                if(filename_list.first() == "noIndex")
                                {
                                    writeToLog("No index in index.json from channel [" + msg.get_to() + "] Try to create one",SERVER_WARN);
                                    if(!createChannelIndex(filename,path))
                                    {
                                        writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",SERVER_ERR);

                                        CPacket err("1","3");

                                        err.Serialize_MessageError(3);
                                        sendToClient(err.GetByteArray(),sender);

                                        break;
                                    }
                                    else
                                    {
                                        writeToLog("Index created successfully for channel [" + msg.get_to() + "]",SERVER);
                                    }
                                }
                                else
                                {
                                    filename_list.append(QString::fromStdString(filename));
                                    if(!insertChannelIndex(path,filename_list))
                                    {

                                        writeToLog("Error while inserting in channel index for channel [" + msg.get_to() + "]",SERVER_ERR);
                                        CPacket err("1","3");

                                        err.Serialize_MessageError(3);
                                        sendToClient(err.GetByteArray(),sender);

                                        break;
                                    }
                                }
                            }
                        }
                        else
                        {
                            if(!createChannelIndex(filename,path))
                            {
                                writeToLog("Error while creating channel index from channel [" + msg.get_to() + "]",SERVER_ERR);

                                CPacket err("1","3");

                                err.Serialize_MessageError(3);
                                sendToClient(err.GetByteArray(),sender);

                                break;
                            }
                        }
                        //Send to all
                        CPacket msgToAll("1","4");

                        msgToAll.Serialize_Message(msg);
                        sendToAll(msgToAll.GetByteArray());

                        break;
                    }

                    case 3:
                    {
                        QList<QString> info = packet->deserialize_messageRequest();

                        QList<CMessage> messages_list = createMessageList(info.at(1), false, info.at(2).toInt(), sender->get_uuid(), info.at(3).toInt());

                        if(messages_list.last().get_from() == "allIsSync")
                        {
                            break;
                        }

                        if(messages_list.last().get_from() == "noIndex")
                        {
                            CPacket reqAns("1","2");

                            reqAns.Serialize_MessageListInfo(info.at(1).toInt());
                            reqAns.Serialize_MessageList(messages_list);

                            sendToClient(reqAns.GetByteArray(), sender);

                            break;
                        }


                        CPacket reqAns("1","2");

                        reqAns.Serialize_MessageListInfo(info.at(1).toInt());
                        reqAns.Serialize_MessageList(messages_list);

                        sendToClient(reqAns.GetByteArray(), sender);

                        break;
                    }

                    case 4:
                    {
                        //Request profile picture
                        QString tmp = packet->deserialize_ppRequest();
                        if(tmp.size() != 38)
                        {
                            writeToLog("[" + sender->get_uuid().toString(QUuid::WithoutBraces) + "(" + sender->get_pseudo() + ")] Error in request :", SERVER_ERR);
                            writeToLog(tmp, SERVER_ERR);
                            break;
                        }
                        QUuid requested(tmp);

                        QString path = "storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png";
                        if(QFile::exists(path))
                        {
                            QImage img(path);
                            CPacket ppAns("1","14");

                            ppAns.Serialize_ppAnswer(img,requested);
                            ppAns.GetByteArray().size();

                            sendToClient(ppAns.GetByteArray(), sender);
                        }
                        else
                        {
                            writeToLog("Unable to find profile pic for user [" + requested.toString(QUuid::WithoutBraces) + "] Random new one generated" , SERVER_WARN);

                            int random = QRandomGenerator::global()->bounded(0,18);
                            path = "storage/server/pp/pp" + QString::number(random) + ".png";
                            if(QFile::exists(path))
                            {
                                QImage tmp(path);
                                QDir test;

                                if(test.exists("storage/private/" + requested.toString(QUuid::WithoutBraces)))
                                {
                                    tmp.save("storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                                }
                                else
                                {
                                    test.mkpath("storage/private/" + requested.toString(QUuid::WithoutBraces));
                                    tmp.save("storage/private/" + requested.toString(QUuid::WithoutBraces) + "/pp.png","PNG");
                                }

                                CPacket ppAns("1","14");

                                ppAns.Serialize_ppAnswer(tmp,requested);
                                ppAns.GetByteArray().size();

                                sendToClient(ppAns.GetByteArray(), sender);
                            }
                        }
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
                        m_db->updateChannel(std::to_string(channel->get_id()), channel->get_name().toStdString(), std::to_string(channel->get_maxUsers()));
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
                        writeToLog("Received invalid channel action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
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
                            break;
                        }
                        if(messages_list.last().get_from() == "noIndex")
                        {
                            break;
                        }

                        CPacket reqAns("2","6");

                        reqAns.Serialize_MessageList(messages_list);
                        sendToClient(reqAns.GetByteArray(), sender);
                        break;
                    }

                    default:
                    {
                        writeToLog("Received invalid user action :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
                        break;
                    }
                }
                break;
            }

            default:
            {
                writeToLog("Received invalid request type :\n" + packet->GetByteArray() +"\nIgnored", SERVER_WARN);
                break;
            }
        }
        return;
 }

// Unused functions, i leave them here in case @Valentin

/*QByteArray CServer::SerializeChannels()
{
    QJsonArray jsonArray;
    foreach(CChannel * c, get_channelList())
    {
        jsonArray.append(c->serializeToObj());
    }

    QJsonDocument jsonDoc(jsonArray);
    return jsonDoc.toJson();
}
*/

/*QByteArray CServer::SerializeClients()
{
    QJsonArray jsonArray;
    foreach(CClient * c, get_clientList())
    {
        jsonArray.append(c->serializeToObj());
    }

    QJsonDocument jsonDoc(jsonArray);
    return jsonDoc.toJson();
}
*/

/*void CServer::deserializeChannel(QJsonArray & json_array)
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
}*/

/*void CServer::deserializeClients(QJsonArray & json_array)
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
}*/

/*CChannel * CServer::deserializeToChannel(QJsonObject json_obj)
{
    CChannel * channel = new CChannel();

    channel->deserialize(json_obj);

    return channel;
}*/

/*CClient * CServer::deserializeToClient(QJsonObject json_obj)
{
    CClient * client = new CClient();
    client->deserialize(json_obj);
    return client;
}*/

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
        writeToLog("Can't open index of channel",SERVER_ERR);
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
        writeToLog("Can't open index of channel",SERVER_ERR);
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
                writeToLog("Error in index.json not containing correct message index",SERVER_ERR);
                return null;
            }
            array.removeFirst();
            index++;
        }
        return filename_list;
    }
    else
    {
        writeToLog(("Error while reading index.json, no field \"index\" found"), SERVER_ERR);
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
        writeToLog("Can't open index of channel",SERVER_ERR);
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

    if(!isPrivate)
    {
        default_path = "storage/public/" + id + "/";
    }
    else
    {
        default_path = "storage/private/" + sender.toString(QUuid::WithoutBraces) + "/" + id + "/";
    }

    QList<QString> filename_list = readChannelIndex(default_path + "index.json");

    if(filename_list.isEmpty() || filename_list.last() == "no_index")
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
                    writeToLog("Can't open message file [" + filename + "]",SERVER_ERR);
                }
                else
                {
                    CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                    message_list.append(tmp);
                }
            }
            else
            {
                writeToLog("Message [" + filename + "] may have been deleted and not removed from index [" + id + "]",SERVER_ERR);
            }
        }
        return message_list;
    }
    else
    {
        for(int i = index; i > start_index; i--)
        {
            path = default_path + filename_list[filename_list.size() - i] + ".xml";
            if(QFile::exists(path))
            {
                QFile file(path);
                if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    writeToLog("Can't open message file [" + filename_list[filename_list.size() - i] + "]",SERVER_ERR);
                }
                else
                {
                    CMessage tmp(QString::fromLocal8Bit(file.readAll()));
                    message_list.append(tmp);
                }
            }
            else
            {
                writeToLog("Message [" + filename_list[filename_list.size() - i] + "] may have been deleted and not removed from index [" + id + "]",SERVER_ERR);
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
