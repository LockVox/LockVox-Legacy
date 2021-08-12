#include "Server/includes/cserver.h"
#include "Server/includes/cdatabase.h"
#include "Server/includes/sha256.h"

#include <QDebug>

/**
 * @brief The CServer constructor
 * @see CServer::CServer
 */
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

    log_level = USER;
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

/**
 * @brief This function should be called to start the server. Starting TCP Server and initialising DB connection.
 */
void CServer::start()
{

    // Gestion du serveur TCP
    writeToLog("Starting server...", SERVER);

    //Read config file
    readConfig();
    printConfig();

    //Databse
    m_db = new CDatabase();
    QString err =  m_db->init(m_UserDb, m_passwordDb, m_portDb, m_hostnameDb, m_nameDb);
    if(err != "success")
    {
        writeToLog(err,DB_ERR);
        abort();
    }

    m_db->start();

    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, 50885)) //Statuate on server starting listening for every adresse on port given
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

//Read and parse config file, assign values
int CServer::readConfig()
{
    qDebug() << "Reading config files..." << CONFIG_FILE_PATH;

    QFile f(CONFIG_FILE_PATH);

    if(f.exists() == true){
        //Open file
        f.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&f);

        //Read lines
        while(!in.atEnd()){
            QString line = in.readLine();

            if(line.contains("=")){
               QStringList strList = line.split("=");
               //qDebug() << strList[0] << " " << strList[1];
               //Assign values

               //Informations
               if(strList[0] == "server")
                   m_serverName = strList[1];

               //Database
               if(strList[0] == "db_port")
                   m_portDb = strList[1].toInt();
               if(strList[0] == "db_hostname")
                   m_hostnameDb = strList[1];
               if(strList[0] == "db_name")
                   m_nameDb = strList[1];

               if(strList[0] == "db_login")
                   m_UserDb = strList[1];
               if(strList[0] == "db_password")
                   m_passwordDb = strList[1];

               //Server parameter
               if(strList[0] == "default_port")
                   m_serverPort = strList[1].toInt();
               if(strList[0] == "audio_default_port")
                   m_audioBasePort = strList[1].toInt();
               if(strList[0] == "maxUsers")
                   m_maxUsers = strList[1].toInt();
               if(strList[0] == "maxChannels")
                   m_maxChannels = strList[1].toInt();
               if(strList[0] == "maxAudioSessions")
                   m_maxAudioSessions = strList[1].toInt();

               //Log
               if(strList[0] == "log_path")
                   m_logFilePath = strList[1];

            }
        }
        return 1;
    } else {
        qDebug() << "Can't find config file " << CONFIG_FILE_PATH;
        return 0;
    }
    //manage errors reading or parsing config file.
}

//Print parsed config file values
void CServer::printConfig()
{
    qDebug() << "Printing server informations and server";
    qDebug() << "\n";
    qDebug() << "[INFORMATIONS]";
    qDebug() << "Server name             : " << m_serverName;
    qDebug() << "";
    qDebug() << "[PARAMETERS]";
    qDebug() << "Serveur port base       : " << m_serverPort;
    qDebug() << "Serveur Audio port base : " << m_audioBasePort;
    qDebug() << "Max simultaneous users  : " << m_maxUsers;
    qDebug() << "Max channels            : " << m_maxChannels;
    qDebug() << "Max audio sessions      : " << m_maxAudioSessions;
    qDebug() << "";
    qDebug() << "[DATABASE]";
    qDebug() << "Database hostname       : " << m_hostnameDb ;
    qDebug() << "Database name           : " << m_nameDb;
    qDebug() << "Username                : " << m_UserDb;
    qDebug() << "Database port           : " << m_portDb;
    qDebug() << "";
    qDebug() << "[LOG]";
    qDebug() << "Log file path           : " << m_logFilePath;
    qDebug() << "";
    qDebug() << "";
}

