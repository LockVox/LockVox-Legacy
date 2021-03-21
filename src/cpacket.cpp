#include "src/includes/cpacket.h"

CPacket::CPacket()
{

}

CPacket::CPacket(QString type, QString action) :
m_type(type),m_action(action)
{
    m_client = NULL;
    Serialize();
}

CPacket::CPacket(QByteArray data, CClient * client){
    m_client = client;                                  //Client

    m_data = QJsonDocument::fromJson(data);             //JSON Doc
    m_obj = m_data.object();

    //Set type & action from m_data
    Deserialize();
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

void CPacket::Serialize()
{
    QJsonObject mainObj;

    mainObj.insert("type", m_type);
    mainObj.insert("action", m_action);

    m_obj["mainObj"] = mainObj;
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
   //qDebug() << jsonDoc;


   m_ba = jsonDoc.toJson();
   return m_ba;
}

//When a new client connected
void CPacket::Serialize_newClient(CClient* client){

   QJsonObject clientObj;
   clientObj.insert("uuid", client->get_uuid().toString());
   clientObj.insert("pseudo", client->get_pseudo());
   clientObj.insert("isOnline", client->get_isOnline());
   clientObj.insert("description", client->get_description());
   m_obj["newClient"] = clientObj;
}

//When a new channel is created
void CPacket::Serialize_newChannel(CChannel* channel){

    QJsonObject channelObj;
    channelObj.insert("id", channel->get_id());
    channelObj.insert("name", channel->get_name());
    channelObj.insert("maxUsers", channel->get_maxUsers());

    m_obj["newChannel"] = channelObj;
}

void CPacket::Serialize_myClient(CClient * client){
    QJsonObject clientObj;
    clientObj.insert("uuid", client->get_uuid().toString());
    clientObj.insert("pseudo", client->get_pseudo());
    clientObj.insert("isOnline", client->get_isOnline());
    clientObj.insert("description", client->get_description());
    clientObj.insert("uuid", client->get_uuid().toString());
    m_obj["myClient"] = clientObj;
}

void CPacket::Deserialize(){

    //qDebug() << m_obj;

    if(m_obj.contains("mainObj")){
            QJsonObject mainObj = m_obj.value("mainObj").toObject();
            QJsonValue type = mainObj.value("type");
            QJsonValue action = mainObj.value("action");
            m_type = type.toString();
            m_action = action.toString();

            //qDebug() << "m_type = " << m_type;
            //qDebug() << "m_action = " << m_action;

        }
}

CClient * CPacket::Deserialize_newClient(){
    try{
        QString name;
        QUuid id;
        bool isOnline;
        QString description;

        if(m_obj.contains("newClient")){
            QJsonObject newClient = m_obj.value("newClient").toObject();
            id = QUuid::fromString(newClient.value("uuid").toString());
            name = newClient.value("pseudo").toString();
            isOnline = newClient.value("isOnline").toBool();
            description = newClient.value("description").toString();

            CClient * client = new CClient(id,name,NULL, -1,isOnline, description);
            //qDebug() << "Name " << name << "   ID " << id;

            return client;
        }
      }
    catch(char* e)
    {
        qDebug() << "Error in deserialize newClient : " << e << Qt::endl;
    }

    return NULL;
}

CChannel * CPacket::Deserialize_newChannel(){
    try{
        QString name;
        int id, maxUsers;

        if(m_obj.contains("newChannel")){
            QJsonObject newClient = m_obj.value("newChannel").toObject();
            id = newClient.value("id").toInt();
            name = newClient.value("name").toString();
            maxUsers = newClient.value("maxUsers").toInt();

            CChannel * channel = new CChannel(name,id,maxUsers);
            return channel;
        }
    }
    catch(char* e)
    {
        qDebug() << "Error in deserialize newChannel : " << e << Qt::endl;
        return NULL;
    }
    return NULL;
}

CClient * CPacket::Deserialize_myClient(){
    try{
        QString name,description;
        QUuid id;
        bool isOnline;

        if(m_obj.contains("myClient")){
            QJsonObject myClient = m_obj.value("newClient").toObject();
            id = QUuid::fromString(myClient.value("uuid").toString());
            name = myClient.value("pseudo").toString();
            isOnline = myClient.value("isOnline").toBool();
            description = myClient.value("description").toString();

            CClient * client = new CClient(id,name,NULL, -1,isOnline, description);
            //qDebug() << "Name " << name << "   ID " << id;
            return client;
        }
      }
    catch(char* e)
    {
        qDebug() << "Error in deserialize newClient : " << e << Qt::endl;
    }
    return NULL;
}

void CPacket::Serialize_ID(int chan, QUuid client){

    QJsonObject channelObj;
    channelObj.insert("id_channel", chan);
    channelObj.insert("id_client", client.toString());

    m_obj["id"] = channelObj;

}

void CPacket::Deserialize_ID(){
    try{
        QJsonObject id = m_obj.value("id").toObject();
        id_channel = id.value("id_channel").toInt();
        id_client = QUuid::fromString(id.value("id_client").toString());
    }
    catch(char* e)
    {
        qDebug() << "Error in deserializeID : " << e << Qt::endl;
    }
}

void CPacket::Serialize_auth(CClient* info, int code)
{
    QJsonObject authObj;
    switch(code)
    {
    case 0:{
        authObj.insert("code", code);
        authObj.insert("uuid", info->get_uuid().toString());
        authObj.insert("pseudo", info->get_pseudo());
        authObj.insert("isOnline", info->get_isOnline());
        authObj.insert("description", info->get_description());
        break;
        }
    case 1:{
        authObj.insert("code", code);
        authObj.insert("reason", "user does not exist");
        break;
        }
    case 2:{
        authObj.insert("code", code);
        authObj.insert("reason", "user already connected");
        break;
        }
    case 3:{
        authObj.insert("code", code);
        authObj.insert("reason", "bad password");
        break;
        }
    }
    m_obj["newAuth"] = authObj;
}

QList<QString> CPacket::Deserialize_auth()
{
    QList<QString> null; //null Qlist in case of error
    null.append("null");
    try {
        QList<QString> info;
        if(m_obj.contains("newAuth"))
        {
            QJsonObject newAuth = m_obj.value("newAuth").toObject();
            info.push_back(newAuth.value("email").toString());
            info.push_back(newAuth.value("pass").toString());
            return info;
        }
        return null;
    }
    catch(char* e)
    {
        qDebug() << "Error in Deserialize_auth :" << e << Qt::endl;
    }
    return null;
}

void CPacket::Serialize_authReq(QString email, QString pass)
{
    QJsonObject credsObj;
    credsObj.insert("email", email);
    credsObj.insert("pass", pass);
    m_obj["newAuth"] = credsObj;
}

CClient* CPacket::Deserialize_authAns()     //Retourne NULL ou un client vide avec erreur en description
{
    try{
        int code;
        QString err;
        if(m_obj.contains("newAuth"))
        {
            QJsonObject newAuth = m_obj.value("newAuth").toObject();
            code = newAuth.value("code").toInt();
            switch(code)
            {
                case 0:
                {
                    CClient* tmp = new CClient(QUuid::fromString(newAuth.value("uuid").toString()), newAuth.value("pseudo").toString(), NULL, -1,newAuth.value("isOnline").toBool(), newAuth.value("description").toString());
                    return tmp;         //On renvoie les infos client
                    break;
                }

                case 1:
                {
                    err = newAuth.value("reason").toString();
                    CClient* error = new CClient(NULL, "NULL", NULL, -1, false, err);    //On renvoie l'erreur par la description
                    return error;
                    break;
                }

                case 2:
                {
                    err = newAuth.value("reason").toString();
                    CClient* error = new CClient(NULL, "NULL", NULL, -1, false, err);    //On renvoie l'erreur par la description
                    return error;
                    break;
                }

                case 3:
                {
                    err = newAuth.value("reason").toString();
                    CClient* error = new CClient(NULL, "NULL", NULL, -1, false, err);    //On renvoie l'erreur par la description
                    return error;
                    break;
                }

                default:
                {
                    return NULL;    //bad packet
                    break;
                }
            }
        }
    }
    catch(char* e)
    {
        qDebug() << "Error in Deserialize_authAns : " << e << Qt::endl;
        return NULL;
    }
    return NULL;
}

void CPacket::Serialize_regReq(QString username, QString mail, QString password,QString password_confirm)
{
    QJsonObject regObj;
    regObj.insert("username", username);
    regObj.insert("mail", mail);
    regObj.insert("password", password);
    regObj.insert("password_confirm", password_confirm);

    m_obj["newReg"] = regObj;
}

void CPacket::Deserialize_regReq()
{
    try {
        if(m_obj.contains("newReg"))
        {
            QJsonObject newReg = m_obj.value("newReg").toObject();
            m_register.name = newReg.value("username").toString();
            m_register.email = newReg.value("mail").toString();
            m_register.password = newReg.value("password").toString();
            m_register.password_confirm = newReg.value("password_confirm").toString();
        }

    }  catch (char* e) {
        qDebug() << "Error in Deserialize_regReq :" << e << Qt::endl;
        QList<QString> err;
        err.push_back("0");
    }
}

void CPacket::Serialize_regAns(int code)
{
    QJsonObject regAnsObj;
    regAnsObj.insert("code",code);
    m_obj["ansReg"] = regAnsObj;
}

int CPacket::Deserialize_regAns()
{
    try {
        int code = NULL;
        if(m_obj.contains("ansReg"))
        {
            QJsonObject ansReg = m_obj.value("ansReg").toObject();
            code = ansReg.value("code").toInt();
        }
        return code;

    }  catch (char* e) {
        qDebug() << "Error in Deserialize_regAns :" << e << Qt::endl;
        return NULL;
    }
}

void CPacket::Serialize_Message(CMessage msg)
{
    QJsonObject sendMsg;
    msg.toXML();
    if(msg.get_xmlmessage().isNull())
    {
        qDebug() << "Why would you send an empty message ?" << Qt::endl;
    }
    else
    {
        sendMsg.insert("xml", msg.toString());
        m_obj["message"] = sendMsg;
    }
}

CMessage CPacket::Deserialize_Message()
{
    CMessage null("null","null","null",false); //null msg in case of error
    try
    {
        if(m_obj.contains("message"))
        {
            QJsonObject sendMsg = m_obj.value("message").toObject();
            CMessage msg(sendMsg.value("xml").toString());
            return msg;
        }

        return null;
    }
    catch (char* e)
    {
        qDebug() << "Error in Deserialize_Message :" << e << Qt::endl;
    }
    return null;
}

void CPacket::Serialize_MessageList(QList<CMessage> list)
{
    int index = 0;
    QJsonArray msglist;
    foreach(CMessage m, list)
    {
        m.toXML();
        if(m.get_xmlmessage().isNull())
        {
            qDebug() << "Empty message ignored" << Qt::endl;
        }
        else
        {
            QJsonObject msg;
            msg.insert("xml",m.toString());
            QJsonObject tmp;
            QString indexstr = QString::number(index);
            tmp[indexstr] = msg;
            msglist.append(tmp);
            index++;
        }
    }
    m_obj["messagelist"] = msglist;
}

QList<CMessage> CPacket::Deserialize_MessageList()
{
    QList<CMessage> null;
    null.append(CMessage("null","null","null",true));
    QList<CMessage> list;
    int index = 0;

    try
    {
        if(m_obj.contains("messagelist"))
        {
            QJsonArray msglist = m_obj.value("messagelist").toArray();

            while(!msglist.isEmpty())
            {
                QJsonObject tmp = msglist.first().toObject();
                QString indexstr = QString::number(index);
                if(tmp.contains(indexstr))
                {
                    QJsonObject tmp0 = tmp.value(indexstr).toObject();

                    if(tmp0.contains("xml"))
                    {
                        CMessage tmpmsg(tmp0.value("xml").toString());
                        tmpmsg.toXML();
                        list.append(tmpmsg);
                    }
                    msglist.removeFirst();
                    index++;
                    indexstr = QString::number(index);
                }
            }
        }
        else
        {
            qDebug() << "No message list" << Qt::endl;
        }
    }
    catch (char *e)
    {
        qDebug() << "Error in Deserialize_MessageList :" << e << Qt::endl;
    }
    return null;
}

void CPacket::Serialize_messageRequest(int id, int nb_msg_to_sync, int start_index)
{
    QJsonObject msgReq;
    msgReq.insert("id",id);
    msgReq.insert("nb",nb_msg_to_sync);
    msgReq.insert("start", start_index);

    m_obj["msgReq"] = msgReq;
}

void CPacket::Serialize_messageRequest(QUuid id, int nb_msg_to_sync, int start_index)
{
    QJsonObject msgReq;
    msgReq.insert("uuid",id.toString(QUuid::WithoutBraces));
    msgReq.insert("nb",nb_msg_to_sync);
    msgReq.insert("start", start_index);

    m_obj["msgReq"] = msgReq;
}

QList<QString> CPacket::deserialize_messageRequest()
{
    QList<QString> null;
    null.append("null");
    try
    {
        if(m_obj.contains("msgReq"))
        {
            QJsonObject msgReq = m_obj.value("msgReq").toObject();
            if(msgReq.contains("uuid"))
            {
                QList<QString> res;
                res.append("private");
                res.append(msgReq.value("uuid").toString());
                res.append(msgReq.value("nb").toString());
                res.append(msgReq.value("start").toString());
                return res;
            }
            else
            {
                QList<QString> res;
                res.append("public");
                res.append(msgReq.value("id").toString());
                res.append(msgReq.value("nb").toString());
                res.append(msgReq.value("start").toString());
                return res;
            }
        }
        return null;
    }
    catch (char* e)
    {
        qDebug() << "Error in deserialize_messageRequest : " << e << Qt::endl;
        return null;
    }
}

//UI
