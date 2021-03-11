#ifndef CSERVER_H
#define CSERVER_H

#include "libs/emiplib/include/emiplib/mipcomponentchain.h"
#include "libs/emiplib/include/emiplib/mipcomponent.h"

#include <QtWidgets>
//#include <QtNetwork>
#include <QDebug>
#include <QRegExp>
#include <QDir>


#include "src/includes/AbstractServer.h"

#include "src/includes/cchannel.h"
#include "src/includes/cdatabase.h"

#include "src/includes/cpacket.h"



class CClient;
class CChannel;

class CServer : public AbstractServer
{

    Q_OBJECT
    public:

        CServer();

        //Getters
        CDatabase * get_database();

        //Setters
        void set_database(CDatabase * db);

        int whichClient(QTcpSocket * soc);                                  //Find ID client from socket
        //Network
        void sendToChannel(const QString &message, int id_channel);         //Send message to a channel
        void sendToClient(const QString &message,CClient * client);         //Send message to a client
        void SendObjectsToClient();                                         //Send channels and clients objects

        void sendToAll(QByteArray out);                                     //Send packet to everyone
        void sendToClient(QByteArray out, CClient * client);

        //Admin
        void AddBannedUser(CClient * client);
        void RemoveBannedUser(CClient* client);
        QList<CClient*> GetBannedUserList();

        //Process
        void processIncomingData(CClient *sender, QByteArray data);         //Process incoming data

        //Serialization | Deserialization
        QByteArray Serialize();                                             //Serialize client and channels on the same json document
        void Deserialize(QByteArray in);

        QByteArray SerializeChannels();                                     //Serialize channels into json document then byte array
        QByteArray SerializeClients();                                      //Serialize clients into json document then byte array

        void DeserializeChannels(QByteArray in);                            //Deserialize channels from byte array
        void DeserializeClient(QByteArray in);                              //Deserialize clients from byte array
        void deserializeChannel(QJsonArray & json_array);
        void deserializeClients(QJsonArray & json_array);

        CChannel * deserializeToChannel(QJsonObject json_obj);              //Deserialize channels from json object
        CClient * deserializeToClient(QJsonObject json_obj);                //Deserialize clients from json object

        bool createChannelIndex(string filename, QString path_to_index); //Creates an index.json for an empty channel and add the first message of it
        QList<QString> readChannelIndex(QString path_to_index); //Returns the list of names of all messages files contained in the given index
        bool insertChannelIndex(QString path_to_index, QList<QString> filename_list);  //Update index.json when inserting new message to it
        QList<CMessage> createMessageList(QString path_to_index, QString id, bool isPrivate); //Creates a QList of CMessage stored localy using index.json


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

};

#endif // CSERVER_H
