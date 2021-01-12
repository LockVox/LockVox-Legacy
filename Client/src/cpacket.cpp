#include "src/includes/cpacket.h"

CPacket::CPacket()
{
    m_type = 0xFF;
    m_action = 0xFF;
}

CPacket::CPacket(QByteArray n_data, CClient *p_sender)
{
    SetType(n_data.at(0));
    SetType(n_data.at(1));
    n_data.remove(0, 1);
    m_client = *p_sender;
    m_data = QJsonDocument::fromJson(n_data);
    qDebug() << m_data << Qt::endl;
}

//Getters

char CPacket::GetType()
{
    return m_type;
}

char CPacket::GetAction()
{
    return m_action;
}

QJsonDocument CPacket::GetData()
{
    return m_data;
}
//Setters

void CPacket::SetType(char p_type)
{
    m_type = p_type;
}

void CPacket::SetAction(char p_action)
{
    m_action = p_action;
}

//Text<-->Code

QString CPacket::TypeDecode(char p_type)
{
    switch (p_type) {

    case 0: return "Server";
    case 1: return "Channel";
    case 2: return "User";
    default: return "Invalid code";
    }
}

QString CPacket::ServerDecode(char p_action)
{
    switch(p_action) {
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

QString CPacket::ChannelDecode(char p_action)
{
    switch (p_action) {
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

QString CPacket::UserDecode(char p_action)
{
    switch(p_action){
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

//Serialiaze

QByteArray CPacket::Serialize()
{
    QByteArray out;
    out.insert(0, m_type);
    out.insert(1, m_action);
    out.push_back(m_data.toJson());
    return out;
}

//Deserialize
void CPacket::Deserialize()
{

}


QList<CClient*> CPacket::DeserializeClient(QJsonArray  in)
{
    QList<CClient *> clients;
    foreach(const QJsonValue & value, in)
    {
        QJsonObject obj = value.toObject();
        CClient* newClient = new CClient(obj["name"].toString(), NULL, obj["id"].toInt());
        clients.push_back(newClient);
    }
    return clients;
}
CChannel CPacket::DeserializeChannel(QJsonArray  array)
{
    CChannel tmp;
    return tmp;
/*
    foreach( const QJsonValue & value, array){

        //Convert it to an json object then to a channel
        QJsonObject obj = (CChannel)value.toObject(value);
        CChannel * newChannel = new CChannel(obj);
        //qDebug() << "Channel : " << newChannel->get_id()<< newChannel->get_name()<< Qt::endl;

        //check if the channel already exist or not
        bool exist = false;
        //if the channel exist, we reload it with new value
        foreach(CChannel * c, get_channelList()){
            if(c->get_id() == newChannel->get_id()){
                 exist = true;
                c->set_all(newChannel);
            }
        }
        //if the channel doesnt exist, we add it to the list of channel
        if(get_channelList().isEmpty() || exist == false){
            qDebug() << "That channel doesnt exist, gonna create it " << Qt::endl;
            addChannel(newChannel);
        }

    }*/

}

/*CServer CPacket::DeserializeServer(QJsonArray in)
{
    CServer serv;
    return serv;
}
*/

//UI
