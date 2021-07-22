#include "Server/includes/cpacket.h"
#include "cserver.h"

CPacket::CPacket()
{

}

CPacket::CPacket(QString type, QString action) :
m_type(type),m_action(action)
{
    m_client = NULL;
    Serialize();
}

CPacket::CPacket(const char *p, CClient *client)
{
    m_client = client;

    QByteArray ba(p);

    QJsonParseError jsonError;
    m_data = QJsonDocument::fromJson(p,&jsonError);

    if (jsonError.error != QJsonParseError::NoError){
#ifdef PACKET_DEBUG
        qDebug() << "[PACKET] JSON PARSE ERROR" << jsonError.errorString();
#endif
    }

    m_obj = m_data.object();

    //Deserialize request Type & Action
    Deserialize();

#ifdef PACKET_DEBUG
    qDebug() << "[PACKET] RECEIVE - " << m_type << " " << m_action;
#endif
}

//Not used anymore
CPacket::CPacket(QByteArray data, CClient * client){
    m_client = client;                                  //Client

    m_data = QJsonDocument::fromJson(data);             //JSON Doc
    m_obj = m_data.object();

    //Set type & action from m_data
    Deserialize();
}

void CPacket::Deserialize(){

    if(m_obj.contains("mainObj"))
    {
            QJsonObject mainObj = m_obj.value("mainObj").toObject();
            QJsonValue type = mainObj.value("type");
            QJsonValue action = mainObj.value("action");
            m_type = type.toString();
            m_action = action.toString();
    }
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

QJsonDocument CPacket::GetData()
{
    return m_data;
}

QByteArray CPacket::GetByteArray()
{
    QJsonDocument doc(m_obj);

    QDataStream ds(&m_ba, QIODevice::ReadWrite);
    QByteArray tmp = doc.toJson();
    ds << (qint32)tmp.size();
    ds << tmp;

#ifdef PACKET_DEBUG
    qDebug() << "[PACKET] OUTGOING - " << m_type << " " << m_action;
#endif
    return m_ba;
}

QUuid CPacket::get_IdClient()
{
    return id_client;
}

int CPacket::get_IdChannel()
{
    return id_channel;
}

/**
 * @brief This function serialize mainObj (type & action)
 */
void CPacket::Serialize()
{
    QJsonObject mainObj;

    mainObj.insert("type", m_type);
    mainObj.insert("action", m_action);

    m_obj["mainObj"] = mainObj;
}

/**
 * @brief This function serialize CServer object informations
 */
void CPacket::Serialize(CServer* c)
{
      QJsonObject obj;
      QJsonArray cArray, sArray;
      QJsonObject mainObj;

      //Serialize request type and action
      m_type="-1";
      m_action="-1";
      Serialize();

      //Serialize channels
      foreach(CChannel * c, c->get_channelList())
      {
          cArray.append(c->serializeToObj());
      }

      //Serialize clients
      foreach(CClient * c, c->get_clientList())
      {
         sArray.append(c->serializeToObj());
      }

      m_obj["channels"] = cArray;
      m_obj["clients"] = sArray;
}

//When a new client connected
void CPacket::Serialize_newClient(CClient* client,bool sendPP)
{

   QJsonObject clientObj;
   clientObj.insert("uuid", client->get_uuid().toString());
   clientObj.insert("pseudo", client->get_pseudo());
   clientObj.insert("isOnline", client->get_isOnline());
   clientObj.insert("description", client->get_description());

   if(!client->get_profilePic().isNull() && sendPP)
   {
        QByteArray array;
        QBuffer buffer(&array);
        client->get_profilePic().save(&buffer, "PNG");

        clientObj.insert("pp",QString::fromLatin1(array.toBase64()));
   }

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

void CPacket::Serialize_myClient(CClient * client)
{
    QJsonObject clientObj;
    clientObj.insert("uuid", client->get_uuid().toString());
    clientObj.insert("pseudo", client->get_pseudo());
    clientObj.insert("isOnline", client->get_isOnline());
    clientObj.insert("description", client->get_description());

    if(!client->get_profilePic().isNull())
    {
         QByteArray array;
         QBuffer buffer(&array);
         client->get_profilePic().save(&buffer, "PNG");

         clientObj.insert("pp",QString::fromLatin1(array.toBase64()));
    }

    m_obj["myClient"] = clientObj;
}

CClient * CPacket::Deserialize_newClient()
{
    try{
        if(m_obj.contains("newClient"))
        {
            QString name;
            QUuid id;
            bool isOnline;
            QString description;

            QJsonObject newClient = m_obj.value("newClient").toObject();
            id = QUuid::fromString(newClient.value("uuid").toString());
            name = newClient.value("pseudo").toString();
            isOnline = newClient.value("isOnline").toBool();
            description = newClient.value("description").toString();

            CClient * client = new CClient(id,name,NULL, -1,isOnline, description);

            if(newClient.contains("pp"))
            {
                QByteArray array = QByteArray::fromBase64(newClient.value("pp").toString().toLatin1());
                QImage tmp;
                tmp.loadFromData(array);
                client->set_profilePic(tmp);
            }
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
        if(m_obj.contains("newChannel"))
        {
            QString name;
            int id, maxUsers;

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
        if(m_obj.contains("myClient"))
        {
            QString name,description;
            QUuid id;
            bool isOnline;

            QJsonObject myClient = m_obj.value("myClient").toObject();
            id = QUuid::fromString(myClient.value("uuid").toString());
            name = myClient.value("pseudo").toString();
            isOnline = myClient.value("isOnline").toBool();
            description = myClient.value("description").toString();

            CClient * client = new CClient(id,name,NULL, -1,isOnline, description);
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

QList<QString> CPacket::Deserialize_regReq()
{
    QList<QString> null;
    null.push_back("null");
    try {
        if(m_obj.contains("newReg"))
        {
            QList<QString> res;
            QJsonObject newReg = m_obj.value("newReg").toObject();
            res.push_back(newReg.value("username").toString());
            res.push_back(newReg.value("mail").toString());
            res.push_back(newReg.value("password").toString());
            res.push_back(newReg.value("password_confirm").toString());
        }

    }  catch (char* e) {
        qDebug() << "Error in Deserialize_regReq :" << e << Qt::endl;
        return null;
    }
    return null;
}

void CPacket::Serialize_regAns(int code)
{
    QJsonObject regAnsObj;
    regAnsObj.insert("code",code);
    m_obj["ansReg"] = regAnsObj;
}

void CPacket::Serialize_Message(CMessage msg)
{
    QJsonObject sendMsg;
    if(msg.get_xmlmessage().isNull())
    {
        msg.toXML();
    }
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

void CPacket::Serialize_MessageListInfo(int index)
{
    QJsonObject msgInfo;
    msgInfo.insert("index",index);
    m_obj["msgInfo"] = msgInfo;
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
                int id = msgReq.value("id").toInt();
                res.append("public");
                res.append(QString::number(id));
                res.append(QString::number(msgReq.value("nb").toInt()));
                res.append(QString::number(msgReq.value("start").toInt()));
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

void CPacket::Serialize_MessageError(int code)
{
    QJsonObject msgErr;
    msgErr.insert("code", code);

    m_obj["msgErr"] = msgErr;
}

void CPacket::Serialize_ppAnswer(QImage pp, QUuid uuid)
{
    QJsonObject ppAns;
    ppAns.insert("uuid",uuid.toString());

    QByteArray array;
    QBuffer buffer(&array);
    pp.save(&buffer, "PNG");
    ppAns.insert("img",QString::fromLatin1(array.toBase64()));

    m_obj["ppAns"] = ppAns;
}

QString CPacket::deserialize_ppRequest()
{
    QString err;
    try
    {
        if(m_obj.contains("ppReq"))
        {
            QJsonObject ppReq = m_obj.value("ppReq").toObject();
            return ppReq.value("uuid").toString();
        }
        return "Error while trying to deserialize profile picture request, no correct field \"ppReq\" found.";
    }
    catch (char *e)
    {
        err.append(e);
        return err;
    }
}

//UI
