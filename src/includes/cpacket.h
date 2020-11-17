#ifndef CPACKET_H
#define CPACKET_H

#include "cclient.h"
#include "cchannel.h"
#include "cserver.h"
#include <QString>
#include <QJsonArray>


class CPacket
{
public:
    CPacket();

    //Getters
    char GetType();
    char GetAction();
    CClient GetSender();

    //Setters
    void SetType(char p_type);
    void SetAction(char p_action);

    //Text<-->code
    QString TypeDecode(char p_type);
    QString ServerDecode(char p_action);
    QString ChannelDecode(char p_action);
    QString UserDecode(char p_action);

    char TypeEncode(QString p_type);
    char ServerEncode(QString p_action);
    char ChannelEncode(QString p_action);
    char UserEncode(QString p_action);

    //Serialiaze
    QByteArray Serialize();


    //Deserialize
    void Deserialize(QByteArray in);
    CClient DeserializeClient(QJsonArray  in);
    CChannel DeserializeChannel(QJsonArray  in);
    CServer DeserializeServer(QJsonArray  in);

private:
    char m_type;
    char m_action;
    CClient m_client;
    QJsonDocument m_data;
};

#endif // CPACKET_H
