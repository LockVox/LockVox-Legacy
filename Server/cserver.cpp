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
    foreach(CClient * client, get_clientList())
    {
        connectClient(client);
    }

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
    CClient * newClient = new CClient();
    newClient->set_socket(serveur->nextPendingConnection());

    connect(newClient->get_socket(), SIGNAL(disconnected()), this, SLOT(onDisconnectClient()));
    connectClient(newClient);
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
    emit sendToAll(request.GetByteArray());
    return;
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

void CServer::connectClient(CClient *client)
{
    connect(client, SIGNAL(writeToLog(QString,int)), this, SLOT(ext_writeToLog(QString,int)));
    connect(client, SIGNAL(sendYou(QTcpSocket*)), this, SLOT(sendMe(QTcpSocket*)));

    connect(client, SIGNAL(updateMe(int,CClient*)), this, SLOT(updateClient(int,CClient*)));
    connect(client, SIGNAL(updateChan(int,CChannel*)), this, SLOT(updateChannel(int,CChannel*)));

    connect(client, SIGNAL(authMe(QList<QString>,CClient*)), this, SLOT(auth(QList<QString>,CClient*)));
    connect(client, SIGNAL(regMe(QList<QString>,CClient*)), this, SLOT(reg(QList<QString>,CClient*)));

    connect(client, SIGNAL(whichChan(int)), this, SLOT(thisChan(int)));

    connect(this, SIGNAL(sendToAll(QByteArray)), client, SLOT(sendToClient(QByteArray)));
    connect(client, SIGNAL(sendToAll(QByteArray)), this, SLOT(ext_sendToAll(QByteArray)));
}

void CServer::ext_writeToLog(QString error, int level)
{
    writeToLog(error, level);

    return;
}

CChannel * CServer::thisChan(int id)
{
    return get_channelById(id);
}

void CServer::sendMe(QTcpSocket * socket)
{
    CPacket * objServer = new CPacket("-1","-1");
    objServer->Serialize(this);
    socket->write(objServer->GetByteArray());
    socket->waitForBytesWritten();

    return;
}

void CServer::updateClient(int update_level, CClient * client)
{
    switch (update_level)
    {
        //Pseudo
        case 0:
        {
            QString res = m_db->updateUsername(client->get_uuid().toString(QUuid::WithoutBraces).toStdString(), client->get_pseudo().toStdString());

            if(res=="success")
            {
                //Send update
                CPacket ans("0","2");
                ans.Serialize_newClient(client,false);
                emit sendToAll(ans.GetByteArray());
            }
            else
            {
                writeToLog(res, DB_ERR);
            }

            free(client);
            break;
        }

        //Description
        case 1:
        {
            QString res = m_db->updateDescription(client->get_uuid().toString(QUuid::WithoutBraces).toStdString(), client->get_description().toStdString());

            if(res=="success")
            {
                //Send update
                CPacket ans("0","3");
                ans.Serialize_newClient(client, false);
                emit sendToAll(ans.GetByteArray());
            }
            else
            {
                writeToLog(res, DB_ERR);
            }

            free(client);
            break;
        }

    }
}

void CServer::updateChannel(int update_type, CChannel * channel)
{
    switch (update_type)
    {
        case 0: //Create audio chan
        {
            addChannel(channel);

            CPacket ans("1","5");
            ans.Serialize_newChannel(channel);
            emit sendToAll(ans.GetByteArray());

            break;
        }

        case 1: //Delete audio chan
        {
            CChannel * toDelChannel = get_channelById(channel->get_id());
            DelChannel(toDelChannel);

            CPacket ans("1","6");
            ans.Serialize_newChannel(channel);
            emit sendToAll(ans.GetByteArray());

            free(channel);
            break;
        }

        case 2: //Rename audio chan
        {
            CChannel * chan = get_channelById(channel->get_id());

            chan->set_name(channel->get_name());
            m_db->updateChannel(std::to_string(chan->get_id()), chan->get_name().toStdString(), std::to_string(chan->get_maxUsers()));

            CPacket ans("1","7");
            ans.Serialize_newChannel(chan);
            emit sendToAll(ans.GetByteArray());

            free(channel);
            break;
        }

        case 3: //Change max user audio chan
        {
            CChannel * chan = get_channelById(channel->get_id());
            chan->set_maxUsers(channel->get_maxUsers());

            CPacket ans("1","8");
            ans.Serialize_newChannel(chan);
            emit sendToAll(ans.GetByteArray());

            free(channel);
            break;
        }
    }
}

