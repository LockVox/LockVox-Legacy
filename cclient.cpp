#include "cclient.h"

CClient::CClient()
{
    m_pseudo = "";
    m_soc = NULL;
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

CClient::~CClient()
{

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



void  CClient::cInsertToDataStream(QDataStream & ds)
{

    qDebug() << "pseudo size : " << get_pseudo().length() << Qt::endl;

    ds << get_pseudo(); //16 - MAX 16 charactere

    ds.device()->seek(32); //Pos max name
    ds << get_idChannel();   //


    qDebug() << " ds : " << ds << Qt::endl;

}

void CClient::cExtractFromDataStream(QDataStream & ds)
{

    QString pseudo;
    int id;


    ds >> pseudo ;
    ds.device()->seek(32); //Pos max name
    ds >> id;

    this->set_pseudo(pseudo);
    this->set_idChannel(id);
}










