#include "cchannel.h"

CChannel::CChannel()
{
    m_clients = {NULL};
    m_messages = {NULL};

    m_name = "";
    m_id = 0;
    m_maxUsers = 5;

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


void  CChannel::cInsertToDataStream(QDataStream & ds)
{

    qDebug() << "Name size : " << get_name().length() << Qt::endl;

    ds << get_name(); //16 - MAX 16 charactere

    ds.device()->seek(32); //Pos max name
    ds << get_maxUsers();   //
    ds << get_id();
    ds << get_nbClients();

    qDebug() << " ds : " << ds << Qt::endl;
}

void CChannel::cExtractFromDataStream(QDataStream & ds)
{

    QString name;
    int nbClients;
    int maxUsers;
    int id;
    ds >> maxUsers >> id >> nbClients;

    ds >> name ;
    ds.device()->seek(32); //Pos max name

    this->set_name(name);
    this->set_nbClients(nbClients);
    this->set_maxUsers(maxUsers);
    this->set_id(id);
}



