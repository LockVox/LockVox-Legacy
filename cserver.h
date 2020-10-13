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

        void set_socket(QTcpSocket* soc);

        //free
        void freeChannels();




        //Channels management
        void addChannel(CChannel * tmp);
        int DelChannel(CChannel * tmp);
        int DelChannel(int id);

        //Users management







        //Network
        void envoyerATous(const QString &message);
        void sendToChannel(const QString &message, int id_channel);
        void sendToClient(const QString &message,CClient * client);
        void sendToAll(QByteArray out);                                     //Send to every client

        void SendObjectsToClient();                                         //Send channels and clients objects

        void sReceiveData();                                                //Server receive data
        void cReceiveData();                                                //Client receive data

        //Serializaion
        void ClientsToDatastream(QDataStream & ds);                         //Take a list of clients and fill the datastream
        void ChannelsToDatastream(QDataStream & ds);                        //Take a list of channels and fill the datastream

        //Deserialization
        void DatastreamToChannels(QDataStream & ds);                        //Take a Datastream and fill the list of channels
        void DatastreamToClients(QDataStream & ds);                         //Take a Datastream and fill the list of channels


        //Création d'un channel grâce à un nouveau process -
        void create_process();
        void del_process();


    private slots:

        void nouvelleConnexion();                           //Add client to the server - default no channel           //get data
        void deconnexionClient();                           //Disconnecting client - del client from channel list - del client


    private:

        //Server mode
        QLabel *etatServeur;                //State of the server
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        QList<QTcpSocket *> clients;        //A suppr plus tard ( m_clients remplace clients)
        quint16 tailleMessage;

        QList<CChannel*> m_channels;        //List of channels
        QList<CClient *> m_clients;         //List of client

        CDatabase * m_db;

        //Client mode

        QTcpSocket * m_socket;

};

#endif // CSERVER_H


