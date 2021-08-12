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

#include "LVObject.h"
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
    REQ=1,
    ANS=2,
    NONE,
};

enum ReqState{
    ERROR=0,
    SUCCESS=1,
    NO_STATE=2
};


enum Type{
    _SERVER=0,
    _CHANNELS=1,
    _USERS=2,
    _MESSAGES=3,
    _CONF=4,
};

//Server
enum ServerAction{
    //NOTIFY
    SHUTDOWN,
    SYNC_INFO           =0001,
    PARAMS              =0100,
    OPTIONS             =0101,
    SERVER_PP           =0103,

    //ACTIONS
    AUTH                =0201,                //!< params : Auth Info
    REG                 =0202                 //!< params : reg info

};

//Channel
enum ChannelAction{
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
enum UserAction{
    CONNECT             =2001,                    //!< User connected
    DISCONNECT          =2002,                    //!< User disconnected
    CHANGE_USERNAME     =2003,
    CHANGE_DESCRIPTION  =2004,
    CHANGE_PP           =2005,
    //INFO
    INFO_USERJOIN_CONF                =2100,
    //ACTION
    UPDATE_USERNAME     =2201,
    UPDATE_DESCRIPTION  =2202,
    UPDATE_MAIL         =2203,
    UPDATE_PASSWORD     =2204,
};

enum MessageAction{


};

enum ConfAction{
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

struct sAuthentication{
    QString mail;
    QString password;
};

struct sRegister{
    QString username;
    QString mail;
    QString password;
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

        //Constructor - most of them are obsolete
        CPacket();
        CPacket(QByteArray data, CClient * client);
        CPacket(const char *p, CClient * client);
        CPacket(QString type, QString action);

        CPacket(PacketClass c, Type type, int action);
        CPacket(PacketClass c, Request req, ReqState state, Type type, UserAction action);
        CPacket(PacketClass c, Request req, ReqState state, Type type, ServerAction action);
        CPacket(PacketClass c, Request req, ReqState state, Type type, ChannelAction action);
        CPacket(PacketClass c, bool requireAns);


        //Serialization functions
        void Serialize();                                           //!< Serialize inherent informations of a packet.

        void serialize_param(QString str);
        void serialize_target();
        void serialize(CObject * lv_object);
        void serialize(QList<CObject*> list_objects);
        void serialize_auth(sAuthentication info);
        void serialize_reg(sRegister info);




        QList<CClient> getClientsFromList();
        QList<CChannel> getChannelsFromList();
        QList<CMessage> getMessagesFromList();

        QString deserialize_param();
        CObject* deserialize(CObject::ObjectType type);
        QList<CObject *> deserializeList(CObject::ObjectType type);
        sAuthentication deserialize_auth();
        sRegister deserialize_reg();

        //Getters
        int GetId();
        PacketClass getClass();
        Request getReq() const;
        ReqState getReqState() const;
        QString GetType();
        QString GetAction();
        QJsonDocument GetData();
        QByteArray GetByteArray();
        CClient *getSender();
        CClient *getReceiver();
        QString getCookie() const;

        //Setters
        void SetId(int id); 
         void setClass(PacketClass c);
        void setReq(const Request &req);
        void setReqState(const ReqState &reqState);
        void SetType(QString p_type);
        void SetAction(QString p_action); 
        void setSender(CClient * c);
        void setReceiver(CClient * c);
        void setCookie(const QString &cookie);
















        //Serialiaze
                                                    //Server mainObj - Type & Action
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


        Type getType() const;
        void setType(const Type &Type);

        int getAction() const;
        void setAction(int Action);

private:

        int m_id;
        //Type & Action corresponding to the request

        PacketClass m_class;                                   //!Notify  - Info - Action
        bool m_requireAns;                                     //Should this packet receive an answer

        ReqState m_reqState;                                   //!< Did request failed or success
        Request m_req;                                         //:< Is this a request or an answer

        Type m_Type;
        int m_Action;

        QString m_type;
        QString m_action;

        //Client who send the request
        CClient *m_sender;
        QString m_cookie;
        //Destinataire
        CClient *m_receiver;                               //!< Who the packet is address to

        QList<CObject*> m_targetObject;

        //Use for serialization / Deserialization
        QJsonObject m_obj;                                   //!< JSON Object 
        QJsonDocument m_data;                                //!< JSON Document
        QByteArray m_ba;                                     //!< Byte Array
};

#endif // CPACKET_H
