/**
 * @file        cpacket.h
 * @brief       Declaration of the class Server CPacket
 * @details     This class used for LockVox protocol
 * @author      LockVox Team
 * @version     0.1.1
 * @date        2021
 * @copyright   GNU GPL-3.0
 */

#ifndef CPACKET_H
#define CPACKET_H

#include "cmessage.h"
#include "cclient.h"
#include "cchannel.h"

#include <QString>
#include <QList>

class CServer;
class CChannel;

enum PacketClass {
    NOTIFY=0,                                    //!< Use when data is send without being query from other side
    INFO=1,                                      //!< Use to request Data to other side
    ACTION=2,                                    //!< Request action on other side
    CHANGE=3
};

enum Request{
    REQ,ANS
};

enum Type{
    //SERVER=1,
    //CHANNELS=1,
    //USERS=2,
    //MESSAGES=3,
    //CONF=4,
};

//Server
enum Server{
    //NOTIFY
    SHUTDOWN,
    //INFO
    PARAMS              =0100, 
    OPTIONS             =0101,
    SERVER_PP           =0103,

    //ACTIONS
    AUTH                =0201,                //!< params : Auth Info   
    REG                 =0202                 //!< params : reg info

};

//Channel
enum Channel{
    //NOTIFY
    JOIN_CONF           =1000,                        
    QUIT_CONF           =1001,
    
    //INFO
    CHAN                =1100,

    //ACTION
    JOIN                =1200,
    QUIT                =1201,

};

//User
enum User{
    //NOTIFY 
    REGISTER            =2000,
    CONNECT             =2001,                    //!< User connected
    DISCONNECT          =2002,                    //!< User disconnected
    CHANGE_USERNAME     =2003,
    CHANGE_DESCRIPTION  =2004,
    CHANGE_PP           =2005,
    //INFO
    INFO_USER                =2100,
    //ACTION
    UPDATE_USERNAME     =2201,
    UPDATE_DESCRIPTION  =2202,
    UPDATE_MAIL         =2203,
    UPDATE_PASSWORD     =2204,
};

enum Message{


};

enum Conf{
    //NOTIFY
    CREATE,
    DELETE,
    //INFO
  /*CONF
    //ACTION
    CREATE,
    DELETE,
    JOIN,
    QUIT,
    KICK_USER,*/
};

enum reqState{
    ERROR=0,
    SUCCESS=1
};


class CPacket
{
    /**
     * @class       CPacket cpacket.h "cpacket.h"
     * @brief       Declaration of the class Server CPacket
     * @details     CPacket are used to discuss between client and server
     * @author      LockVox Team
     * @version     0.1.1
     * @date        2021
     * @note
     * @bug
     * @warning
     * @remark
     * @copyright   GNU GPL-3.0
     */

    public:
        CPacket();
        CPacket(QByteArray data, CClient * client);
        CPacket(const char *p, CClient * client);
        CPacket(QString type, QString action);

        //Getters
        int GetId();
        QString GetType();
        QString GetAction();
        QJsonDocument GetData();
        QByteArray GetByteArray();

        //Setters
        void SetId(int id); 
        void SetType(QString p_type);
        void SetAction(QString p_action);

        //Serialiaze
        void Serialize();                                            //Server mainObj - Type & Action
        void Serialize(CServer* c);                            //Serialize Object Server

        //Serialize specific object -
        void Serialize_newClient(CClient * client, bool sendPP);                  //Serialize Client object ( Pseudo & ID & isOnline & ...)
        void Serialize_newChannel(CChannel * channel);               //Serialize Channel object ( Name & ID & MaxUsers=
        void Serialize_myClient(CClient * client);

        void Serialize_auth(CClient* info, int code);
        void Serialize_ID(int chan, QUuid client);                     //Serialize ID Object (ID chan & ID client)
        void Serialize_regReq(QString username, QString mail, QString password,QString password_confirm);
        void Serialize_regAns(int code);
        void Serialize_MessageListInfo(int index);
        void Serialize_Message(CMessage msg);
        void Serialize_MessageList(QList<CMessage> list);
        void Serialize_MessageError(int code);
        void Serialize_ppAnswer(QImage pp,QUuid uuid);


        QString getCookie() const;
        void setCookie(const QString &cookie);

        PacketClass getClass();
        void setClass(PacketClass c);

        void Deserialize();


        //Deserialize specific object
        CClient * Deserialize_myClient();
        CClient * Deserialize_newClient();
        CChannel * Deserialize_newChannel();

        QList<QString> Deserialize_auth();
        void Deserialize_ID();                                       //Deerialize ID Object (ID chan & ID client)
        QList<QString> Deserialize_regReq();
        CMessage Deserialize_Message();
        QList<CMessage> Deserialize_MessageList();
        QList<QString> deserialize_messageRequest();
        QString deserialize_ppRequest();

        void Serialize_Register(struct register_info reg);           //Serialize Register
        struct register_info Deserialize_Register();                 //Deserialize Register
        //To remove
        QUuid get_IdClient();
        int get_IdChannel();




private:

        int m_id;
        //Type & Action corresponding to the request

        PacketClass m_class;                                       //!Notify  - Info - Action
        Request m_req;                                         //

        QString m_type;
        QString m_action;

        //Client who send the request
        CClient * m_client;
        QString m_cookie;

        //Destinataire
        CClient * m_receiver;                               //!< Who the packet is address to


        //If ID object exist - on call Deserialize_ID();
        //QUuid id_client;
        //int id_channel;

        //Use for serialization / Deserialization
        QJsonObject m_obj;                                   //!< JSON Object 
        QJsonDocument m_data;                                //!< JSON Document
        QByteArray m_ba;                                     //!< Byte Array
};

#endif // CPACKET_H