void CServer::fromJSON(QJsonObject obj)
{
    QJsonObject serverParams;
    QJsonArray clientsArray, channelsArray;
    CServer server;
    CChannel channel;

    if(obj.contains("serverParams")){
        serverParams  = obj.value("serverParams").toObject();
        server.setServerName(serverParams["name"].toString());
        //To continue;
    }

    if(obj.contains("clients")){
        clientsArray = obj.value("clients").toArray();

        foreach(QJsonValue c, clientsArray){
            QJsonObject tmp = c.toObject();
            CClient client;
            client.fromJSON(tmp);
            server.addClient(&client);
        }
    }

    if(obj.contains("channels")){
        clientsArray = obj.value("channels").toArray();

        foreach(QJsonValue c, clientsArray){
            QJsonObject tmp = c.toObject();
            CChannel channel;
            channel.fromJSON(tmp);
            server.addChannel(&channel);
        }
    }
}

QJsonObject CServer::toJSON()
{
    QJsonObject json_obj;
    QJsonObject json_serverParams;
    QJsonArray clientsArray, channelsArray;

    json_serverParams["name"] = m_serverName;

    foreach(CClient * c, m_clients){
        clientsArray.append(c->toJSON());
    }

    foreach(CChannel * c, m_channels){
        channelsArray.append(c->toJSON());
    }

    json_obj["channels"] = channelsArray;
    json_obj["clients"] = clientsArray;
    json_obj["serverParams"] = json_serverParams;

    return json_obj;

}

CChannel *CServer::findChannel(CChannel * channel)
{
    foreach(CChannel * c, m_channels){
        if(c == channel){
            return c;
        }
    }
    return NULL;
}

CClient *CServer::findUsers(CClient * client)
{
    foreach(CClient * c, m_clients){
        if(c == client){
            return c;
        }
    }
    return NULL;
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

/*
 * Look in the list of users to check if the socket exists.
 */
CClient * CServer::whichClient(QTcpSocket * s)
{

    foreach(CClient * client, get_clientList())
    {
        if(*client == s)
        {
            return client;
        }
    }
    return NULL;
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
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][User]";
                break;
            }
            case 1:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server]";
                break;
            }
            case 2:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server Warning]";
                break;
            }
            case 3:
            {
                prefix = "[" + current.toString("dd/MM/yy hh:mm:ss") + "][Server Error]";
                break;
            }
            case 4:
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

    connect(client, SIGNAL(updateClient(ClientParams,CClient*,QString)), this, SLOT(updateClient(ClientParams,CClient*,QString)));
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

void CServer::updateClient(ClientParams param,CClient * client, QString newString)
{
    switch (param)
    {
        //Pseudo
        case USERNAME:
        {
            //UPDATE USERNAME
            QString res = m_db->updateUsername(client->get_uuid().toString(QUuid::WithoutBraces).toStdString(), newString.toStdString());
            if(res=="success")
            {
#ifdef SERVER_DEBUG
                qDebug() << "[SERVER] Update username for " << client->get_pseudo() << " to " << newString;
#endif
                client->set_pseudo(newString);
                //Send update
                CPacket ans("0","2");
                ans.Serialize_newClient(client,false);
                emit sendToAll(ans.GetByteArray());
            }
            else
            {
#ifdef SERVER_DEBUG
                qDebug() << "[SERVER] Failed to update username for " << client->get_pseudo() << " to " << newString << " - " << res;
#endif
                writeToLog(res, DB_ERR);
            }
            break;
        }
        //Description
        case DESCRIPTION:
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
        case MAIL:
        {

        }
    }
}

void CServer::updateChannel(ChannelParams param, CChannel * target, QString newString)
{
    CChannel * channel = findChannel(target);
    if(channel == NULL) { return ; }

    switch (param)
    {
        case NAME:
        {
            //m_db->updateChannel()

            break;
        }

        case NB_USERS:
        {

            break;
        }

        case MAX_USERS:
        {

            break;
        } 
    }
}

QString CServer::getServerName() const
{
    return m_serverName;
}

void CServer::setServerName(const QString &serverName)
{
    m_serverName = serverName;
}

