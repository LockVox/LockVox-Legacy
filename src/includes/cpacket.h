#ifndef CPACKET_H
#define CPACKET_H


#include "cclient.h"
#include "cchannel.h"
#include "Client/cserver.h"
#include <QString>


#include "src/includes/cchannel.h"
#include "src/includes/cclient.h"


class CServer;

class CPacket
{
public:
    CPacket();
    CPacket(QByteArray data, CClient * client);
    CPacket(QString action, QString type);

    //Getters
    QString GetType();
    QString GetAction();
    CClient GetSender();

    QJsonDocument GetData(){
        return m_data;
    }

    //Convert m_obj to a byte array
    QByteArray GetByteArray(){
        QJsonDocument doc(m_obj);
        qDebug() << doc;
        m_ba = doc.toJson();
        return m_ba;
    }

    int get_IdClient(){
        return id_client;
    }

    int get_IdChannel(){
        return id_channel;
    }


    //Setters
    void SetType(QString p_type);
    void SetAction(QString p_action);

    //Text<-->code
    QString TypeDecode(QString p_type);
    QString ServerDecode(QString p_action);
    QString ChannelDecode(QString p_action);
    QString UserDecode(QString p_action);

    char TypeEncode(QString p_type);
    char ServerEncode(QString p_action);
    char ChannelEncode(QString p_action);
    char UserEncode(QString p_action);

    //Serialiaze
    void Serialize();                                            //Server mainObj - Type & Action
    void Deserialize();                                          //Deserialize mainObj - Type & Action
    QByteArray Serialize(CServer* c);                            //Serialize Object Server
    QByteArray Serialize(bool isActionValid);                    //Serialize - Is Action request is valid or not


    //Serialize specific object -
    void Serialize_newClient(CClient * client);                  //Serialize Client object ( Pseudo & ID & isOnline & ...)
    void Serialize_newChannel(CChannel * channel);               //Serialize Channel object ( Name & ID & MaxUsers=

    //Deserialize specific object
    CClient * Deserialize_newClient();                           //Deserialize Client object
    CChannel * Deserialize_newChannel();                         //Deserialize Channel object

    void Serialize_ID(int chan, int client);                     //Serialize ID Object (ID chan & ID client)
    void Deserialize_ID();                                       //Deerialize ID Object (ID chan & ID client)







private:
    //Type & Action corresponding to the request
    QString m_type;
    QString m_action;

    //Client who send the request
    CClient * m_client;


    //If ID object exist - on call Deserialize_ID();
    int id_client;
    int id_channel;


    //Use for serialization / Deserialization
    QJsonObject m_obj;                                           //regroup all of the object that has been serialize

    //From Object to Byte Array
    QJsonDocument m_data;
    QByteArray m_ba;
};

#endif // CPACKET_H
