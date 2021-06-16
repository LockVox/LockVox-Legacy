/**
 * @file        cclient.h
 * @brief       Declaration of the class CServer
 * @details     Here is the main process of a LockVox server
 * @author      LockVox Team
 * @version     0.2.0
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CSERVER_H
#define CSERVER_H

#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QRandomGenerator>

#include "AbstractServer.h"
#include "cdatabase.h"
#include "cpacket.h"
#include "caudio.h"

#include "Server/config.h"

class CDatabase;

class CServer : public AbstractServer
{
    Q_OBJECT
    public:

        CServer();
        void start();

        //Getters
        CDatabase * get_database();

        //Setters
        void set_database(CDatabase * db);

        CClient * whichClient(QTcpSocket * soc);                                  //Find ID client from socket

        void sendToClient(QByteArray out, CClient * client);
        void sendToAllExecptClient(QByteArray out, CClient * client);

        //Admin
        void AddChannel(CChannel* channel);
        void AddBannedUser(CClient * client);
        void RemoveBannedUser(CClient* client);
        QList<CClient*> GetBannedUserList();

        //Log lol
        void writeToLog(QString error, int level); //Write to server log, level : 0 normal | 1 warning | 2 error


        void connectClient(CClient * client);

    signals:
        void sendToAll(QByteArray out);

    private slots:
        void onNewConnection();                                           //Add client to the server - default no channel           //get data
        void onDisconnectClient();                                        //Disconnecting client - del client from channel list - del client

        //CClient
        void ext_writeToLog(QString error, int level);
        void ext_sendToAll(QByteArray out);

        void sendMe(QTcpSocket * socket);
        void updateClient(int update_type, CClient * client);
        void updateChannel(int update_type, CChannel * channel);

        void auth(QList<QString> info, CClient * client);
        void reg(QList<QString> info, CClient * client);

        CChannel * thisChan(int id);

    private:
        //Server mode
        QTcpServer *serveur;

        //Database
        CDatabase * m_db;                                                //Database
        QList<CClient*> m_banned_users;

        QFile log_file;
        QTextStream log;
        int log_level;

        QDateTime current;
        //CAudio* m_audio;

};

#endif // CSERVER_H
