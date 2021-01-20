#include "src/includes/cclient.h"

CClient::CClient()
{
    m_pseudo = "";
    m_soc = NULL;
    m_idChannel = 0;
    m_isOnline = false;
}


CClient::CClient( const CClient & copy)
{
    m_pseudo = copy.m_pseudo;
    m_idChannel = copy.m_idChannel;
    m_soc = copy.m_soc;
    m_isOnline = copy.m_isOnline;
}

CClient::CClient(int id,QString pseudo, QTcpSocket * soc, int idChannel, bool online)
{
    m_id = id;
    m_pseudo = pseudo;
    m_soc = soc;
    m_idChannel = idChannel;
    m_isOnline = online;
}

CClient::CClient( QTcpSocket * soc){
    m_soc = soc;
}



QString CClient::get_pseudo()
{
    return m_pseudo;
}

QTcpSocket * CClient::get_socket()
{
        return m_soc;
}

int CClient::get_idChannel(){
    return m_idChannel;
}

int CClient::get_id(){
    return m_id;
}


void CClient::set_pseudo(QString pseudo)
{
    m_pseudo = pseudo;
}

void CClient::set_socket(QTcpSocket * soc)
{
    m_soc = soc;
}

void CClient::set_idChannel(int id){
    m_idChannel = id;
}

void CClient::set_id(int id){
    m_id = id;
}

void CClient::set_all(CClient *c){
    this->set_id(c->get_id());
    this->set_pseudo(c->get_pseudo());
    this->set_idChannel(c->get_idChannel());
}

//Optionnal
QByteArray CClient::serialize(){

    //Create JSON object from client
    QJsonObject obj;
    obj["id"] = this->get_id();
    obj["idChannel"]= this->get_idChannel();
    obj["pseudo"]= this->get_pseudo();


    //Cast JSON object in a JSON document
    QJsonDocument doc(obj);
    QByteArray out = doc.toJson();

    //qDebug() << "out document :" << doc << Qt::endl;

    return out;
}
void CClient::deserialize(QByteArray & in){
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
    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());


    //qDebug() << "output: " << json_obj;
}

//Serialize | Deserialize
QJsonObject CClient::serializeToObj(){
    QJsonObject obj;
    obj["id"] = this->get_id();
    obj["idChannel"]= this->get_idChannel();
    obj["pseudo"]= this->get_pseudo();

    return obj;
}
void CClient::deserialize(QJsonObject json_obj){
    this->set_id(json_obj["id"].toInt());
    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());
}








