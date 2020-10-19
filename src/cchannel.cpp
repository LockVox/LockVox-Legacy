#include "src/includes/cchannel.h"

CChannel::CChannel()
{
    m_clients = {NULL};
    m_messages = {NULL};

    m_name = "";
    m_id = 0;
    m_maxUsers = 5;

}

CChannel::CChannel(const CChannel & copy){
    m_name = copy.m_name;
    m_nbClients = copy.m_nbClients;
    m_maxUsers = copy.m_maxUsers;
    m_id = copy.m_id;
}

CChannel::CChannel(QList<CClient*> clients, QList<CMessage*> msg, QString name, int id)
{
    m_clients = clients;
    m_messages = msg;
    m_name = name;
    m_id = id;

}



QList<CClient *> CChannel::get_clients()
{
    return m_clients;
}

QList<CMessage *> CChannel::get_message(){
    return m_messages;
}

QString CChannel::get_name(){
    return m_name;
}

int CChannel::get_id()
{
    return m_id;
}

int CChannel::get_maxUsers(){
    return m_maxUsers;
}

int CChannel::get_nbClients(){
    return m_nbClients;
}




void CChannel::set_clients(QList<CClient*> clients){
    m_clients = clients;
}

void CChannel::set_message(QList<CMessage*>message){
    m_messages = message;
}

void CChannel::set_name(QString name){
    m_name = name;
}

void CChannel::set_id(int id){
    m_id = id;
}

void CChannel::set_maxUsers(int maxUsers){
    m_maxUsers = maxUsers;
}

void CChannel::set_nbClients(int nb){
    m_nbClients = nb;
}

void CChannel::set_all(CChannel * c){
    this->set_id(c->get_id());
    this->set_name(c->get_name());
    this->set_maxUsers(c->get_maxUsers());
    this->set_nbClients(c->get_nbClients());

}




QByteArray CChannel::serialize(){

    //Create JSON object from channel
    QJsonObject obj;
    obj["id"]= this->get_id();
    obj["name"]= this->get_name();
    obj["maxUsers"]= this->get_maxUsers();
    obj["nbClients"]= this->get_nbClients();

    //Cast JSON object in a JSON document
    QJsonDocument doc(obj);
    QByteArray out = doc.toJson();

    //qDebug() << "out document :" << doc << Qt::endl;

    return out;
}

void CChannel::deserialize(QByteArray & in){

    QJsonDocument json_doc = QJsonDocument::fromJson(in);
    if(json_doc.isNull()){
        qDebug() << "Failed to create JSON document. " << Qt::endl;
    }
    if(json_doc.isObject()){
        qDebug() << "JSON isn't an object." << Qt::endl;
    }

    QJsonObject json_obj = json_doc.object();

    if(json_obj.isEmpty()){
        qDebug() << "JSON object is empty. " << Qt::endl;
    }


    this->set_id(json_obj["id"].toInt());
    this->set_name(json_obj["name"].toString());
    this->set_maxUsers(json_obj["maxUsers"].toInt());
    this->set_nbClients(json_obj["nbClients"].toInt());

    //qDebug() << "output: " << json_obj;
}

QJsonObject CChannel::serializeToObj(){
    QJsonObject obj;
    obj["id"]= this->get_id();
    obj["name"]= this->get_name();
    obj["maxUsers"]= this->get_maxUsers();
    obj["nbClients"]= this->get_nbClients();

    return obj;                                 //return json object
}

void CChannel::deserialize(QJsonObject json_obj){
    this->set_id(json_obj["id"].toInt());
    this->set_name(json_obj["name"].toString());
    this->set_maxUsers(json_obj["maxUsers"].toInt());
    this->set_nbClients(json_obj["nbClients"].toInt());
}
