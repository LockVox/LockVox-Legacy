#ifndef CPACKET_H
#define CPACKET_H

#include "cclient.h"
#include "cchannel.h"
#include "Server/cserver.h"
#include <QString>
#include <QJsonArray>

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

    QByteArray GetByteArray(){
        return m_ba;
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
    //QByteArray Serialize();
    QByteArray Serialize(CServer* c);
    QByteArray Serialize(bool isActionValid);




    //Serialize by action -
    void Serialize_newClient(CClient * client);
    void Serialize_newChannel();






private:
    QString m_type;
    QString m_action;
    CClient * m_client;
    QJsonDocument m_data;
    QByteArray m_ba;
};

#endif // CPACKET_H
