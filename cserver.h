#ifndef CSERVER_H
#define CSERVER_H

#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QRegExp>


#include "cchannel.h"
#include "cclient.h"

class CClient;
class CChannel;

class CServer : public QWidget
{
    Q_OBJECT

    public:
        CServer();


        //Création d'un channel grâce à un nouveau process -
        void create_process();
        void del_process();



        int addChannel(CChannel * tmp);
        int DelChannel(CChannel * tmp);
        int DelChannel(int id);


        QList<CChannel*> m_channels;


        void FillDataStreamFromClient(QDataStream & ds);
        void FillDataStreamFromChannel(QDataStream & ds);




        void FillClientFromDataStream(QDataStream & ds);
        void FillChannelFromDataStream(QDataStream & ds);

        void SendObjectsToClient();
        void receiveData();



        //Getters
        QTcpSocket * get_socket();



        //Setters

        void set_socket(QTcpSocket* soc);



    private slots:

        void nouvelleConnexion();           //Add client to the server - default no channel
        void donneesRecues();               //get data
        void deconnexionClient();           //Disconnecting client - del client from channel list - del client

        void connect();
        void disconnect();

        void errorTcpSocket(QAbstractSocket::SocketError error);


    private:
        QLabel *etatServeur;                //State of the server

        QTcpSocket * m_socket;

        QPushButton *boutonQuitter;


        quint16 tailleMessage;


                //List of channels
        QList<CClient *> m_clients;         //List of client





};

#endif // CSERVER_H


