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

/*CClient::CClient(int id, QString name, QTcpSocket * soc, int idChannel, bool online, QString description)
{
    m_id = id;
    m_pseudo = name;
    m_soc = soc;
    m_idChannel = idChannel;
    m_isOnline = online;
    m_description = description;
    m_isAuthenticate = false;
}*/

CClient::CClient(QUuid id,QString pseudo, QTcpSocket * soc, int idChannel, bool online, QString description)
{
    m_uuid = id;
    m_pseudo = pseudo;
    m_soc = soc;
    m_idChannel = idChannel;
    m_isOnline = online;
    m_description = description;
    m_isAuthenticate = false;
}

CClient::CClient( QTcpSocket * soc)
{
    m_soc = soc;
}

QString CClient::get_pseudo()
{
    return m_pseudo;
}

QString CClient::get_mail()
{
    return m_mail;
}

QTcpSocket * CClient::get_socket()
{
        return m_soc;
}

int CClient::get_idChannel(){
    return m_idChannel;
}

/*int CClient::get_id(){
    return m_id;
}*/

QUuid CClient::get_uuid()
{
    return m_uuid;
}

QImage CClient::get_profilePic()
{
    return profilePic;
}

bool CClient::get_isOnline()
{
    return m_isOnline;
}

bool CClient::get_isAuthenticate()
{
    return m_isAuthenticate;
}

QString CClient::get_description()
{
    return m_description;
};

void CClient::set_pseudo(QString pseudo)
{
    m_pseudo = pseudo;
}

void CClient::set_mail (QString mail)
{
    m_mail = mail;
}

void CClient::set_socket(QTcpSocket * soc)
{
    m_soc = soc;
}

void CClient::set_idChannel(int id){
    m_idChannel = id;
}


void CClient::set_isOnline(bool online)
{
    m_isOnline = online;
}

void CClient::set_isAuthenticate(bool Auth)
{
    m_isAuthenticate = Auth;
}

void CClient::set_description(QString d)
{
    m_description = d;
};

void CClient::set_uuid(QUuid uuid)
{
    m_uuid = uuid;
}

void CClient::set_profilePic(QImage img)
{
    profilePic = img;
}

void CClient::set_all(CClient *c){
    this->set_uuid(c->get_uuid());
    this->set_pseudo(c->get_pseudo());
    this->set_idChannel(c->get_idChannel());
    this->set_mail(c->get_mail());
}

//Optionnal
QByteArray CClient::serialize(){

    //Create JSON object from client
    QJsonObject obj;
    obj["uuid"] = this->get_uuid().toString();
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
        qDebug() << "Failed to create JSON document." << Qt::endl;
    }
    if(json_doc.isObject()){
        qDebug() << "JSON isn't an object." << Qt::endl;
    }

    QJsonObject json_obj = json_doc.object();

    if(json_obj.isEmpty()){
        qDebug() << "JSON object is empty. " << Qt::endl;
    }

    QUuid tmp = QUuid::fromString(json_obj["uuid"].toString());
    this->set_uuid(tmp);
    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());


    //qDebug() << "output: " << json_obj;
}

//Serialize | Deserialize
QJsonObject CClient::serializeToObj(){
    QJsonObject obj;
    obj["uuid"] = this->get_uuid().toString();
    obj["idChannel"]= this->get_idChannel();
    obj["pseudo"]= this->get_pseudo();
    obj["isOnline"] = this->get_isOnline();

    return obj;
}
void CClient::deserialize(QJsonObject json_obj){
    QUuid tmp = QUuid::fromString(json_obj["uuid"].toString());
    this->set_uuid(tmp);
    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());
    this->set_isOnline(json_obj["isOnline"].toBool());
}









