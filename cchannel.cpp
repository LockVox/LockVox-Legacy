#include "cchannel.h"

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

QDataStream & operator << (QDataStream & out, const CChannel & channel)
{
    out << channel.m_name
            << channel.m_nbClients
              << channel.m_maxUsers
                <<channel.m_id;
    return out;
}

QDataStream & operator >> (QDataStream & in, CChannel & channel)
{
    in >> channel.m_name;
    in >> channel.m_nbClients;
    in >> channel.m_maxUsers;
    in >> channel.m_id;

    return in;
}



void CChannel::initCChannelSystem(){

    qRegisterMetaTypeStreamOperators<CChannel>("CChannel"); //Define operator
    qRegisterMetaTypeStreamOperators<QList<CChannel>>("lCChannel"); //Define operator

    qMetaTypeId<CChannel>();
    qMetaTypeId<QList<CChannel>>();
    //Test validity of cclient class
}



void loadMessage()
{
    //BDD request - fill list of messages

    //select id, date, data from message where channel = id


}


