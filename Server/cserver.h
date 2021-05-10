#ifndef CSERVER_H
#define CSERVER_H

#include "caudio.h"
#include "libs/emiplib/include/emiplib/mipcomponentchain.h"
#include "libs/emiplib/include/emiplib/mipcomponent.h"

#include <QDebug>
#include <QRegExp>
#include <QDir>
#include <QRandomGenerator>


#include "src/includes/AbstractServer.h"
#include "src/includes/cchannel.h"
#include "src/includes/cdatabase.h"
#include "src/includes/cpacket.h"

#include "Server/config.h"

#define SERVER 0
#define SERVER_WARN 1
#define SERVER_ERR 2
#define DB_ERR 3


class CClient;
class CChannel;

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

        void sendToAll(QByteArray out);                                     //Send packet to everyone
        void sendToClient(QByteArray out, CClient * client);
        void sendToAllExecptClient(QByteArray out, CClient * client);

        //Admin
        void AddChannel(CChannel* channel);
        void AddBannedUser(CClient * client);
        void RemoveBannedUser(CClient* client);
        QList<CClient*> GetBannedUserList();

        //Process
        void processIncomingData(CClient *sender, QByteArray data);         //Process incoming data

        //Unused functions, i leave it here for the moment @Valentin
        /*
        //Serialization | Deserialization
        QByteArray SerializeChannels();
        QByteArray SerializeClients();

        void deserializeChannel(QJsonArray & json_array);
        void deserializeClients(QJsonArray & json_array);

        CChannel * deserializeToChannel(QJsonObject json_obj);              //Deserialize channels from json object
        CClient * deserializeToClient(QJsonObject json_obj);                //Deserialize clients from json objec
        */

        //Messages functions
        bool createChannelIndex(string filename, QString path_to_index); //Creates an index.json for an empty channel and add the first message of it
        QList<QString> readChannelIndex(QString path_to_index); //Returns the list of names of all messages files contained in the given index
        bool insertChannelIndex(QString path_to_index, QList<QString> filename_list);  //Update index.json when inserting new message to it
        QList<CMessage> createMessageList(QString id, bool isPrivate,int nb_msg_to_sync, QUuid sender, int start_index); //Creates a QList of CMessage stored localy using index.json

        //Log lol
        void writeToLog(QString error, int level); //Write to server log, level : 0 normal | 1 warning | 2 error


    public slots:


    private slots:
        void onNewConnection();                                           //Add client to the server - default no channel           //get data
        void onDisconnectClient();                                        //Disconnecting client - del client from channel list - del client
        void onReceiveData();                                                //Server receive data

    private:
        //Server mode
        QTcpServer *serveur;

        //Database
        CDatabase * m_db;                                                   //Database
        QList<CClient*> m_banned_users;
        QFile log_file;
        QTextStream log;
        int log_level;
        QDateTime current;
        CAudio* m_audio;

};

#endif // CSERVER_H
