#ifndef CSERVER_H
#define CSERVER_H

#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QRegExp>


#include "cchannel.h"
#include "cdatabase.h"

class CClient;
class CChannel;

class CServer : public QWidget
{
    Q_OBJECT

    public:
        CServer();


        //Getters
        CDatabase * get_database();
        QList<CChannel *> get_channelList();
        QList<CClient *> get_clientList();
        QList<CChannel> get_staticChannelList();
        QList<CClient> get_staticClientList();
        QTcpSocket * get_socket();


        //Setters
        void set_database(CDatabase * db);
        void set_clients(QList<CClient> clients);
        void set_channels(QList<CChannel> channels);
        void set_channel(CChannel channel, int index);
        void set_socket(QTcpSocket* soc);


        //free
        void freeChannels();



        //Network
        void envoyerATous(const QString &message);
        void sendToChannel(const QString &message, int id_channel);
        void sendToClient(const QString &message,CClient * client);


        //Network refait
        void SendObjectsToClient();                                         //Send channels and clients objects
        void cReceiveData();                                                //Client receive data
        void sendToAll(QByteArray out);                                     //Send packet to everyone

        //Channels management
        void addChannel(CChannel * tmp);
        int DelChannel(CChannel * tmp);
        int DelChannel(int id);

        //Users management
        void addClient(CClient * client);
        int delClient(CClient * client);
        int delClient(int id);

        //Server action - To develop
        void changeChannel(int id);
        void quitChannel(int id);
        void joinChannel(int id);

        int whichClient(QTcpSocket * soc);



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


        //Création d'un channel grâce à un nouveau process -
        void create_process();
        void del_process();


    private slots:

        void nouvelleConnexion();                                           //Add client to the server - default no channel           //get data
        void deconnexionClient();                                           //Disconnecting client - del client from channel list - del client
        void sReceiveData(CClient sender, QByteArray data);                                         //Server receive data

    private:

        //Server mode
        QLabel *etatServeur;                                                //State of the server
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        QMap<QTcpSocket *, CClient> m_online_clients;       //Lien socket/user
        quint16 tailleMessage;

        QList<CChannel*> m_channels;                                        //List of channels
        QList<CClient *> m_clients;                                         //List of client
        QList<crole*> m_roles;
        CDatabase * m_db;                                                   //Database

        //Tableaux
        bool **t_user_right;    //true user doesn't have the right ; false he has it
        bool **t_chan_right;    //true channel isn't concerned by the right ; 1 he is
        int **t_user_chan;     //-1 not part of ; 0 not in chan ; 1 in chan

        QList<CChannel*> GetRightChannels(int p_rightid);
        QList<CClient*> GetRightClient(int p_rightid);

        QList<crole*> GetUserRoles(int p_userid);
        QList<CChannel*> GetUserChannels(int p_userid);

        QList<CClient*> GetChannelClients(int p_chanid, bool p_online);
        QList<crole*> GetChannelRoles(int p_chanid);

        //Client mode

        QTcpSocket * m_socket;



       //Audio
       // QUdpSocket * m_udp_server;

        QTcpServer  m_audio_server;

};

#endif // CSERVER_H


/*
// Get network interfaces list
QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();

// Interfaces iteration
for (int i = 0; i < ifaces.size(); i++)
{
    // Now get all IP addresses for the current interface
    QList<QNetworkAddressEntry> addrs = ifaces[i].addressEntries();

    // And for any IP address, if it is IPv4 and the interface is active, send the packet
    for (int j = 0; j < addrs.size(); j++)
        if ((addrs[j].ip().protocol() == QAbstractSocket::IPv4Protocol) && (addrs[j].broadcast().toString() != ""))
            udpManager.writeDatagram(packet->data(), packet->length(), addrs[j].broadcast(), 64501);
}*/

