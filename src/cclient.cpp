#include "src/includes/cclient.h"

CClient::CClient()
{
    m_pseudo = "";
    m_soc = NULL;
    m_idChannel = 0;
}


CClient::CClient( const CClient & copy)
{
    m_pseudo = copy.m_pseudo;
    m_idChannel = copy.m_idChannel;
    m_soc = copy.m_soc;
}

CClient::CClient(QString pseudo, QTcpSocket * soc, int id)
{
    m_pseudo = pseudo;
    m_soc = soc;
    m_idChannel = id;

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


void CClient::initCClientSystem(){

    qRegisterMetaTypeStreamOperators<CClient>("CClient"); //Define operator
    qRegisterMetaTypeStreamOperators<QList<CClient>>("QList<CClient>"); //Define operator
    qMetaTypeId<QList<CClient>>();                                 //Test validity of cclient class
    qMetaTypeId<CClient>();
}

QDataStream & operator << (QDataStream & out, const CClient & client){
    out << client.m_pseudo
        << client.m_idChannel;
    return out;
}

QDataStream & operator >> (QDataStream & in, CClient & client){
    in >> client.m_pseudo;
    in >> client.m_idChannel;

    return in;
}











