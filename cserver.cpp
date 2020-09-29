#include "cserver.h"

CServer::CServer()
{
    m_socket = new QTcpSocket(this);

    connect(m_socket,SIGNAL(readyRead()), this , SLOT(receiveData()));
    connect(m_socket,SIGNAL(connected()), this , SLOT(connect()));
    connect(m_socket,SIGNAL(disconnected()), this , SLOT(disconnect()));
    tailleMessage = 0;
}





void CServer::deconnexionClient()
{
    //Looking for sender -
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
        return;

    //Compare to clients list
    for( int i = 0 ; i  < m_clients.size(); i++){
        if(m_clients[i]->get_socket() == socket){
            free(m_clients[i]);
            m_clients.removeAt(i);
            qDebug() << "Client " << i << " has disconnect" << Qt::endl;
        }
    }

    //send msg to everybody to say someone disconnect (id client)

}



int CServer::addChannel(CChannel * tmp){
    m_channels.push_back(tmp);


    //can return something else -> permissions;
    return 0;
}


void CServer::FillDataStreamFromClient(QDataStream & ds){


    int size = m_clients.size();

    ds << size;
    for( int i = 0; i < size; i++)
    {
        m_clients[i]->cInsertToDataStream(ds);
    }
}

void CServer::FillDataStreamFromChannel(QDataStream & ds){

    int size = m_channels.size();

    ds << size;
    for( int i = 0; i < size; i++)
    {
        m_clients[i]->cInsertToDataStream(ds);
    }
}

#define PSEUDO_SIZE 32

void CServer::FillClientFromDataStream(QDataStream & ds)
{

        int size;

        ds >> size;


        for(int i = 0; i < size; i++)
        {
            m_clients[i]->cExtractFromDataStream(ds);          // Extracting datastream to fill client i
            ds.device()->seek(i*(PSEUDO_SIZE+1));              //Move cursor to next client
        }
}


void CServer::FillChannelFromDataStream(QDataStream & ds)
{

        int size;

        ds >> size;


        for(int i = 0 ; i < size; i++)
        {
            m_channels[i]->cExtractFromDataStream(ds);      //Extract ds into channel i
            ds.device()->seek(i*(PSEUDO_SIZE+3));           //Move cursor to next channel
        }
}


void CServer::receiveData(){

       QDataStream in(get_socket());

       int flag = -1;
       if(tailleMessage == 0)
       {
           if(get_socket()->bytesAvailable() < (int)sizeof(quint16)){
               return;
           }
           in >> flag;
           qDebug() << "Flag found : " << flag << Qt::endl;
           switch(flag){

               case 1:
                       //Not implement yet - send CServer object
                   break;

               case 2:
                    FillChannelFromDataStream(in);
                   break;
               case 3:
                    FillClientFromDataStream(in);
                   break;
               default:
                   qDebug() << "Flag not found - \n";
                   return;
               break;
           }
           //No more bytes ready for reading
           /*if(get_socket()->bytesAvailable() < tailleMessage){
               return;
           }*/
           tailleMessage = 0;
       }
}





QTcpSocket * CServer::get_socket(){
    return m_socket;
}


void CServer::set_socket(QTcpSocket* soc){
    m_socket  = soc;
}



























