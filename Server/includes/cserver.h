/**
 * @file        cserver.h
 * @brief       Declaration of the class CServer
 * @details     This class is the main class of LockVox server
 * @author      LockVox Team
 * @version     0.2.0
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CSERVER_H
#define CSERVER_H

//QT Includes
#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QRandomGenerator>

//Includes
#include "AbstractServer.h"
#include "cdatabase.h"
#include "cpacket.h"
#include "caudio.h"

//Config file for compilation
#include "Server/config.h"

//Class definition
class CDatabase;

//Class
class CServer : public AbstractServer
{
    /**
     * @class       CServer cserver.h "cserver.h"
     * @brief       Declaration of the class CServer
     * @details     This class is the main process of a LockVox server
     * @author      LockVox Team
     * @version     0.2.0
     * @date        2021
     * @note
     * @bug
     * @warning
     * @remark
     * @copyright   GNU GPL-3.0
     */

    Q_OBJECT
    public:
        //Constructor
        CServer();

        //Getters
        CDatabase * get_database();
        //Setters
        void set_database(CDatabase * db);


        //Start server
        void start();
        void readConfig();
        void printConfig();

        //Server action
        void connectClient(CClient * client);
        void AddChannel(CChannel* channel);
        void AddBannedUser(CClient * client);
        void RemoveBannedUser(CClient* client);
        QList<CClient*> GetBannedUserList();



        //Not used anymore
        CClient * whichClient(QTcpSocket * soc);                                  //Find ID client from socket


        //Will be switch into a NetworkWorker class;
        void sendToClient(QByteArray out, CClient * client);
        void sendToAllExecptClient(QByteArray out, CClient * client);

        //Log lol
        void writeToLog(QString error, int level); //Write to server log, level : 0 normal | 1 warning | 2 error

    signals:
        void sendToAll(QByteArray out);

    private slots:
        //TCP Server slots
        void onNewConnection();                                           //Add client to the server - default no channel           //get data
        void onDisconnectClient();                                        //Disconnecting client - del client from channel list - del client

        //CClient
        void ext_writeToLog(QString error, int level);
        void ext_sendToAll(QByteArray out);

        void sendMe(QTcpSocket * socket);
        void updateClient(int update_type, CClient * client);
        void updateChannel(int update_type, CChannel * channel);

        //Authentication / Register
        void auth(QList<QString> info, CClient * client);
        void reg(QList<QString> info, CClient * client);

        CChannel * thisChan(int id);

    private:
        //Server mode
        QTcpServer *serveur;                    //!< TCP Serveur

        //Informations
        QString m_serverName;

        //Database
        CDatabase * m_db;                       //!< Database
        QString m_UserDb;                       //!< databse username
        QString m_passwordDb;                   //!< database password
        QString m_hostnameDb;                   //!< database hostname
        QString m_nameDb;                       //!< database name
        int m_portDb;                           //!< database port

        //Logs
        QString m_logFilePath;                   //!< Log file path
        QFile log_file;                         //!< Log file
        QTextStream log;
        int log_level;                          //!< Log level, if the event is equal or higher to log level, then event will be log.

        QDateTime current;                      //!< Current date time

        QList<CClient*> m_banned_users;         //!< Blacklist users
        QList<QString> m_blacklist_ip;          //!< Blacklist @IP

        //Server parameters
        int m_serverPort;                       //!< TCP Server port
        int m_audioBasePort;                    //!< Audio base port for audio sessions
        int m_maxUsers;                         //!< Max simultaneous connected users
        int m_maxChannels;                      //!< Max channels
        int m_maxAudioSessions;                 //!< Limit number of simultaneous audio sessions

};

#endif // CSERVER_H