void CServer::auth(sAuthentication info)
{
    CClient * client = qobject_cast<CClient*>(sender());
    if(client == NULL) { return; }

    //Password hash
    std::string hashed = sha256(info.password.toStdString());
    bool valid = false;

    QString * err = new QString;
    CClient * tmp_client = m_db->parseClient(info.mail.toStdString(), err);

    if(*err == "success")
    {
        if(hashed == m_db->getHash(info.mail.toStdString(), err))  //If password is correct
        {
            foreach(CClient *c, m_clients) //Check if user already connected
            {
                if(tmp_client == c && c->get_isAuthenticate()) //For the moment if yes just don't allow second connection
                {

#ifdef SERVER_DEBUG
                    qDebug() << "[SERVER] Authentication failed - " << c->get_pseudo();
#endif
                    writeToLog("User [" + c->get_uuid().toString() + "(" + c->get_pseudo() + ")] Already connected", SERVER_WARN);
                    return;
                }

                if(tmp_client->get_uuid() == c->get_uuid() && !c->get_isAuthenticate()) //If not connect him
                {
                    writeToLog("User [" + c->get_uuid().toString()  + "(" + c->get_pseudo() + ")] connected from [" + client->get_socket()->peerAddress().toString() + "]",SERVER);

#ifdef SERVER_DEBUG
                      qDebug() << "[SERVER] Authentication success - " << c->get_pseudo();
#endif
                    c->set_isOnline(true);
                    c->set_isAuthenticate(true);
                    c->set_socket(client->get_socket());

                    //Sending client info 
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
                delete(err);
            }
            else //Bad password
            {
                writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Bad password for [" + tmp_client->get_uuid().toString()  + "(" + tmp_client->get_pseudo() + ")]", SERVER_WARN); 
                delete(err);
            }
        }
    }
    else
    {
#ifdef SERVER_DEBUG
                      qDebug() << "[SERVER] Authentication failed" << info.mail;
#endif

        if(*err == "no_client")
        {
            writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Trying to connect to a non-existing account", SERVER_WARN);

            delete(err);
        }
        else
        {
            writeToLog("Error with database when fetching client",SERVER_ERR);
            writeToLog(*err,DB_ERR);

            delete(err);
        }
    }


    if(valid)
    {
        //Send Authentication answer
        CPacket packet_auth(NOTIFY, ANS, SUCCESS, _SERVER, AUTH);
        client->writePacket(packet_auth);


        CPacket packet_sync(INFO, REQ, NO_STATE,  _SERVER, SYNC_INFO);
        packet_sync.serialize(this);
        client->writePacket(packet_sync);


        CPacket packet_inf(INFO, REQ, SUCCESS, _USERS, UserAction::CONNECT);
        //sendToAllExecptClient()



        //Get ride of temporary CClient, now we use in list client
        client->set_socket(NULL);
        client->quit();
        client->deleteLater();
    }
    return;
}

void CServer::reg(sRegister info)
{
    CClient * client = qobject_cast<CClient*>(sender());
    if(client == NULL) { return; }

    //Check informations
    if(info.mail == "" || info.password == "" || info.username == "")
    {
        writeToLog("Received incomplete register information from [" + client->get_socket()->peerAddress().toString() + "]", SERVER_WARN);
        //Send error to client
        //TODO
        return;
    }

    QUuid uuid = QUuid::createUuid();
    QString error = m_db->newUser(uuid.toString(QUuid::WithoutBraces).toStdString(), info.username.toStdString(), info.mail.toStdString(),info.password.toStdString());

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
    else if(error != "success") {
            writeToLog("Error in creating new user", SERVER_ERR);
            writeToLog(error,DB_ERR);
            //Send error to client
            //TODO
            return;
    }
    else
    {
            writeToLog("[" + client->get_socket()->peerAddress().toString() + "] Created new account [" + uuid.toString() + "(" + info[0] + ")] Successfully", SERVER);
            CClient * c = new CClient(uuid, info.username, client->get_socket(), -1, true, "");
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
            CPacket packet(ACTION, ANS, SUCCESS, _SERVER, REG);
            c->writePacket(packet);

            //Get ride of temporary CClient now it's in da list
            client->set_socket(NULL);
            client->quit();
            client->deleteLater();
    }
    return;
}

void CServer::ext_sendToAll(QByteArray out)
{
    emit sendToAll(out);
}


/*
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
*/
