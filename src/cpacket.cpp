#include "src/includes/cpacket.h"
#include "cserver.h"

CPacket::CPacket()
{

}

CPacket::CPacket(QString type, QString action) :
m_type(type),m_action(action)
{
    m_sender = NULL;
    Serialize();
}

CPacket::CPacket(PacketClass c, Request req, ReqState state, Type type, UserAction action)
{
    m_class = c;
    m_req = req;
    m_reqState = state;
    m_type = type;
    m_action = action;
}

CPacket::CPacket(PacketClass c, Request req, ReqState state, Type type, ChannelAction action)
{
    m_class = c;
    m_req = req;
    m_reqState = state;
    m_type = type;
    m_action = action;
}

CPacket::CPacket(PacketClass c, Request req, ReqState state, Type type, ServerAction action)
{
    m_class = c;
    m_req = req;
    m_reqState = state;
    m_type = type;
    m_action = action;
}


CPacket::CPacket(const char *p, CClient *client)
{
    m_sender = client;

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

CPacket::CPacket(QByteArray data, CClient * client){
    m_sender = client;                                  //Client

    m_data = QJsonDocument::fromJson(data);             //JSON Doc
    m_obj = m_data.object();

    //Set type & action from m_data
    Deserialize();
}

QString CPacket::getCookie() const
{
    return m_cookie;
}

void CPacket::setCookie(const QString &cookie)
{
    m_cookie = cookie;
}

CClient *CPacket::getReceiver()
{
    return m_receiver;
}

CClient *CPacket::getSender()
{
    return m_sender;
}

void CPacket::setSender(CClient *client)
{
    m_sender = client;
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

/**
 * @brief This function serialize mainObj (type & action)
 */
void CPacket::Serialize()
{
    QJsonObject mainObj;

    m_obj.insert("pCookie", m_cookie);
    m_obj.insert("pClass", m_class);

    mainObj.insert("type", m_type);
    mainObj.insert("action", m_action);
    m_obj["mainObj"] = mainObj;

}

void CPacket::serialize_param(QString str)
{
   QJsonObject params;
   params.insert("val", str);
}

void CPacket::serialize(CObject *lv_object)
{
    QJsonObject json_obj;
    json_obj = lv_object->toJSON();
    m_obj["object"] = json_obj;
}

void CPacket::serialize(QList<CObject *> list_objects)
{
    QJsonObject json_obj;
    foreach(CObject * obj, list_objects){
        json_obj.insert("object",obj->toJSON());
    }
    m_obj["list_objects"] = json_obj;
}

void CPacket::serialize_auth(sAuthentication info)
{
    QJsonObject auth_obj;

    auth_obj.insert("mail", info.mail);
    auth_obj.insert("password", info.password);

    m_obj["authentication"] = auth_obj;
}

void CPacket::serialize_reg(sRegister info)
{
    QJsonObject reg_obj;
    reg_obj.insert("username", info.username);
    reg_obj.insert("mail", info.mail);
    reg_obj.insert("password", info.password);

    m_obj["register"] = reg_obj;
}

QList<CClient> CPacket::getClientsFromList()
{
      QList<CClient> list;
      CClient tmp;
      QJsonArray cArray = m_obj["list_objects"].toArray();
      foreach(QJsonObject json_obj, cArray{
            if(json_obj["type"] == ObjectType::CLIENT){
                tmp.fromJSON(json_obj);
                list.push_back(tmp);
            }
      }
      return list;
}

QList<CChannel> CPacket::getChannelsFromList()
{
    QList<CChannel> list;
    CChannel tmp;
    foreach(QJsonObject json_obj, m_obj["list_objects"]){
          if(json_obj["type"] == CObject::CHANNEL){
              tmp.fromJSON(json_obj);
              list.push_back(tmp);
          }
    }
    return list;
}

QList<CMessage> CPacket::getMessagesFromList()
{

}

CObject* CPacket::deserialize(CObject::ObjectType type)
{

}

QList<CObject *> CPacket::deserializeList(CObject::ObjectType type)
{

}

sAuthentication CPacket::deserialize_auth()
{
    sAuthentication info;
    if(m_obj.contains("authentication")){
        QJsonObject auth_obj = m_obj.value("authentication").toObject();
        info.mail = auth_obj.value("mail").toString();
        info.password = auth_obj.value("password").toString();
    }
    return info;
}

sRegister CPacket::deserialize_reg()
{
    sRegister info;
    if(m_obj.contains("register")){
        QJsonObject auth_obj = m_obj.value("register").toObject();
        info.mail = auth_obj.value("mail").toString();
        info.username = auth_obj.value("username").toString();
        info.password = auth_obj.value("password").toString();
    }
    return info;
}



void CPacket::Deserialize(){

    //Get Type and Action
    if(m_obj.contains("mainObj")){
            QJsonObject mainObj = m_obj.value("mainObj").toObject();
            QJsonValue type = mainObj.value("type");
            QJsonValue action = mainObj.value("action");
            m_type = type.toString();
            m_action = action.toString();
    }

    //Get session cookie
    if(m_obj.contains("pClass")){
        m_cookie = m_obj.value("cookie").toString();
    }

    //Get session cookie
    if(m_obj.contains("pCookie")){
        m_cookie = m_obj.value("cookie").toString();
    }

    if(m_obj.contains("object")){
        //deserializeList();
    }

    if(m_obj.contains("list_objects")){
        //deserializeList();
    }
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

Type CPacket::getType() const
{
    return m_Type;
}

void CPacket::setType(const Type &Type)
{
    m_Type = Type;
}

int CPacket::getAction() const
{
    return m_Action;
}

void CPacket::setAction(int Action)
{
    m_Action = Action;
}

Request CPacket::getReq() const
{
    return m_req;
}

void CPacket::setReq(const Request &req)
{
    m_req = req;
}

ReqState CPacket::getReqState() const
{
    return m_reqState;
}

void CPacket::setReqState(const ReqState &reqState)
{
    m_reqState = reqState;
}

//UI