void CServer::auth(QList<QString> info, CClient * client)
{
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

                    client->get_socket()->write(ans->GetByteArray());
                    client->get_socket()->waitForBytesWritten();

                    client->get_socket()->abort();
                    client->deleteLater();
                    return;
                }

                if(tmp_client->get_uuid() == c->get_uuid() && !c->get_isAuthenticate()) //If not connect him
                {
                    writeToLog("User [" + c->get_uuid().toString()  + "(" + c->get_pseudo() + ")] connected from [" + client->get_socket()->peerAddress().toString() + "]",SERVER);

                    c->set_isOnline(true);
                    c->set_isAuthenticate(true);
                    c->set_socket(client->get_socket());

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
                delete(err);
            }
            else //Bad password
            {
                writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Bad password for [" + tmp_client->get_uuid().toString()  + "(" + tmp_client->get_pseudo() + ")]", SERVER_WARN);
                ans->Serialize_auth(NULL, 3);
                delete(err);
            }
        }
    }
    else
    {
        if(*err == "no_client")
        {
            writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Trying to connect to a non-existing account", SERVER_WARN);
            ans->Serialize_auth(NULL, 1);
            delete(err);
        }
        else
        {
            writeToLog("Error with database when fetching client",SERVER_ERR);
            writeToLog(*err,DB_ERR);
            ans->Serialize_auth(NULL, 1);
            delete(err);
        }
    }

    client->get_socket()->write(ans->GetByteArray());
    client->get_socket()->waitForBytesWritten();

    //If authentification suceed - Send Server Object to the client
    if(valid)
    {
        sendToAllExecptClient(newUser.GetByteArray(), tmp_client);

        //Get ride of temporary CClient, now we use in list client
        client->set_socket(NULL);
        client->quit();
        client->deleteLater();
    }
    return;
}

void CServer::reg(QList<QString> info, CClient * client)
{
    //Check informations
    if(info[0] == "null")
    {
        writeToLog("Received incomplete register information from [" + client->get_socket()->peerAddress().toString() + "]", SERVER_WARN);
        //Send error to client
        //TODO
        return;
    }

    QUuid uuid = QUuid::createUuid();

    QString error = m_db->newUser(uuid.toString(QUuid::WithoutBraces).toStdString(), info[0].toStdString(), info[1].toStdString(),info[2].toStdString());

    if(error == "mailerror")
    {
        //Mail already exist
        writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Trying to create account with existing mail address" ,SERVER_WARN);
        CPacket ans("0", "8");
        ans.Serialize_regAns(0);

        //Send register answer
        client->get_socket()->write(ans.GetByteArray());
        client->get_socket()->waitForBytesWritten();
        return;
    }
    else
    {
        if(error != "success")
        {
            writeToLog("Error in creating new user", SERVER_ERR);
            writeToLog(error,DB_ERR);
            //Send error to client
            //TODO
            return;
        }
        else
        {
            writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Created new account [" + uuid.toString() + "(" + info[0] + ")] Successfully", SERVER);
            CClient * c = new CClient(uuid, info[0], client->get_socket(), -1, true, "");
            addClient(c);

            int random = QRandomGenerator::global()->bounded(0,18);
            QString path = "storage/server/pp/pp" + QString::number(random) + ".png";
            if(QFile::exists(path))
            {
                QImage tmp(path);
                c->set_profilePic(tmp);
                QDir test;
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

            //Send answer to the client
            CPacket ans("0", "8");
            ans.Serialize_regAns(1);

            //Serialize here the client with the corresponding information
            ans.Serialize_myClient(c);

            //Send register answer
            c->get_socket()->write(ans.GetByteArray());
            c->get_socket()->waitForBytesWritten();

            //sender->get_socket()->write(Serialize());
            //sender->get_socket()->waitForBytesWritten();

            CPacket newUser("0","0");
            newUser.Serialize_newClient(c,true);
            sendToAllExecptClient(newUser.GetByteArray(),c);

            //Get ride of temporary CClient now it's in da list
            client->set_socket(NULL);
            client->quit();
            client->deleteLater();
        }
    }
    return;
}

void CServer::ext_sendToAll(QByteArray out)
{
    emit sendToAll(out);
}
