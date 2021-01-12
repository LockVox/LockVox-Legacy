#include "src/includes/cclient.h"

CClient::CClient()
{
    m_pseudo = "";
    m_soc = NULL;
    m_idChannel = 0;
}

CClient::~CClient()
{

}

CClient::CClient(const CClient & copy)
{
    m_pseudo = copy.m_pseudo;
    m_idChannel = copy.m_idChannel;
    m_soc = copy.m_soc;
}

CClient::CClient(QString name, QTcpSocket * soc, int id)
{
    m_userid = id;
    m_pseudo = name;
    m_soc = soc;
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

int CClient::GetUserID()
{
        return m_userid;
}


void CClient::set_pseudo(QString pseudo)
{
    m_pseudo = pseudo;
}

void CClient::set_socket(QTcpSocket * soc)
{
    m_soc = soc;
}

void CClient::set_id(int id){
    m_userid = id;
}

//Optionnal
/*QByteArray CClient::serialize(){

    //Create JSON object from client
    QJsonObject obj;
    obj["idChannel"]= this->get_idChannel();
    obj["pseudo"]= this->get_pseudo();


    //Cast JSON object in a JSON document
    QJsonDocument doc(obj);
    QByteArray out = doc.toJson();

    //qDebug() << "out document :" << doc << Qt::endl;

    return out;
}
/*void CClient::deserialize(QByteArray & in){
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


    this->set_idChannel(json_obj["idChannel"].toInt());
    this->set_pseudo(json_obj["pseudo"].toString());


    //qDebug() << "output: " << json_obj;
}

*/
