#ifndef CSERVER_H
#define CSERVER_H

#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QRegExp>


#include "cchannel.h"

class CClient;
class CChannel;

class CServer : public QWidget
{
    Q_OBJECT

    public:
        CServer();
        void envoyerATous(const QString &message);
        void sendToChannel(const QString &message, int id_channel);
        void sendToClient(const QString &message,CClient * client);


        void sendToAll(QByteArray out);





        //Création d'un channel grâce à un nouveau process -
        void create_process();
        void del_process();



        void addChannel(CChannel * tmp);
        int DelChannel(CChannel * tmp);
        int DelChannel(int id);


        QList<CChannel*> m_channels;
        QList<CClient *> m_clients;


        void FillDataStreamFromServer(QDataStream & ds);
        void FillDataStreamFromClient(QDataStream & ds);
        void FillDataStreamFromChannel(QDataStream & ds);


        void FillClientFromDataStream(QDataStream & ds);
        void FillChannelFromDataStream(QDataStream & ds);
        void FillServerFromDataStream(QDataStream & ds);

        void SendObjectsToClient();
        void receiveData();

    private slots:

        void nouvelleConnexion();           //Add client to the server - default no channel           //get data
        void deconnexionClient();           //Disconnecting client - del client from channel list - del client

    private:
        QLabel *etatServeur;                //State of the server
        QPushButton *boutonQuitter;

        QTcpServer *serveur;
        QList<QTcpSocket *> clients;        //A suppr plus tard ( m_clients remplace clients)
        quint16 tailleMessage;


                //List of channels
                //List of client

};

#endif // CSERVER_H


