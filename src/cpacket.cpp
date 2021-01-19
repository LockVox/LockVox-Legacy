#include "src/includes/cpacket.h"

CPacket::CPacket()
{
    m_type = 0xFF;
    m_action = 0xFF;
}

CPacket::CPacket(QString action, QString type) :
m_action(action), m_type(type)
{
    m_client = NULL;
}



CPacket::CPacket(QByteArray data, CClient * client){
    m_client = client;                              //Client
    m_data = QJsonDocument::fromJson(data);         //JSON Doc

    qDebug() << m_data;

    //Set type & action from m_data
    m_type = m_data["type"].toString();
    m_action = m_data["action"].toString();
}

//Getters

QString CPacket::GetType()
{
    return m_type;
}

QString CPacket::GetAction()
{
    return m_action;
}

//Setters

void CPacket::SetType(QString p_type)
{
    m_type = p_type;
}

void CPacket::SetAction(QString p_action)
{
    m_action = p_action;
}

//Text<-->Code

QString CPacket::TypeDecode(QString p_type)
{
    switch (p_type.toInt()) {

    case 0: return "Server";
    case 1: return "Channel";
    case 2: return "User";
    default: return "Invalid code";
    }
}

QString CPacket::ServerDecode(QString p_action)
{
    switch(p_action.toInt()) {
    case 0: return "Connexion";
    case 1: return "Deconnexion";
    case 2: return "Nick";
    case 3: return "Description";
    case 4: return "Ban";
    case 5: return "IPBan";
    case 6: return "Kick";
    default: return "Invalid action";
    }
}

QString CPacket::ChannelDecode(QString p_action)
{
    switch (p_action.toInt()) {
    case 0: return "Connexion";
    case 1: return "Deconnexion";
    case 2: return "text_send";
    case 3: return "text_modif";
    case 4: return "text_del";
    case 5: return "chanv_create";
    case 6: return "chanv_del";
    case 7: return "chanv_rename";
    case 8: return "chanv_setmax";
    case 9: return "chanv_kick";
    case 10: return "chanv_mute";
    case 11: return "chant_create";
    case 12: return "chant_delete";
    case 13: return "chant_rename";
    default: return "Invalid action";
    }
}

QString CPacket::UserDecode(QString p_action)
{
    switch(p_action.toInt()){
    case 0: return "Mute";
    case 1: return "add_friend";
    case 2: return "del_friend";
    case 3: return "msg_friend";
    case 4: return "nick_mod";
    case 5: return "right_mod";
    default: return "Invalid action";
    }
}

char CPacket::TypeEncode(QString p_type)
{
    if(p_type == "Server")
        return 0;
    if(p_type == "Channel")
        return 1;
    if(p_type == "User")
        return 2;
    return 0xFF;
}

char CPacket::ServerEncode(QString p_action)
{
    if(p_action == "Connexion")
        return 0;
    if(p_action == "Deconnexion")
        return 1;
    if(p_action == "Nick")
        return 2;
    if(p_action == "Description")
        return 3;
    if(p_action == "Ban")
        return 4;
    if(p_action == "IPBan")
        return 5;
    if(p_action == "Kick")
        return 6;
    return 0xFF;
}

char CPacket::ChannelEncode(QString p_action)
{
    if(p_action == "Connexion")
        return 0;
    if(p_action == "Deconnexion")
        return 1;
    if(p_action == "text_send")
        return 2;
    if(p_action == "text_modif")
        return 3;
    if(p_action == "text_del")
        return 4;
    if(p_action == "chanv_create")
        return 5;
    if(p_action == "chanv_del")
        return 6;
    if(p_action == "chanv_rename")
        return 7;
    if(p_action == "chanv_setmax")
        return 8;
    if(p_action == "chanv_kick")
        return 9;
    if(p_action == "chanv_mute")
        return 10;
    if(p_action == "chant_create")
        return 11;
    if(p_action == "chant_delete")
        return 12;
    if(p_action == "chant_rename")
        return 13;
    return 0xFF;
}

char CPacket::UserEncode(QString p_action)
{
    if(p_action == "Mute")
        return 0;
    if(p_action == "add_friend")
        return 1;
    if(p_action == "del_friend")
        return 2;
    if(p_action == "msg_friend")
        return 3;
    if(p_action == "nick_mod")
        return 4;
    if(p_action == "right_mod")
        return 5;
    return 0xFF;
}



QByteArray CPacket::Serialize(CServer* c){
      m_ba = c->Serialize();
      return m_ba;
}


//Answer to a client request
QByteArray CPacket::Serialize(bool isActionValid){
   //Create a JSON Document with m_type & m_action
   QJsonObject obj;

   obj.insert("type", m_type);
   obj.insert("action", m_action);
   obj.insert("isActionValid", isActionValid);

   QJsonDocument jsonDoc(obj);
   qDebug() << jsonDoc;

   m_ba = jsonDoc.toJson();
   return m_ba;
}



void CPacket::Serialize_newClient(CClient* client){
   QJsonObject clientObj;
   clientObj.insert("id", client->get_id());
   clientObj.insert("pseudo", client->get_pseudo());
   clientObj.insert("isOnline", client->get_isOnline());

   //m_data.insert(clientObj);
}


void Serialize_newChannel(CChannel* channel){

}






//UI
