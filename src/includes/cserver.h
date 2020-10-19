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

        QByteArray SerializeChannels();                                     //Serialize channels into json document then byte array
        QByteArray SerializeClients();                                      //Serialize clients into json document then byte array

        void DeserializeChannels(QByteArray in);                            //Deserialize channels from byte array
        void DeserializeClient(QByteArray in);                              //Deserialize clients from byte array

        CChannel * deserializeToChannel(QJsonObject json_obj);              //Deserialize channels from json object
        CClient * deserializeToClient(QJsonObject json_obj);                //Deserialize clients from json object


        //Création d'un channel grâce à un nouveau process -
        void create_process();
        void del_process();


    private slots:

        void nouvelleConnexion();                                           //Add client to the server - default no channel           //get data
        void deconnexionClient();                                           //Disconnecting client - del client from channel list - del client
        void sReceiveData();                                                //Server receive data

    private:

        //Server mode
        QLabel *etatServeur;                                                //State of the server
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        QList<QTcpSocket *> clients;                                        //A suppr plus tard ( m_clients remplace clients)
        quint16 tailleMessage;

        QList<CChannel*> m_channels;                                        //List of channels
        QList<CClient *> m_clients;                                         //List of client

        CDatabase * m_db;                                                   //Database

        //Client mode

        QTcpSocket * m_socket;

};

#endif // CSERVER_H


