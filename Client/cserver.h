#ifndef CSERVER_H
#define CSERVER_H

#include <QtWidgets>
#include <QtNetwork>
#include <QDebug>
#include <QRegExp>
#include <QUuid>

#include "src/includes/AbstractServer.h"

#include "src/includes/cclient.h"
#include "src/includes/cchannel.h"
#include "src/includes/audioinput.h"
#include "src/includes/audiooutput.h"

#include "src/includes/cpacket.h"




class CClient;
class CChannel;

class CServer : public AbstractServer
{
    Q_OBJECT
    public:

        CServer();

        //Getters
        QTcpSocket * get_socket();

        CClient * get_self(){
            return m_self;
        }

        //Setters
        void set_socket(QTcpSocket* soc);



        //Process
        void processIncomingData(QByteArray data);         //Process incoming data



        //Server action - To develop
        void changeChannel(int id);
        void quitChannel(int id);
        void joinChannel(int id);

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


    signals:
        void changeState(int newState);
        void on_Authentification(int newValue);

        void updateMainWindow();
    public slots:
        void RequestServer(int type, int action, CClient * client, CChannel * chan);
        bool Login(QString mail, QString passwd);               //Requests the server to authenticate

        bool Register(QString username, QString mail, QString password);

    private slots:

        void onReceiveData();
        void sendToServer(QByteArray ba);
        void sendToServer();

    private:
        //Client mode
        QTcpSocket * m_socket;
        CClient* m_self;

};

#endif // CSERVER_H
